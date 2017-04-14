
/*
 * bbs.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * BBS entry-point, main loop, and tear-down/cleanup routines.
 */

// $Id: bbs.c 27 2011-08-08 17:02:49Z mholger $

#define _DEFINE_GLOBALS_

#include "vars.h"

int main( int argc, char **argv )
{
    int argx = 0, runmode = 0;
	char *lmargs = "MAIN";
	char cfgfile[255];
	/* Need getopts in here...? */

	sprintf(cfgfile, "config.dat");

	for( argx = 0; argx < argc; argx++ )
	{
		if( argv[argx][0] == '-' ) {
			switch( argv[argx][1] ) {
				case 'i':
					runmode = 1;
					break;
				case 'c':
					argx++;
					strcpy( cfgfile, argv[argx] );
					break;
			}
		}
	}

	_hangup = 0;
	_timeout = 0;

	_echo = 1;		// Enable input echoing...
	mciok = 1;		// Turn on pipe codes
	expert = 0;		// Default MCI support to enabled...
	lines_listed = 0;	// For the screen-pause counter...
	okcolor = 1;		// Color is ok until we have a reason otherwise.
	initring( 128 );	// 128-byte ring buffer.  Waaay overkill.
	configinit(cfgfile);	// Load/process system configuration.
	mciinit();	// Setup MCI codes
	cominit();		// Setup tty in unbuffered raw mode

	if( runmode == 1 ) {
        ansi = 1;
		initdata(cfgfile);
        bbsexit(0);
    }

	openlog();
	userinit( &thisuser );	// Initialize default user record
	//initsub( &sub );

	plugininit();	// Load plugins
	menuinit();		// Load menu configurations

	// Begin USER I/O!
	pnl();
	outstr( bbs_long_name );
	nln(2);
	if( detectansi() )	// See if ANSi is available
		confirmansi();

	nln(2);
	//clrscreen();		// Clear the screen (if we can)
	outstr( bbs_version );	// Print the BBS version info
	nln( 3 );		// And a few newlines...
	outfile( "logon" );	// Spit out the login screen
	pausescr();		// And then ... wait!

	if( login() )
	{
		loadmenu( 1, &lmargs );
		// Main Loop
		while( !_hangup && !_timeout )
		{
			//menu( state.menu );
			menu( 1 );
		}
	}

	bbsexit( 0 );
	return 0;
}

/* bbsexit( exitcode )
 * Performs BBS tear-down, memory release, cleanup, etc.
 */
void bbsexit( int e )
{
	if( e )
		printf( "ERROR %i\n", e );

	freering();		// Free up the ring buffer
	comreset();		// Flip terminal back to regular I/O mode (for consoles)
	plugin_free();	// Free memory allocated for plugin structures
	mci_free();		// Free memory allocated for MCI codes
	closelog();
	exit( e );		// Exit to OS!
}

/* logoff( argc, argv )
 * mnemonic callback to save user state and perform logoff procedures
 */
void logoff( int argc, char **argv )
{
	int fast = 0;

	if( argc )
		fast = atoi( argv[0] );

	pnl();
	if( !fast )
	{
		outstr( strings[S_PROMPT_LOGOFF] );
		if ( ny() != 'Y' )
		{
			return;
		}
		outfile( "logoff" );
		pausescr();
		pnl();
		
	}
	pnl();
	resetcolor();
	bbsexit( 0 );
}

int login( void )
{
	char un[255], up[255];
	int npcount = 0, authenticated = 0, usernum = 0;
	userrec_t u;


	do {
        clean(un);
        clean(up);
		npcount++;
		outstr( strings[S_MSG_LOGIN] );
		mpl( 20 );
		inputw( un, sizeof( u.username ), 20 );

		if( !strcmp( un, "NEW" ))
		{
			newuser();
			strcpy( un, thisuser.username );
		}

		usernum = findusernum(atoi(un));
		if(usernum > 0) {
			usernum = loaduser(usernum, &u);
		}
		else {
			usernum = loaduser(finduser(un), &u);
		}
		logger(9, "login(): findusernum(atoi(%i))=%i", atoi(un), usernum);
		if( !strcmp( "", un ) || !usernum)
		{
			logger( 9, "login(): Couldn't find user '%s'", un );
			usernum = -1;
			userinit(&u);
		}

		logger( 9, "login(%i): '%s'", u.userid, u.username );
		outstr( strings[S_MSG_PASSWORD] );
		mpl( 20 );
		_echo = 0;
		inputw( up, sizeof( u.password ), 20 );
		_echo = 1;
        logger( 9, "login(): '%s' '%s'", un, up );
        authenticated = checkpass( u.userid, up );
	} while( !authenticated && npcount < 3 );

	if( usernum > 0 && authenticated )
	{
		if( u.flags & USER_SYSOP ) {
			logger( 1, "login(): SYSOP ON DECK" );
		}
		return( 1 );
	}

	logger(5, "login(): usernum=%i, authenticated=%i, u.userid=%i", usernum, authenticated, u.userid);
	pnl();
	outstrnl( strings[S_MSG_SORRY_BYE] );
	bbsexit( 0 );
	return( 0 );
}

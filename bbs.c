
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
	char *lmargs = "MAIN";
	/* Need getopts in here...? */

	_hangup = 0;
	_timeout = 0;

	/* These need implementation...! */
	//loadconfig( &cfg );
	configinit();	// Load/process system configuration.
	openlog();
	cominit();		// Setup tty in unbuffered raw mode
	userinit( &thisuser );	// Initialize default user record
	//initsub( &sub );
	_echo = 1;		// Enable input echoing...
	mciok = 1;		// Turn on pipe codes
	expert = 0;		// Default MCI support to enabled...
	lines_listed = 0;	// For the screen-pause counter...
	okcolor = 1;		// Color is ok until we have a reason otherwise.
	initring( 128 );	// 128-byte ring buffer.  Waaay overkill.

	plugininit();	// Load plugins
	mciinit();	// Setup MCI codes
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
	outfile( "screens/logon" );	// Spit out the login screen
	pausescr();		// And then ... wait!

	loadmenu( 1, &lmargs );
	// Main Loop
	while( !_hangup && !_timeout )
	{
		//menu( state.menu );
		menu( 1 );
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
		outfile( "screens/logoff" );
		pausescr();
		pnl();
	}
	pnl();
	resetcolor();
	bbsexit( 0 );
}


/*
 * ansic. - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Functions and procedures surrounding the generation and interpretation
 * of ANSI-style terminal control codes.
 */

// $Id: ansi.c 26 2010-09-02 02:40:23Z mholger $

#include "vars.h"

/* makeansi( string )
 * Takes MCI-code color arguments and turns them into ANSI escape sequences
 */
char* makeansi( char *s )
{
	char cbuf[20], buf[255];
	unsigned int co, tfg = 0, tbg = 0, thic = 0;
	unsigned int nsc = 0;

	memset( cbuf, '\0', 20 );

	if( upcase( s[0] ) == 'B' )
	{
		s[0] = s[1];
		s[1] = 0;
		co = atoi( s ) + 16;
	}
	else
		co = atoi( s );


	if( co >= 16 && co <= 23 ) {
		tfg = fgcolor;
		thic = hicolor;
		tbg = co - 16;
	}
	else {
		tbg = bgcolor;
		tfg = co;
	}

	if( tbg == bgcolor ) {
		if( tfg > 7 && tfg < 16 ) {
			thic = 1;
			tfg -= 8;
		}

		if( thic != hicolor ) {
			if( nsc )
				strcat( cbuf, ";" );

			sprintf( buf, "%i", thic );
			strcat( cbuf, buf );
			nsc = 1;
			hicolor = thic;
		}

		if( co != fgcolor || thic != hicolor ) {
			if( nsc )
				strcat( cbuf, ";" );

			sprintf( buf, "3%i", tfg );
			strcat( cbuf, buf );
			nsc = 1;
			fgcolor = co;
		}
	}
	else {
		if( nsc )
			strcat( cbuf, ";" );

		sprintf( buf, "4%i", tbg );
		strcat( cbuf, buf );
		nsc = 1;
		bgcolor = tbg;
	}

	if( strlen( cbuf ) && ansi )
		sprintf( s, "\x1b[%sm", cbuf );
	else
		memset( s, '\0', strlen( s ));

	return( s );
}

/* goxy( x, y )
 * Move cursor to screen column x, row y
 */
void goxy( int x, int y )
{
	if( ansi )
		outstr( "\x1b[%d;%dH", y, x );
}

/* resetcolor()
 * Sets output color to flat grey, if user has ANSI enabled.
 */
void resetcolor( void )
{
	if( ansi )
		outstr( "\x1b[0m" );
	
	fgcolor = 7;
	bgcolor = 0;
	hicolor = 0;
}

/* clrscreen()
 * For ANSI-enabled users, clears the screen and locates the cursor at (0,0)
 */
void clrscreen( void )
{
	if( !ansi )
		return;

	outstr( "\x1b[2J" );
	goxy( 0, 0 );
}

/* detectansi()
 * Queries the user's terminal for ANSI-support; returns 1 if available, else 0.
 */
int detectansi( void )
{
	char buf[25];
	int c, i = 0, deltaclock = 0;
	time_t clock1, clock2;

	ansi = 0;

	memset( buf, '\0', 25 );
	outstr( "%s", strings[S_DETECT_ANSI] );
	outstr( "\x1b[5n" );	// Request device status
	
	time( &clock1 );
	while( deltaclock < .5 && !ansi )
	{
		// kbhit() tells us if anything is in the ring buffer.
		// kbhit() drains the system input buffer into the ring
		// buffer, thus ensuring that if there's something to get,
		// we can get it.
		while( kbhit() )
		{
			c = get1ch();
			if( c )
			{
				buf[i++] = (char) c;
				c = 0;

				// Responses: \x1b[0n: OK; \x1b[3n: Failure
				if( !strcmp( buf, "\x1b[0n" ))
				{
					ansi = 1;
					outstr( "%s", strings[S_ANSI_DETECTED] );
					resetcolor();
				}
			}
		}
		deltaclock = ( time( &clock2 ) - clock1 );
	}

	/* Try again, using getxy() method */
	/* mTelnet responds to this - but not the actual spec request? */
	if( !ansi )
	{
		time( &clock1 );
		deltaclock = 0;
		outstr( "\x1b[6n" );
		while( deltaclock < .5 && !ansi )
		{
			while( kbhit() )
			{
				c = get1ch();
				if( c )
				{
					buf[i++] = (char) c;
					c = 0;
				}
			}

			if( strstr( buf, "\x1b[" ) != NULL ) /* good enough ... */
			{
				ansi = 1;
				outstr( "%s", strings[S_ANSI_DETECTED] );
				resetcolor();
			}
			deltaclock = ( time( &clock2 ) - clock1 );
		}
	}

	if( !ansi )
		outstr( "%s\n", strings[S_NO_ANSI] );

	return( ansi );
}

/* confirmansi()
 * Gives the user a chance to opt-out of ANSI; good for debugging, but why
 * else would you ever encounter a no-ANSI situation in this day and age?
 */
void confirmansi()
{
	int c;

	outstr( strings[S_KEEP_ANSI] );
	c = yesno( 0 );
	switch( c )
	{
		case 'N':
			nln(2);
			outstr( "Disabling ANSi..." );
			pnl();
			ansi = 0;
			break;
		case 'Y':
		default:
			break;
	}
	return;
}

void ansidemo( int argc, char **argv )
{
	int x;

	for( x = 1; x <= 23; x++ )
	{
		outstr( "|%02i%02i|07|NL", x, x );
	}
	resetcolor();
	nln( 2 );
	pausescr();
}

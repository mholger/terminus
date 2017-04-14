
/*
 * com.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Low-level communications and I/O routines.
 */

// $Id: com.c 24 2010-08-27 11:24:04Z mholger $

#include "vars.h"

void out1ch( char c ) /* com.c */
{
	write( 1, &c, sizeof( c ));
	fflush( stdout );
}

void cominit( void )
{
	struct termios params;

	// Save old params for resetting...
	tcgetattr( 0, &oldparms );
	params = oldparms;

	cfmakeraw( &params );

	tcsetattr( 0, TCSANOW, &params );
	setvbuf( stdout, (char *)NULL, _IONBF, 0 );
	setvbuf( stdin, (char *)NULL, _IONBF, 0 );
}

// Reset terminal to original parameters before dying...
void comreset( void )
{
	tcsetattr( 0, TCSAFLUSH, &oldparms );
}

/* Taken from Linux Journal */
int _kbhit(void)
{
	struct timeval tv;
	fd_set read_fd;
	static clock_t cld = 0;
	clock_t cnow;
	static int tlast;
	int tnow;
	struct timeval tv_now;
	long tsleep;

	if( !cld ) {
		cld = clock();
		gettimeofday(&tv_now, NULL);
		tlast = tv_now.tv_sec;
	}

	cnow = clock();
	gettimeofday(&tv_now, NULL);
	tnow = tv_now.tv_sec;

	if((tnow - tlast) > cfg.timeout) {
		timeout();
	}

	tsleep = ( kdelay - ( cnow - cld ));
	if( tsleep < 0 )	// This should never happen ...
		tsleep = 0;

	if( tsleep )
		usleep( tsleep );

	/* Do not wait at all, not even a microsecond */
	tv.tv_sec=0;
	tv.tv_usec=outdelay;

	/* Must be done first to initialize read_fd */
	FD_ZERO(&read_fd);

	/* Makes select() ask if input is ready:
	 * 0 is the file descriptor for stdin    */
	FD_SET(0,&read_fd);

	/* The first parameter is the number of the
	 * largest file descriptor to check + 1. */
	if(select(1, &read_fd, NULL, NULL, &tv) == -1)
	{
		_hangup = 1;
		return 0;  /* An error occured */
	}

	/*  read_fd now holds a bit map of files that are
	 * readable. We test the entry for the standard
	 * input (file 0). */
	if(FD_ISSET(0,&read_fd)) {
		/* Character pending on stdin */
		cld = cnow;
		tlast = tnow;
		return 1;
	}

	/* no characters were pending */
	return 0;
}

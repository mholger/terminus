
/*
 * rbuf.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * The ringbuffer and associated routines.  These are lower-level drivers
 * for the routines in com.c, and everything above it, in terms of reading
 * input from the user.
 */

// $Id: rbuf.c 26 2010-09-02 02:40:23Z mholger $

#include "vars.h"

/* Initialize the ring buffer to the specified size... */
void initring( unsigned int sz )
{
	// Initialize the ring buffer...
	ring.sz = sz;
	ring.buf = malloc( ring.sz * sizeof( unsigned int ) + 1);	// Give us an sz-byte buffer

	if( !ring.buf )
		bbsexit( ENOMEM );

	ring.iptr = ring.optr = 0;	// Buffer is empty.
	memset( ring.buf, '\0', ring.sz );	// Make the buffer really empty
}

void rflush( void )
{
	memset( ring.buf, '\0', ring.sz * sizeof( unsigned int ));
}

/* Push a character into the buffer */
void stash( int c )
{
	// This is DESTRUCTIVE and will overwrite unread buffered data
	//ring.buf[ ring.iptr ] = c;
	//ring.iptr = ( ++ring.iptr % ring.sz );

	// This is NON-DESTRUCTIVE, but LOSES DATA once the buffer fills
	if(( ring.iptr != ring.optr ) || ( ring.buf[ ring.iptr ] == 0 ))
	{
		ring.buf[ ring.iptr ] = c;
		ring.iptr = ( (ring.iptr + 1) % ring.sz );
	}
}

/* Pop and return the top character from the buffer */
int unstash( void )
{
	char c = 0;

	if( ring.iptr != ring.optr || ring.buf[ ring.optr ] != 0 )
	{
		c = ring.buf[ ring.optr ];
		//ring.buf[ ring.optr ] = '\0';
		ring.buf[ ring.optr ] = 0;
		ring.optr = ( (ring.optr + 1) % ring.sz );
	}

	return( c );
}

/* Return the top of the ring buffer without "popping" it off the stack. */
int peek1ch( void )
{
	if( ring.iptr != ring.optr || ring.buf[ ring.optr ] != 0 )
		return( ring.buf[ ring.optr ] );
	else
		return( 0 );
}

char kbhit( void )
{
	int tsleep;
	clock_t c1 = 0;
	clock_t c2;

	if( !c1 )
		c1 = c2 = clock();

	while( _kbhit() ) {
		stash( getchar() );
	}

	tsleep = kdelay - ( c2 - c1 );
	if( tsleep < 0 )
		tsleep = 0;
	usleep( tsleep );
	c1 = c2;
	c2 = clock();

	return(( ring.iptr != ring.optr ) || ( ring.buf[ ring.optr ] != 0 ));
}

void freering( void )
{
	if( ring.buf )
		free( ring.buf );
}

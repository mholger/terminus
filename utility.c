
/*
 * utility.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Generic utility functions, largely not even BBS-specific.
 */

// $Id: utility.c 26 2010-09-02 02:40:23Z mholger $

#include "vars.h"

/* I wrote this routine, and I now say I barely understand the arcane
 * depths from whence it sprung.  The idea was to quickly strtok() an
 * entire string into an array, allay split() or somesuch from other,
 * higher-level languages.
 *
 * The results here are reliable - but its memory utilization is
 * obscene and its invocation is invariably surrounded by gremlins.
 * Use at your own risk.
 */
char **toktok( char *s, char sep, int *cnt )
{
	static char *toktok_[MAXTOK], *buf;
	int i = 0;

	//if( !buf )
	//	buf = (void *)malloc( strlen( s ));

	buf = realloc( buf, strlen( s ) + 1);
	memset( buf, '\0', strlen( s ));
	sprintf( buf, "%s", s );
	memset( toktok_, '\0', sizeof( toktok_ ));
	toktok_[i++] = strtok( buf, &sep );
	while(( toktok_[i++] = strtok( NULL, &sep )) != NULL );
	toktok_[i] = NULL;
	*cnt = i - 1;
	return( toktok_ );
}

int exists( char *fn )
{
	struct stat t;

	if( stat( fn, &t ) != -1 )
		return( 1 );
	else
		return( 0 );
}

int direxists( char *fn )
{
    struct stat t;

    if( stat( fn, &t ) != -1 && S_ISDIR( t.st_mode ))
        return( 1 );
    else
        return( 0 );
}

char *stripcolor( char *s )
{
	int len = 0, x = 0;
	static char buf[255];

	memset( buf, '\0', sizeof( buf ));
	while( s[x] )
	{
		if(( s[x+1] && s[x+2] ) && ismcicode( s[x], s[x+1], s[x+2] ))
			x += 3;
		else
			buf[len++] = s[x++];
	}
	while( buf[len] )
		buf[len++] = 0;

	return( buf );
}

char *neutralizecolor( char *s )
{
	int len = 0;
	static char buf[255];

	memset( buf, '\0', sizeof( buf ));
	while( *s )
	{
		if( ismcicode( *s, *(s+1), *(s+2) ))
			buf[len++] = '\\';
		else if( *s == '\\' )
			buf[len++] = '\\';
		buf[len++] = *(s++);
	}
	return( buf );
}

int stripstrlen( char *s )
{
	char t[255];
/*	int len = 0, x = 0;

	while( s[x] != 0 )
	{
		if( ismcicode( s[x], s[x+1], s[x+2] ))
			x += 3;
		else
		{
			x++;
			len++;
		}
	}
	return( len );
	*/
	sprintf( t, "%s", stripcolor( s ));
	return( strlen( t ));
}

char upcase( char c )
{
	if( c >= 97 && c <= 122 )
		c -= 32;

	return( c );
}

void clean( char *s )
{
    bzero(s, strlen(s));
}


/*
 * msgbase.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Message storage, parsing and display-related functions, including MCI decoding.
 */

// $Id: msgbase.c 24 2010-08-27 11:24:04Z mholger $

#include "vars.h"

int ismcicode( char c1, char c2, char c3 )
{
	char code[2];

	if( c1 == '|' )
	{
		sprintf( code, "%c%c", toupper( c2 ), toupper( c3 ));
		mci = mcicode_head;
		while( mci )
		{
			if( !strcmp( mci->code, code ))
				return(1);
			mci = mci->next;
		}
	}
	return( 0 );
}

char* mcidecode( char *s )
{
	char s2[255];
	static char buf[255];
	int cok;

	if( !mciok )
		return( s );

	cok = okcolor;
	okcolor = 0;
	memset( buf, '\0', 255 );

	sprintf( buf, "%s", s );

	// This should cover color codes...
	if(( s[1] >= '0' && s[1] <= '2' ) && ( s[2] >= '0' && s[2] <= '9' ))
	{
		sprintf( s2, "%c%c", s[1], s[2] );
		sprintf( buf, "%s", makeansi( s2 ));
		return( buf );
	}
	else
		// And here goes MCI codes:
		switch( s[1] )
		{
			case '!':
				switch( s[2] )
				{
					case 'R':
						memset( buf, '\0', strlen( buf ));
						resetcolor();
						break;
					case 'P':
						pausescr();
						break;
				}
				break;
			case 'N':
				switch( s[2] )
				{
					case 'L':
						memset( buf, '\0', strlen( buf ));
						sprintf( buf, "\r\n" );
						return( buf );
						break;
				}
				break;
			case 'S':
				switch( s[2] )
				{
					case 'V':
						memset( buf, '\0', strlen( buf ));
						return( bbs_version );
						break;
				}
				break;
		}
	return( buf );
	okcolor = cok;
}

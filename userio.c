
/*
 * userio.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * high-level routines for interacting with the user.  Most user-facing
 * interactions will use these functions, or build upon these functions -
 * there shouldn't be much (if any) reason to go into the lower-level routines
 * in com.c or rbuf.c.
 */

// $Id: userio.c 27 2011-08-08 17:02:49Z mholger $

#include "vars.h"

void outchr( char c ) /* userio.c */
{
	if(( c >= 33 && c <= 126 ) || ( c == ' ' ))
		_echo ? out1ch( c ) : out1ch( strings[S_HIDDEN_FILL][ansi] );
	else
		out1ch( c );
}

void outstr( char *fmt, ... ) /* userio.c */
{
	va_list ap;
	char s[4096];

	va_start(ap, fmt);
	vsprintf(s, fmt, ap);
	va_end(ap);

	out1str( s );
}

void outstrnl( char *fmt, ... ) /* userio.c */
{
	va_list ap;
	char s[4096];

	va_start(ap, fmt);
	vsprintf(s, fmt, ap);
	va_end(ap);

	out1str( s );
	pnl();
}

void out1str( char *s )
{
	char buf[20], cbuf[3], out[4096], _out[4096];
	int x = 0, lastslash = 0, wd;

	memset( out, '\0', strlen( out ));
	while( *s )
	{
		if( *s == '\\' && !lastslash )
			lastslash = 1;
		else
			lastslash = 0;

		if( ismcicode( *s, *(s+1), *(s+2) ) && !lastslash )
		{
			cbuf[0] = *(s++);
			cbuf[1] = *(s++);
			cbuf[2] = *(s++);
			cbuf[3] = '\0';
			strncpy( buf, mcidecode( cbuf ), sizeof( buf ));
			sprintf( _out, "%s%s", out, buf );
			strncpy( out, _out, sizeof( out ));
			x += strlen( buf );
		}
		else if( *s == '\n' && *(s-1) != '\r' )
		{
			sprintf( _out, "%s\r%c", out, *(s++) );
			strncpy( out, _out, sizeof( out ));
		}
		else if( *s == '\\' && *(s+1) == '|' )
		{
			s++;
		}
		else
		{
			sprintf( _out, "%s%c", out, *(s++) );
			strncpy( out, _out, sizeof( out ));
		}
	}

	wd = write( 1, out, strlen( out ));

	if( wd == -1 || wd == EPIPE )
		_hangup = 1;
}

int outfile( char *fn ) /* userio.c */
{
	char fnascii[255], fnansi[255], buf[255], _fn[255], file[255];
	FILE *fp;

	sprintf( _fn, "%s/%s", cfg.textpath, fn );
	sprintf( fnascii, "%s/%s.txt", cfg.textpath, fn );
	sprintf( fnansi, "%s/%s.ans", cfg.textpath, fn );

	if( exists( _fn ))
		sprintf( file, "%s", _fn );
	else if( ansi && exists( fnansi ))
		sprintf( file, "%s", fnansi );
	else if( exists( fnascii ))
		sprintf( file, "%s", fnascii );
	else
		return 0;

	if(( fp = fopen( file, "r" )) == NULL )
	{
		logger( 1, "outfile(): Couldn't open '%s' for reading\n", file );
		abort();
	}

	while( fgets( buf, sizeof( buf ), fp ) != NULL )
		outstr( buf );
	
	fclose( fp );
	return 1;
}

void pnl( void ) /* userio.c */
{
	outstr( "\n" );
}

void nln( int n ) /* userio.c */
{
	int i;

	for( i = 0; i < n; i++ )
		pnl();
}

int getkey( void ) /* userio.c */
{
	int c = 0,buf[4];

	memset( buf, '\0', 4 );
	while( c == 0 )
		c = get1ch();

	if( c == '\x1b' && kbhit() ) {
		buf[0] = c;
		if( peek1ch() == '[' ) {
			buf[1] = get1ch();
			buf[2] = get1ch();
			switch( buf[2] )
			{
				case 'A':
					c = KEY_ARROW_UP;
					break;
				case 'B': /* Down */
					c = KEY_ARROW_DOWN;
					break;
				case 'C': /* Right */
					c = KEY_ARROW_RIGHT;
					break;
				case 'D': /* Left */
					c = KEY_ARROW_LEFT;
					break;
				case '1': /* Home */
					while( peek1ch() != 0 )
						get1ch();
					c = KEY_HOME;
					break;
				case '2': /* Ins */
					if( !insert )
						insert = 1;
					else
						insert = 0;
					break;
				case '3': /* Del */
					c = KEY_DEL;
					break;
				case '4': /* End */
					c = KEY_END;
					break;
				case '5': /* PgUp */
					break;
				case '6': /* PgDn */
					break;
				case '[': /* F1- thru F5- keys */
					buf[3] = get1ch();
					break;
			}
			if( peek1ch() == '~' )
				get1ch();
		}
	}
	return( c );
}

int get1ch( void ) /* userio.c */
{
	if( kbhit() )
		return( unstash() );
	else
		return( 0 );
}

void mpl( int n ) /* userio.c */
{
	int x = 0, y;

	if( ansi )
	{
		outstr( strings[S_MPL_PREFIX] );
		outstr( strings[S_MPL_FILL_COLOR] );
		while( x++ < n )
			outchr( strings[S_MPL_FILL][ansi] );
		outstr( strings[S_MPL_SUFFIX] );
		for( y = 0; y < stripstrlen( strings[S_MPL_SUFFIX] ); y++ )
			outchr( '\b' );
		while( --x > 0 )
			outchr( '\b' );
	}
}

void inputc( char *s, int maxlen ) /* userio.c */
{
	input1( s, maxlen, 2, 0 );
}

void inputl( char *s, int maxlen ) /* userio.c */
{
	input1( s, maxlen, 1, 0 );
}

void input( char *s, int maxlen ) /* userio.c */
{
	input1( s, maxlen, 0, 0 );
}

void inputw( char *s, int maxlen, int window )
{
	input1( s, maxlen, 0, window );
}

void inputwl( char *s, int maxlen, int window )
{
	input1( s, maxlen, 1, window );
}

void inputwc( char *s, int maxlen, int window )
{
	input1( s, maxlen, 2, window );
}

void prompt( char *pstr, char *s, int maxlen, int window )
{
	outstr( pstr );
	mpl( window );
	inputw( s, maxlen, window );
}

void promptl( char *pstr, char *s, int maxlen, int window )
{
	outstr( pstr );
	mpl( window );
	inputwl( s, maxlen, window );
}

void promptc( char *pstr, char *s, int maxlen, int window )
{
	outstr( pstr );
	mpl( window );
	inputwc( s, maxlen, window );
}

void promptp( char *pstr, char *s, int maxlen, int window )
{
	outstr( pstr );
	mpl( window );
	_echo = 0;
	inputw( s, maxlen, window );
	_echo = 1;
}

void input1( char *s, int maxlen, int mode, int window ) /* userio.c */
{
	int x = 0, cursor = 0, cx, boxx = 0, boxy;
	int c;
    char tmp[maxlen+1];

	/* Window is for input areas that accept more text than they can show;
	 * ie: input area is 20 characters wide, but input text can be 80 -
	 * maxlen = 80; window = 20.
	 */

    strcpy( tmp, s );
    clean(s);
    strcpy( s, tmp );
    clean(tmp);
    x = strlen(s);

	if( !window )
		window = maxlen;
    else {
        if( x > window ) {
            cx = x - window;
            boxx = cx;
        }
        else {
            cx = 0;
        }
        while( cx < x ) {
            outchr(s[cx++]);
        }
    }

	boxy = boxx + window;

	do
	{
		c = 0;
		if( kbhit())
			c = getkey();

		if( mode == 2 )
			if( c >= 'a' && c <= 'z' )
				c -= 32;
		if( mode == 1 )
			if( c >= 'A' && c <= 'Z' )
				c += 32;

		if( c == 13 || c == 10 )
		{
			okcolor = 1;
			resetcolor();
			outstr( "\n" );
			return;
		}
		else if(( c == KEY_DEL ) && x < strlen( s ) && ansi )
		{
			cursor = x+1;
			cx = strlen( s );
			while( cursor <= cx )
			{
				s[cursor-1] = s[cursor];
				cursor++;
			}
			s[cursor] = 0;

			cx = x - boxx + 1; /* cx = current cursor location in the box */
			while( --cx > 0 )   /* Move to beginning of box */
				outchr( '\b' );

			/* Recalculate window, if 'x' is outside box */
			if( x < boxx )
			{
				boxx = x;
				boxy = boxx + window;
			}

			/* Display as much string as possible, within the box */
			while( cx < window && s[cx+boxx] )
			{
				outchr( s[cx+boxx] );
				cx++;
			}

			if( cx < boxy )
				outstr( "%s%c%s\b", strings[S_MPL_FILL_COLOR], strings[S_MPL_FILL][ansi], strings[S_MPL_TEXT_COLOR] );

			while( cx-- > (x - boxx) )    /* Return cursor to where it started */
				outchr( '\b' );
		}
		else if(( c == 8 || c == 127 ) && x > 0 )
		{
			cursor = x;
			cx = strlen( s );
			//outchr( '\b' );
			while( cursor <= cx )
			{
				s[cursor-1] = s[cursor];
				cursor++;
			}
			s[cursor] = 0;

			cx = x - boxx + 1; /* cx = current cursor location in the box */
			x--;
			while( --cx > 0 )   /* Move to beginning of box */
				outchr( '\b' );

			/* Recalculate window, if 'x' is outside box */
			if( x < boxx )
			{
				boxx = x;
				boxy = boxx + window;
			}

			/* Display as much string as possible, within the box */
			while( cx < window && s[cx+boxx] )
			{
				outchr( s[cx+boxx] );
				cx++;
			}

			if( cx < boxy )
				outstr( "%s%c%s\b", strings[S_MPL_FILL_COLOR], strings[S_MPL_FILL][ansi], strings[S_MPL_TEXT_COLOR] );

			while( cx-- > (x - boxx) )    /* Return cursor to where it started */
				outchr( '\b' );
		}
		else if( c >= 32 && c <= 126 && strlen( s ) < maxlen )
		{
			if( !insert )
			{
				s[x++] = c;
			}
			else
			{
				cx = strlen( s ) + 1;
				s[cx] = 0;
				while( --cx > x )
					s[cx] = s[cx-1];

				s[x++] = c;
			}

			cx = x - boxx; /* cx = current cursor location in the box */
			while( --cx > 0 )   /* Move to beginning of box */
				outchr( '\b' );

			/* Recalculate window, if 'x' is outside box */
			if( x > boxy )
			{
				boxx = ( x - window );
				boxy = boxx + window;
			}

			/* Display as much string as possible, within the box */
			while( cx < window && s[cx+boxx] )
			{
				outchr( s[cx+boxx] );
				cx++;
			}

			while( cx-- > (x - boxx) )    /* Return cursor to where it started */
				outchr( '\b' );
		}
		else if( ansi && (c == KEY_ARROW_LEFT && x > 0 ))
		{
			cx = x - boxx + 1; /* cx = current cursor location in the box */
			x--;
			while( --cx > 0 )   /* Move to beginning of box */
				outchr( '\b' );

			/* Recalculate window, if 'x' is outside box */
			if( x < boxx )
			{
				boxx = x;
				boxy = boxx + window;
			}

			/* Display as much string as possible, within the box */
			while( cx < window && s[cx+boxx] )
			{
				outchr( s[cx+boxx] );
				cx++;
			}

			while( cx-- > (x - boxx) )    /* Return cursor to where it started */
				outchr( '\b' );
		}
		else if( ansi && ( c == KEY_ARROW_RIGHT && x < strlen( s )))
		{
			cx = x - boxx + 1;
			x++;
			while( --cx > 0 )
				outchr( '\b' );

			if( x > boxy )
			{
				boxx = ( x - window );
				boxy = boxx + window;
			}

			while( cx < window && s[cx+boxx] )
			{
				outchr( s[cx+boxx] );
				cx++;
			}

			while( cx -- > (x - boxx ))
				outchr( '\b' );

		}
		else if( ansi && c == KEY_HOME )
		{
			cx = x - boxx + 1;
			while( --cx > 0 )
				outchr( '\b' );

			x = 0;
			boxx = 0;
			boxy = boxx + window;

			while( s[cx] && cx < boxy )
				outchr( s[cx++] );

			while( --cx >= 0 )
				outchr( '\b' );
			
		}
		else if( ansi && c == KEY_END )
		{
			/* Move to beginning of window */
			cx = x - boxx + 1;
			while( --cx > 0 )
				outchr( '\b' );

			x = strlen( s );

			if( x > boxy )
			{
				boxx = ( x - window );
				boxy = boxx + window;
			}

			cx = boxx;
			while( s[cx] && cx < boxy )
				outchr( s[cx++] );
		}
		else if( c == '' ) // Clear entire input line
		{
			memset( s, '\0', strlen(s) );
			cx = x - boxx + 1; /* cx = current cursor location in the box */
			x = 0;
			while( --cx > 0 )   /* Move to beginning of box */
				outchr( '\b' );

			/* Recalculate window, if 'x' is outside box */
			if( x < boxx )
			{
				boxx = x;
				boxy = boxx + window;
			}

			/* Display as much string as possible, within the box */
			while( cx < window && s[cx+boxx] )
			{
				outchr( s[cx+boxx] );
				cx++;
			}

			outstr( "%s", strings[S_MPL_FILL_COLOR] );
			while( cx < boxy )
			{
				outstr( "%c", strings[S_MPL_FILL][ansi] );
				cx++;
			}

			while( cx-- > (x - boxx) )    /* Return cursor to where it started */
				outchr( '\b' );
			outstr( "%s", strings[S_MPL_TEXT_COLOR] );
			/*
			while( x < strlen( s ))
				outchr( s[x++] );
			while( x > 0 )
			{
				outstr( "\b%s%c%s\b", strings[S_MPL_FILL_COLOR], strings[S_MPL_FILL][ansi], strings[S_MPL_TEXT_COLOR] );
				//outstr( "\b%c\b", strings[S_MPL_FILL][ansi] );
				s[--x] = 0;
			}
			*/
		}
		else if( c )
			outchr( '\007' );
	} while( c != 13 && c != 10 && !_hangup );
}

void pausescr( void ) /* userio.c */
{
	char **throbber;
	int i, ithrob = 0, throbs = 0, _tok;
	clock_t c1, c2;
	int x;

	if( ansi )
	{
		throbber = toktok( strings[S_PROMPT_PAUSE_THROB], '', &_tok );
		while( throbber[throbs] != 0 )
			throbs++;
		c1 = c2 = clock();

		do
		{
			c2 = clock();
			usleep( throb_delay - ( c2 - c1 ));
			outstr( "%s", throbber[ithrob] );
			for( i = 0; i < stripstrlen( throbber[ithrob] ); i++ )
				outchr( '\b' );
			ithrob = (( ithrob + 1 ) % throbs );
			c1 = c2;
		} while( !kbhit() && !_hangup );
		getkey();

		while( i-- > 0 )
			outchr( ' ' );
		while( ++i < stripstrlen( throbber[ithrob-1] ))
			outchr( '\b' );
		resetcolor();
	}
	else
	{
		outstr( strings[S_PROMPT_PAUSE_BASIC] );
		while( !kbhit()) {}
		getkey();
		x = 0;
		while( x++ < stripstrlen( strings[S_PROMPT_PAUSE_BASIC] ))
			outchr( '\b' );
		while( x-- > 0 )
			outchr( ' ' );
		while( x++ < stripstrlen( strings[S_PROMPT_PAUSE_BASIC] ))
			outchr( '\b' );
		resetcolor();
	}

	return;
}

int yesno( char mode )
{
	int c,x;
	char sbuf[15];

	memset( sbuf, '\0', sizeof( sbuf ) + 1);

	switch( mode )
	{
		case 0:
			x = S_PROMPT_YESNO;
			sprintf( sbuf, "%s", "YN" );
			break;
		case 1:
			x = S_PROMPT_NOYES;
			sprintf( sbuf, "%s", "NY" );
			break;
		case 2:
			x = S_PROMPT_YESNOQUIT;
			sprintf( sbuf, "%s", "YNQ" );
			break;
		case 3:
			x = S_PROMPT_NOYESQUIT;
			sprintf( sbuf, "%s", "NYQ" );
			break;
	}

	logger( 9, "mode = %i, sizeof(sbuf) = %i, sbuf = '%s', str = '%s'", mode, sizeof( sbuf ), sbuf, strings[x] );
	outstr( strings[x] );
	c = onekey( sbuf, strings[S_THROBBER_DEFAULT] );

	resetcolor();

	switch( c )
	{
		case 'Y':
			outstr( strings[S_YES] );
			break;
		case 'N':
			outstr( strings[S_NO] );
			break;
		case 'Q':
			outstr( strings[S_QUIT] );
			break;
	}

	resetcolor();
	return( c );
}

int _yn( void )
{
	return( yesno( 0 ));
}

int ny( void )
{
	return( yesno( 1 ));
}

int ynq( void )
{
	return( yesno( 2 ));
}

int nyq( void )
{
	return( yesno( 3 ));
}

/* rawkey() is a test routine - don't expect it to be useful */
void rawkey( int argc, char **argv )
{
	int ch;

	do
	{
		outstr( "Hit a Key: " );
		while( !kbhit() ) {}
		ch = getkey();
		outstr( "\r\nYou hit '%i'\r\n", ch );
	}
	while( !_hangup && ch != 27 );

	return;
}

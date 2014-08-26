
/*
 * menu.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Functions handling menus and user interaction - choices, selections, etc.
 */

// $Id: menu.c 27 2011-08-08 17:02:49Z mholger $

#include "vars.h"

void menu( int dl )
{
	char s[81];

	memset( s, '\0', sizeof( s ));
	if( !expert )
		helpmenu( 0, NULL );

	outstr( thismenu.prompt );

	mmkey( dl, s );

	if( !menucommand( s ))
		outstr( strings[S_CMD_UNKNOWN], neutralizecolor( s ));
}

/* Waits for user to make a selection from a list of hotkey choices; the default
 * response is the first choice provided in *choices.
 */
int onekey( char *choices, char *throbsequence )
{
	static char **throbber;
	char _choice[2];
	int c, i, choice, ithrob = 0, throbs = 0;
	clock_t c1, c2;

	rflush();
	if( throbsequence && ansi )
	{
		throbber = toktok( throbsequence, '', &throbs );
		ithrob = throbs - 1;
		c1 = c2 = clock();
	}

	do
	{
		do
		{
			c = 1;
			if( throbsequence && ansi )
			{
				c2 = clock();
				usleep( throb_delay - ( c2 - c1 ) );
				outstr( "%s", throbber[ithrob] );
				for( i = 0; i < stripstrlen( throbber[ithrob] ); i++ )
					outchr( '\b' );
				ithrob = (( ithrob + 1 ) % throbs );
				c1 = c2;
			}
			if( kbhit() )
			{
				c = toupper( getkey() );
				if( c == '+' )
				{
					outdelay++;
					printf( "outdelay: %i\r\n", outdelay );
				}
				if( c == '-' )
				{
					outdelay--;
					printf( "outdelay: %i\r\n", outdelay );
				}
					
				sprintf( _choice, "%c", c );
			}
		} while( !strstr( choices, _choice ) && c != 13 && !_hangup );
		if( c == 13 )
			choice = choices[0];
		else
			choice = c;
	} while( !_hangup && !choice );

	return( choice );
}

void mmkey( int dl, char *cmd )
{
	char cmd1[51], cmd2[81], c;
	static char **throbber;

	int i, ithrob = 0, throbs = 0;
	clock_t c1, c2;

	memset( cmd1, '\0', sizeof( cmd1 ));
	memset( cmd2, '\0', sizeof( cmd2 ));

	rflush();

	if(( thismenu.flags & MENU_THROBBER ) && ansi )
	{
		throbber = toktok( thismenu.throbsequence, ' ', &throbs );
		ithrob = throbs-1;
		c1 = c2 = clock();
	}

	do
	{
		do
		{
			c = 0;
			if(( thismenu.flags & MENU_THROBBER ) && ansi )
			{
				c2 = clock();
				usleep( throb_delay - ( c2 - c1 ) );
				outstr( "%s", throbber[ithrob] );
				for( i = 0; i < stripstrlen( throbber[ithrob] ); i++ )
					outchr( '\b' );
				ithrob = (( ithrob + 1 ) % throbs );
				c1 = c2;
			}

			if( kbhit() )
				c = getkey();

		} while(( c <= ' ' || c > 126 ) && !_hangup );

		c = upcase( c );
		resetcolor();
		outchr( c );

		cmd1[0] = c;
		cmd1[1] = 0;

		if( cmd1[0] == 27 )
		{
			c = getkey();
			if( c == '[' )
			{
				c = getkey();
				switch( c )
				{
					case 'A':
						c = KEY_ARROW_UP;
						break;
					case 'B':
						c = KEY_ARROW_DOWN;
						break;
					case 'C':
						c = KEY_ARROW_RIGHT;
						break;
					case 'D':
						c = KEY_ARROW_LEFT;
						break;
				}
			}
		}

		if( dl && ( cmd1[0] == '/' ))
		{
			c1 = c2 = clock();
			do
			{
				c = 0;
				if(( thismenu.flags & MENU_THROBBER ) && ansi )
				{
					c2 = clock();
					usleep( throb_delay - ( c2 - c1 ) );
					outstr( "%s", throbber[ithrob] );
					for( i = 0; i < stripstrlen( throbber[ithrob] ); i++ )
						outchr( '\b' );
					ithrob = (( ithrob + 1 ) % throbs );
					c1 = c2;
				}
				if( kbhit() )
					c = getkey();
			} while(( c <= ' ' || c > 126 ) && ( c != 8 && c != 127 ) && !_hangup && !_timeout );

			c = upcase( c );
			if( c == 8 || c == 127 )
			{
				outstr( " \b\b" );
				cmd1[0] = 0;
			}
			else if( c == 13 || c == 10 )
			{
				cmd1[0] = 0;
				pnl();
				cmd = cmd1;
				return;
			}
			else if( c == '/' )
			{
				if( ansi )
					outchr( '\b' );
				else
					outchr( c );
				mpl( 20 );
				inputwc( cmd2, 30, 20 );
				sprintf( cmd, "%s", cmd2 );
				return;
			}
			else
			{
				cmd1[1] = c;
				cmd1[2] = 0;
				resetcolor();
				outchr( c );
				outstr( "|NL" );
				sprintf( cmd, "%s", cmd1 );
				return;
			}
		}
		else
		{
			pnl();
			sprintf( cmd, "%s", cmd1 );
			return;
		}
	} while( !_hangup );

	cmd1[0] = 0;
	cmd = cmd1;
	return;
}

unsigned int menucommand( char *s )
{
	char **cmd;
	unsigned int k = 0, exec = 0;
	int _tok = 0;

	while( thismenu.keys[k].key[0] )
	{
		if( !strcmp( thismenu.keys[k].key, s ))
		{
			exec++;
			cmd = toktok( thismenu.keys[k].command, ' ', &_tok );
			plugin = plugins_head;
			while( plugin )
			{
				if( !strcmp( plugin->cmdcode, cmd[0] ))
				{
					logger( 5, "menucommand(): Hitting '%s' callback at %p", plugin->cmdcode, plugin->callback );
					plugin->callback( _tok - 1, cmd + 1 );
					logger( 5, "menucommand(): Returning from '%s' callback at %p", plugin->cmdcode, plugin->callback );
					break;
				}
				plugin = plugin->next;
			}
		}
		k++;
	}

	return( exec );
}

void loadmenu( int argc, char **argv )
{
	char **cmd;
	int x = 0, _tok;

	while( thismenu.keys[x].key[0] != '\\' )
	{
		if( !strcmp( thismenu.keys[x].key, "**" ))
		{
			cmd = toktok( thismenu.keys[x].command, ' ', &_tok );
			plugin = plugins_head;
			while( plugin )
			{
				if( !strcmp( plugin->cmdcode, cmd[0] ))
				{
					logger( 5, "loadmenu(): Autoexec '%s' callback at %p", plugin->cmdcode, plugin->callback );
					plugin->callback( _tok - 1, cmd + 1 );
				}
				plugin = plugin->next;
			}
		}
		x++;
	}
}

void menu_add_command( char *menu, char *cmd, menukeyrec_t *mk )
{
	
}

void helpmenu( int argc, char **argv )
{
	int k = 0;

	// If there exists an exact match to the menu's fn - use it. Period.
	// Else, if an ANSI variation exists, and the users has ANSI support
	// enabled, we use that one.  Last (custom) resort - ASCII.
	// If none of the above succeeds, use the built-in menu strings...
	if( !outfile( thismenu.fn ))
		while( thismenu.keys[k].key[0] )
		{
			if( strcmp( thismenu.keys[k].key, "**" ) && thismenu.keys[k].key[0] != '\\' )
			{
				outstr( thismenu.keys[k].menutext );
				outstr( "|NL" );
			}
			k++;
		}
}


/*
 * newuser.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Functions and actions specifically related to user creation
 */

// $Id:$

#include "vars.h"

int newuser( void )
{
	char s[255];
	int npcount = 1, ok = 0, num = 1;
	userrec_t u;

	while( !ok )
	{
        clean(s);
		promptc( strings[S_PROMPT_NEWUSER_USERNAME], s, sizeof( u.username ), 20 );
		logger( 4, "newuser(): un: %s", s );
		if( okusername( s ))
			ok = 1;
		else
			outstr( strings[S_MSG_BADUSER] );
	}
	strcpy( u.username, s );

	ok = 0;
	while( !ok )
	{
		if( !findusernum( num ))
			ok = 1;
		else
			num++;
	}

	u.userid = num;
	ok = 0;
	memset( s, '\0', sizeof( s ));
	while( !ok && npcount < 3 )
	{
        clean(s);
		if( newpassword( s, sizeof( u.password )))
			ok = 1;
		else
		{
			pnl();
			outstrnl( strings[S_PROMPT_PASSMISMATCH] );
			npcount++;
		}
	}

	// Three tries to get a password, then die.
	if( !ok )
	{
		outstrnl( strings[S_MSG_SORRY_BYE] );
		bbsexit( 0 );
	}

	strcpy( u.password, s );

	// User #1 - SysOp
	if( u.userid == 1 )
	{
		logger( 1, "newuser(): SysOp Created - %s", u.username );
		u.flags |= USER_SYSOP;
	}

    u.flags ^= USER_DELETED;

	thisuser = u;

	writeuser( thisuser.userid, &thisuser );
	return( 1 );
}

int newpassword( char *pwd, int maxlen )
{
	char s[255], s2[255];

    clean(s);
    clean(s2);

	promptp( strings[S_PROMPT_NEWUSER_PASSWORD], s, maxlen, 20 );
	promptp( strings[S_PROMPT_NEWUSER_PASSVERIFY], s2, maxlen, 20 );

	if( strcmp( s, s2 ) )
		return( 0 );
	else
	{
		strcpy( pwd, s );
		return( 1 );
	}
}

int okusername( char *un )
{
	if( !finduser( un ))
	{
		return( 1 );
	}
	return( 0 );
}

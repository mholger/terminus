
/*
 * user.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * User-account related functions
 */

// $Id: user.c 27 2011-08-08 17:02:49Z mholger $

#include "vars.h"

int loaduser( int un, userrec_t *u )
{
	char tmp[81];
	int uf;
	userrec_t *tu;

	u->userid = 0;
	sprintf( tmp, "%s/users.dat", cfg.datapath );
	uf = open( tmp, O_CREAT | O_RDONLY | O_SYNC, S_IRUSR | S_IWUSR );
	if( uf == -1 )
	{
		//something bad happened
		logger( 1, "loaduser(%i) open(3) failed: %i", un, errno );
		bbsexit( 200 );
	}
	tu = malloc( sizeof( userrec_t ));
	memset( tu, '\0', sizeof( userrec_t ));

	if( un > 0 && lseek( uf, sizeof( userrec_t ) * (un - 1), SEEK_SET ) != -1 )
	{
		if( read( uf, (void *)tu, sizeof( userrec_t )))
		{
			logger( 4, "loaduser(%i) - found user", un );
			memcpy( u, tu, sizeof( userrec_t ));
		}
		else {
            close( uf );
            free( tu );
			return -1;
        }
	}
	else	// Bad User!
	{
		logger( 4, "loaduser(%i) - bad user", un );
        close( uf );
        free( tu );
		return -1;
	}

	close( uf );
	free( tu );
	logger( 3, "loaduser(%i)->[%i,%s]", un, u->userid, u->username );
	return u->userid;
}

int writeuser( int un, userrec_t *u )
{
	int uf;
	char tmp[81];

	sprintf( tmp, "%s/users.dat", cfg.datapath );
	uf = open( tmp, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
	if( uf == -1 )
	{
		logger( 1, "writeuser(%i) open(3) failed: %i", un, errno );
		bbsexit( 203 );
	}

	if( lseek( uf, sizeof( userrec_t ) * ( un - 1 ), SEEK_SET ) == -1 )
	{
		logger( 1, "writeuser(%i) lseek(3) failed: %i", un, errno );
		bbsexit( 201 );
	}

	if( write( uf, (void *)u, sizeof( userrec_t )) == -1 )
	{
		logger( 1, "writeuser(%i) write(3) failed: %i", un, errno );
		bbsexit( 202 );
	}

	logger( 1, "writeuser(%i) succeeded", un );
	close( uf );
	return 1;
}

int finduser( char *uname )
{
	int un = 1, ok = 1;
	userrec_t u;

	logger( 5, "finduser(%s)", uname );
	do
	{
        logger( 6, "finduser(%s) Loading #%i...", uname, un );
		ok = loaduser( un, &u );
		logger( 6, "finduser(%s) Loaded [%s]", uname, u.username );
		if( !strcmp( uname, u.username ))
		{
			logger( 5, "finduser(%s)->[%i]", uname, u.userid );
			return( u.userid );
		}
		un++;
	}
	while( ok >= 0 );

	logger( 5, "finduser(%s) failed", uname );
	return( 0 );
}

int findusernum( int un )
{
	userrec_t lu;

	if( loaduser( un, &lu ) >= 0 && !(lu.flags & USER_DELETED ))
	{
		logger( 3, "findusernum(%i) = found", un );
		return( lu.userid );
	}
	else
	{
		logger( 3, "findusernum(%i) = notfound", un );
		return( 0 );
	}
}

void experttoggle( int argc, char **argv )
{
	expert = !expert;
}

int checkpass( int userid, char *s1 )
{
	int usernum = -1;
	userrec_t u;

	usernum = loaduser(userid, &u);
	logger(5, "checkpass(): userid=%i, u.userid=%i, s1=%s, u.password=%s", userid, u.userid, s1, u.password);
	if(usernum <= 0 || strcmp(s1, u.password)) {
		return( 0 );
	}
	else {
		return( 1 );
	}
}

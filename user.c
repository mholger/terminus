
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
		else
			return -1;
	}
	else	// Bad User!
	{
		logger( 4, "loaduser(%i) - bad user", un );
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
		bbsexit( 200 );
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
		ok = loaduser( un, &u );
		logger( 5, "finduser(%s) [%s]", uname, u.username );
		if( !strcmp( uname, u.username ))
		{
			logger( 5, "finduser(%s)->[%i]", uname, u.userid );
			return( u.userid );
		}
		un++;
	}
	while( ok );

	logger( 5, "finduser(%s) failed", uname );
	return( 0 );
}

int findusernum( int un )
{
	userrec_t u;

	if( loaduser( un, &u ) && !(u.flags && USER_DELETED ))
	{
		logger( 3, "findusernum(%i) = found", un );
		return( 1 );
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

int checkpass( char *s1, char *s2 )
{
	if( !strcmp( s1, s2 ))
		return( 1 );
	else
		return( 0 );
}


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
	sprintf( tmp, "%susers.dat", cfg.datapath );
	uf = open( tmp, O_CREAT | O_RDONLY | O_SYNC, S_IRUSR | S_IWUSR );
	if( uf != 0 )
	{
		//something bad happened
		return -1;
	}
	tu = malloc( sizeof( userrec_t ));
	memset( tu, '\0', sizeof( userrec_t ));

	if( un > 0 )
	{
		lseek( uf, sizeof( userrec_t ) * ( un - 1 ), SEEK_SET );
		read( uf, (void *)tu, sizeof( userrec_t ));
		memcpy( u, tu, sizeof( userrec_t ));
	}
	else
		while( read( uf, (void *)tu, sizeof( userrec_t )) != -1 )
			if( strcmp( tu->username, u->username ) == 0 )
				memcpy( u, tu, sizeof( userrec_t ));

	close( uf );
	free( tu );
	return u->userid;
}

int writeuser( int un, userrec_t *u )
{
	int uf;
	char tmp[81];

	sprintf( tmp, "%susers.dat", cfg.datapath );
	uf = open( tmp, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR );
	if( uf != 0 )
		return -1;
	lseek( uf, sizeof( userrec_t ) * ( un - 1 ), SEEK_SET );
	write( uf, (void *)u, sizeof( userrec_t ));
	close( uf );
	return 1;
}


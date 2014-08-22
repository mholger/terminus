
/*
 * logging.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Logging-related functionality.
 *
 */

// $Id: logging.c 27 2011-08-08 17:02:49Z mholger $

#include "vars.h"

int logfile;	/* Filehandle for log file */
int openlog( void )
{
	char buf[255], buf2[30];
	time_t tm;

	memset( buf, '\0', sizeof( buf ));
	time( &tm );
	sprintf( buf, "%s/messages", cfg.logpath );
	logfile = open( buf, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
	if( !logfile )
	{
		outstr( "** CRITICAL ERROR **\r\n" );
		outstr( "Couldn't open or create logfile '%s'\r\n", buf );
		outstr( "errno = %i\r\n\r\n", errno );
		return 0;
	}
	sprintf( buf2, "%s", asctime( gmtime( &tm )));
	buf2[strlen( buf2 ) - 1] = '\0';
	sprintf( buf, "\n\n%s BEGIN SESSION\n", buf2 );
	write( logfile, (void *)buf, strlen( buf ));
	return 1;
}

void logger( int lv, char *str, ... )
{
	va_list ap;
	char buf[4096], buf2[4096], buf3[27];
	time_t tm;

	if( lv <= debuglevel )
	{
		time( &tm );

		memset( buf, '\0', sizeof( buf ));
		va_start( ap, str );
		vsprintf( buf2, str, ap );
		va_end( ap );

		sprintf( buf3, "%s", asctime( gmtime( &tm )));
		buf3[strlen( buf3 ) - 1] = '\0';
		sprintf( buf, "%s [%2i] %s\n", buf3, lv, buf2 );
		write( logfile, (void *)buf, strlen( buf ));
	}
}

void closelog( void )
{
	close( logfile );
}

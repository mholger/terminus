
#include "../../vars.h"

void gfiles( int argc, char **argv );
void gfiles_exec( int argc, char **argv );

void plugin_register( void )
{
	plugin_add( "G!", &gfiles, 0 );
	plugin_add( "G*", &gfiles_exec, PLUGIN_EXEC );
	return;
}

void gfiles( int argc, char **argv )
{
	int x;

	outstr( "G-files, bitch!\r\n" );
	if( argc > 0 )
	{
		for( x = 0; x < argc; x++ )
			outstr( "args: argv[%i] = '%s'\r\n", x, argv[x] );
	}

	return;
}

void gfiles_exec( int argc, char **argv )
{
	outstr( "Autoexec'd!\r\n" );
	return;
}

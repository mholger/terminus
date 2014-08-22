
/*
 * plugins.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Routines for loading, unloading and managing plugins/dynamic libraries.
 */

// $Id: plugins.c 26 2010-09-02 02:40:23Z mholger $

#include "vars.h"

int isplugin( char *pname )
{
	plugin = plugins_head;

	while( plugin )
	{
		if( !strcmp( plugin->cmdcode, pname ))
			return 1;
		plugin = plugin->next;
	}
	return 0;
}

void plugin_load( char *pso )
{
	void *pluginso;
	char *dlerr;
	void (*plugin_register)();

	logger( 1, "plugin_load(): Loading '%s'", pso );
	pluginso = dlopen( pso, RTLD_LAZY );
	dlerr = dlerror();
	if( dlerr )
	{
		logger( 1, "plugin_load(): dlerror: %s\r\n", dlerr );
	}

	plugin_register = dlsym( pluginso, "plugin_register" );
	if( dlerr )
	{
		logger( 1, "plugin_load(): dlerror: %s\r\n", dlerr );
	}
	plugin_register();
}

void plugin_add( char *cmdcode, void (*callback)( int, char** ), int flags )
{
	pluginrec_t *p;

	if( !isplugin( cmdcode ))
	{
		logger( 1, "plugin_add(): Binding cmdcode '%s'", cmdcode );
		if(( p = malloc( sizeof( pluginrec_t ))) == NULL )
		{
			logger( 1, "plugin_add(): malloc(%i) failed!", sizeof( pluginrec_t ));
			return;
		}
		/* null-out the new record */
		memset( p, '\0', sizeof( pluginrec_t ));

		if(( strncpy( p->cmdcode, cmdcode, sizeof( p->cmdcode ))) == NULL )
			logger( 1, "plugin_add(): strncpy(cmdcode) failed" );

		p->flags = flags;
		p->callback = callback;
		p->next = NULL;
		p->prev = plugins_tail;

		plugins_tail->next = p;
		plugins_tail = p;
	}
	else
		logger( 1, "plugin_add(): Binding of '%s' failed - already registered", cmdcode );
}

void plugin_del( pluginrec_t *p )
{
}

void plugin_free( void )
{
	while( plugins_head )
	{
		plugins_tail = plugins_head->next;
		free( plugins_head );
		plugins_head = plugins_tail;
	}
}

void plugin_load_menu( int argc, char **argv )
{
	if( argc == 1 )
		plugin_load( argv[0] );

	return;
}

void mci_register( char *mcd, char* (*callback)( char * ) )
{
	mcicoderec_t *m;

	if( !ismcicode( '|', mcd[0], mcd[1] ))
	{
		logger( 4, "mci_register(): Registering '%s'", mcd );
		if(( m = malloc( sizeof( mcicoderec_t ))) == NULL )
		{
				logger( 1, "mci_register(): malloc(%i) failed!", sizeof( mcicoderec_t ));
				return;
		}
		memset( m, '\0', sizeof( mcicoderec_t ));
		strncpy( m->code, mcd, 2 );
		m->mcidecoder = callback;
		m->next = NULL;

		mcicode_tail->next = m;
		mcicode_tail = m;
	}
	else
		logger( 4, "mci_register(): '%s' already registered", mcd );
}

void mci_free( void )
{
	while( mcicode_head )
	{
		mcicode_tail = mcicode_head->next;
		free( mcicode_head );
		mcicode_head = mcicode_tail;
	}
}

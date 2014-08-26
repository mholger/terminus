
/*
 * init.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Subroutines to handle BBS structure, configuration, and user initialization
 * before handing control over to the user.
 */

// $Id: init.c 28 2011-08-09 13:07:30Z mholger $

#include "vars.h"

void configinit( void )
{
	configrec_t t;
	
	strcpy( t.bbsname, "Terminus" );
	strcpy( t.sysop, "Radix" );
	strcpy( t.email, "radix@terminusbbs.com" );
	strcpy( t.datapath, "./data" );
	strcpy( t.pluginpath, "./plugins" );
	strcpy( t.logpath, "./logs" );
	strcpy( t.textpath, "./text" );
	strcpy( t.tmppath, "./temp" );
	
	t.flags &= (SYS_CLOSED);
	t.nodes = 8;
	
	cfg = t;
}

void menuinit( void )
{
	menurec_t m;
	menukeyrec_t mk;

	m.flags |= (MENU_THROBBER|MENU_HOTKEY);

	strcpy( m.fn, "menus/mainmenu" );
	strcpy( m.identifier, "MAIN" );
	strcpy( m.throbsequence, "|08. |07. |15. |07." );
	strcpy( m.prompt, "|NL|14m|06ain |14m|06enu|08: " );

	strcpy( mk.key, "**" );
	strcpy( mk.title, "Autotweet" );
	strcpy( mk.command, "!S Terminus auto-tweet" );
	strcpy( mk.menutext, "|12[|14**|12] |15A|14u|06to-|15T|14w|06eet" );
	mk.order = 0;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "O" );
	strcpy( mk.title, "Logoff" );
	strcpy( mk.command, "SO 0" );
	strcpy( mk.menutext, "|12[|14 O|12] |15L|14o|06goff" );
	mk.order = 4;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "E" );
	strcpy( mk.title, "Read Email" );
	strcpy( mk.command, "ER" );
	strcpy( mk.menutext, "|12[|14 E|12] |15E|14m|06ail" );
	mk.order = 2;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "/O" );
	strcpy( mk.title, "Logoff Now" );
	strcpy( mk.command, "SO 1" );
	strcpy( mk.menutext, "|12[|14/O|12] |15L|14o|06goff |15N|14o|06w" );
	mk.order = 5;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "X" );
	strcpy( mk.title, "eXpert Mode" );
	strcpy( mk.command, "UX" );
	strcpy( mk.menutext, "|12[|14 X|12] |14e|15X|14p|06ert |15M|14o|06de" );
	mk.order = 12;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "VERSION" );
	strcpy( mk.title, "Version" );
	strcpy( mk.command, "SV" );
	strcpy( mk.menutext, "|12[|14VERSION|12] |15V|14e|06rsion |15I|14n|06fo" );
	mk.order = 6;
	m.keys[mk.order] = mk;

	sprintf( mk.key, "G" );
	strcpy( mk.title, "G-Files" );
	sprintf( mk.command, "G! FOO" );
	strcpy( mk.menutext, "|12[|14 G|12] |15T|14e|06xtfiles" );
	mk.order = 3;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "**" );
	strcpy( mk.title, "G-Files AutoExec" );
	strcpy( mk.command, "G*" );
	strcpy( mk.menutext, "|12[|14**|12] |15T|14e|06st" );
	mk.order = 1;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "?" );
	strcpy( mk.title, "Menu" );
	strcpy( mk.command, "H0" );
	strcpy( mk.menutext, "|12[|14 ?|12] |15M|14e|06nu" );
	mk.order = 10;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "COLORS" );
	strcpy( mk.title, "Color Demo" );
	strcpy( mk.command, "A0" );
	strcpy( mk.menutext, "|12[|14 COLORS|12] |15C|14o|06lor |15D|14e|06mo" );
	mk.order = 8;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "RAWKEY" );
	strcpy( mk.title, "Raw Key Test" );
	strcpy( mk.command, "A1" );
	strcpy( mk.menutext, "|12[|14 RAWKEY|12] |15R|14a|06w |15K|14e|06y |15T|14e|06st" );
	mk.order = 9;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "TWEET" );
	strcpy( mk.title, "Tweet!" );
	strcpy( mk.command, "!T" );
	strcpy( mk.menutext, "|12[|14  TWEET|12] |15T|14w|06eet" );
	mk.order = 7;
	m.keys[mk.order] = mk;

	strcpy( mk.key, "\\" );
	mk.order = 11;
	m.keys[mk.order] = mk;

	thismenu = m;
}

void userinit( userrec_t *u )
{
	u->sl = 0;
	u->dsl = 0;
	memset( u->ar, '\0', 27 );
	memset( u->dar, '\0', 27 );
	u->screenlen = 25;
}

void mciinit( void )
{
	int x;
	char buf[2];
	mcicoderec_t *m;

	if(( m = (void *)malloc( sizeof( mcicoderec_t ))) == NULL )
	{
		logger( 1, "mciinit(): malloc(%i) failed!", sizeof( mcicoderec_t ));
		return;
	}

	memset( m, '\0', sizeof( mcicoderec_t ));

	mci = m;
	mcicode_head = m;
	mcicode_tail = m;

	strcpy( m->code, "00" );
	m->mcidecoder = &mcidecode;
	m->next = NULL;

	/* Register internal MCI codes; colors, mostly */
	for( x = 1; x <= 23; x++ )
	{
		sprintf( buf, "%02i", x );
		mci_register( buf, &mcidecode );
		if( !ismcicode( '|', buf[0], buf[1] ))
			logger( 1, "mciinit(): Registration of '%02i' failed!", x );
	}

	mci_register( "SV", &mcidecode );
	mci_register( "NL", &mcidecode );
	mci_register( "!R", &mcidecode );
	mci_register( "!P", &mcidecode );
}

void plugininit( void )
{
	DIR *dirp;
	struct dirent *de;
	pluginrec_t *p;
	char tmp[81];

	/* First record! */
	if(( p = (void *)malloc( sizeof( pluginrec_t ))) == NULL )
	{
		logger( 1, "plugininit(): malloc(%i) failed!", sizeof( pluginrec_t ));
		abort();
	}

	memset( p, '\0', sizeof( pluginrec_t ));

	/* Initialize head and tail list pointers */
	plugin = p;
	plugins_head = p;
	plugins_tail = p;

	/* Build the first record, or else bad things happen */
	strcpy( p->cmdcode, "SV" );
	p->callback = &vers;
	p->next = NULL;
	p->prev = NULL;

	/* Once the first record exists, we can just plugin_add() 'til done! */
	/* Add intneral functions first; we provide just a handful, but they're key */
	plugin_add( "SO", &logoff, 0 );
	plugin_add( "LM", &loadmenu, 0 );
	plugin_add( "LP", &plugin_load_menu, 0 );
	plugin_add( "H0", &helpmenu, 0 );
	plugin_add( "A0", &ansidemo, 0 );
	plugin_add( "A1", &rawkey, 0 );
	plugin_add( "UX", &experttoggle, 0 );

	/* And here we register the plugins, that plugin_add() their own routines */
	logger( 9, "plugininit(): Scanning '%s' for plugins...", cfg.pluginpath );
	if(( dirp = opendir( cfg.pluginpath )) == NULL )
	{
		logger( 1, "plugininit(): CRITICAL: Couldn't read '%s': %i", cfg.pluginpath, errno );
		return;
	}
	if( dirp )
	{
		while(( de = readdir( dirp )) != NULL )
		{
			if( de == NULL )
				logger( 1, "plugininit(): de == NULL in %s at %i", __FILE__, __LINE__ );

			logger( 9, "plugininit(): Working on '%s'", de->d_name );
			if( de->d_type == 0x8 )
			{
				char *strtmp;
				strtmp = strstr( de->d_name, ".so" );
				if( strtmp != NULL && strlen( strtmp ) == 3 )
				{
					sprintf( tmp, "%s/%s", cfg.pluginpath, de->d_name );
					logger( 7, "plugininit(): Loading %s", tmp );
					plugin_load( tmp );
				}
			}
			logger( 9, "plugininit(): Done with '%s'", de->d_name );
		}
		logger( 9, "plugininit(): Done loading plugins" );
		closedir( dirp );
	}
}


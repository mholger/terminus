
#ifndef __FCNS_H
#define __FCNS_H

#include "vardec.h"

/* ansi.c - ANSI terminal handling procedures */
char*	makeansi( char *s );
void	goxy( int x, int y );
void	resetcolor( void );
void	clrscreen( void );
int		detectansi( void );
void	confirmansi( void );
void	ansidemo( int argc, char **argv );


/* bbs.c - main() */
int		main( int argc, char **argv );
void	bbsexit( int e );
void	logoff( int argc, char **argv );


/* com.c - Comms-related routines */
void	out1ch( char c );
void	comreset( void );
int		_kbhit( void );
int		set_blocking_mode( int fd, int mode );

/* config.c - Configuration handler */

/* email.c - Internet and BBS email utility functions */
/*	Largely built on msgbase.c */

/* init.c - BBS initialization routines */
void	configinit( void );
void	menuinit( void );
void	cominit( void );
void	userinit( userrec_t *u );
void	plugininit( void );
void	mciinit( void );

/* logging.c - Logging-related functionality */
int		openlog( void );
void	logger( int lv, char *str, ... );
void	closelog( void );

/* menu.c - Menu definition handling procedures */
void	menu( int dl );
void	mmkey( int dl, char *cmd );
void	loadmenu( int argc, char **argv );
unsigned int menucommand( char *s );
void	helpmenu( int argc, char **argv );

/* msgbase.c - Message handling functions */
int		ismcicode( char c1, char c2, char c3 );
char* 	mcidecode( char *s );

/* newuser.c - Newuser functions */

/* plugins.c - Plugin management functions */
int		isplugin( char *pname );
void	plugin_load( char *pso );
void	plugin_add( char *cmdcode, void (*callback)( int, char** ), int flags );
void	plugin_del( pluginrec_t *p );
void	plugin_free( void );
void	plugin_load_menu( int argc, char **argv );
void	mci_register( char *mcd, char* (*callback)( char * ) );
void	mci_free( void );

/* rbuf.c - Ring buffer implementation */
void	initring( unsigned int sz );
void	rflush( void );
void	stash( int c );
int		unstash( void );
int		peek1ch( void );
char	kbhit( void );
void	freering( void );

/* user.c - User management functions */
int		loaduser( int un, userrec_t *u );
int		writeuser( int un, userrec_t *u );
void		experttoggle( int argc, char **argv );

/* userio.c - Functions for interacting with Users */
/*	Higher-level functions built on com.c */
int		outfile( char *fn );
void	outchr( char c );
void	outstr( char *fmt, ... );
void	out1str( char *s );
int		onekey( char *choices, char *throbsequence );
void	input( char *s, int maxlen );
void	inputl( char *s, int maxlen );
void	inputc( char *s, int maxlen );
void	inputw( char *s, int maxlen, int window );
void	inputwl( char *s, int maxlen, int window );
void	inputwc( char *s, int maxlen, int window );
void	input1( char *s, int maxlen, int mode, int window );
void	mpl( int n );
void	pnl( void );
void	nln( int n );
int		get1ch( void );
int		getkey( void );
int		yesno( char mode );
int		ny();
int		ynq();
int		nyq();
void	pausescr( void );
void	rawkey( int argc, char **argv );

/* utility.c - Utility functions */
/*	Stand-alone utility type functions */
char**	toktok( char *s, char sep, int *cnt );
void	paint( int pattern, char *s );
int		exists( char *fn );
char*	stripcolor( char *s );
char*	neutralizecolor( char *s );
int		stripstrlen( char *s );
char	upcase( char c );

/* version.c - Version data */
void	vers( int argc, char **argv );

#endif // __FCNS_H

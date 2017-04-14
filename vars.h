#ifndef __VARS_H
#define __VARS_H

#define MAXTOK 255
#define MAXSTRINGS 255
#define	MAXMEM 1024

#include "fcns.h"
#include "strings.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <termios.h>
#include <time.h>
#include <term.h>
#include <unistd.h>
#include <fcntl.h>
#include <dlfcn.h>

#include <arpa/telnet.h>

#include <sys/time.h>

extern char *bbs_version;
extern char *bbs_long_name;
extern unsigned int bbs_build_num;
extern char *bbs_date;
extern char strings[MAXSTRINGS][512];
extern char menu_throbber[20][25];

#ifdef _DEFINE_GLOBALS_

unsigned int debuglevel = 9; // Maximum logging level to write to logfile
unsigned int dopause = 1; // Whether or not we need to pause every screenful
unsigned long throb_delay = (10*7500); // clocks between throbber updates
unsigned int kdelay = 0; // uS sleep appended by kbhit()
unsigned int outdelay = 0; // uS delay introduced in console fd
unsigned char insert = 1; // Character-editing mode, Ins or Ovr

#define __EXTERN__

#else // _DEFINE_GLOBALS_

extern unsigned int debuglevel;
extern unsigned int dopause;
extern unsigned long throb_delay;
extern unsigned int kdelay;
extern unsigned int outdelay;
extern unsigned char insert;

#define __EXTERN__ extern

#endif // _DEFINE_GLOBALS

__EXTERN__ ringbuf_t ring;
__EXTERN__ struct termios oldparms;
__EXTERN__ userrec_t thisuser;
__EXTERN__ configrec_t cfg;
__EXTERN__ subrec_t sub;
__EXTERN__ menurec_t thismenu;
__EXTERN__ noderec_t node;
__EXTERN__ pluginrec_t *plugin;
__EXTERN__ pluginrec_t *plugins_head;
__EXTERN__ pluginrec_t *plugins_tail;
__EXTERN__ mcicoderec_t *mci;
__EXTERN__ mcicoderec_t *mcicode_head;
__EXTERN__ mcicoderec_t *mcicode_tail;
__EXTERN__ void* bbsptr[MAXMEM];

__EXTERN__ int ansi, fgcolor, bgcolor, hicolor, expert, okcolor, lines_listed, _echo, linect, mciok;
__EXTERN__ int _hangup, _timeout;

#endif // __VARS_H

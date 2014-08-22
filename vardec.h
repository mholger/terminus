#ifndef __VARDEC_H
#define __VARDEC_H

/* Input ring buffer */
typedef struct ringbuf
{
	int				*buf;	// Actual buffer space
	unsigned int	iptr,	// Pointer to current write location in *buf
					optr,	// Pointer to current read location in *buf
					sz;		// Size of the buffer in (int)'s.
} ringbuf_t;

/* System Configuration */
typedef struct configrec
{
	char	bbsname[100],	// The name of your board, silly!
			sysop[25],		// SysOp name.  Just use username from userid 1?
			email[100],		// SysOp's internet email addy
			nup[50],		// New User Password
			menu[20],		// Initial menu to load
			authmenu[20],	// Menu to use for login purposes
			datapath[100],	// Path to BBS Data files
			textpath[100],	// Path to BBS text/ascii/ansi files
			pluginpath[100],// Path to plugin libraries
			logpath[100],	// Path we keep logfiles in
			tmppath[100];	// Path to storage for temp data...
	unsigned int	calls,	// Total number of calls the system has received
					flags,	// System status flags
					nodes;	// Number of nodes available for remote logins
} configrec_t;

/* User records */
typedef struct userrec
{
	int		userid;			// onno.  Seemed like a good idea?
	char	username[25],	// Alias.  You know.
			realname[50],	// Realnames.  Optional?
			gender,			// M/F/undisclosed.
			email[100],		// Mandatory!
			url[100],		// Optional
			password[20],	// Can't forget this!! (But I did anyway)
			ar[27],			// Various user-level flags
			dar[27],		// Ditto...
			comments[81],	// User-defined comments
			notes[81];		// Sysop-defined comments
	unsigned int	screenlen,	// Lines per screen...mostly for pause purposes
					flags,		// Reserved
					sl,			// Security level
					dsl,		// Download sec. level ...
					bd_day,		// Birth day
					bd_mo,		// Birth month
					bd_yr,		// Birth year
					calls,		// Number of times the user has called us
					lc_day,		// Day of last call
					lc_mo,		// last call month
					lc_yr,		// Year of last call.
					lc_hr,		// Hour of last call
					lc_min;		// And the minute mark.
} userrec_t;

/* Subs - message bases in WWIV parlance */
typedef struct subrec
{
	char	name[50],		// Board name
			datafile[50],	// Filename to contain messages
// Note - ACS fields here are inconsistent with elsewhere in the code...
			readacs[255],	// Minimum requirements to read
			postacs[255];	// Minimum requirements to post
} subrec_t;

/* Each menu has some number of "keys" - this is how we define which keys
 * get bound to which functions or plugins, what the security requirements
 * for those functions are, and how to represent it in the default system
 * menu.
 */
typedef struct menukeyrec
{
	char	key[20],		// Hotkey, or //string
			title[50],		// Notes...
			command[255],	// Command, space-seperated
			menutext[80];	// Text for default system menu
	int		acs;			// Minimum user ACS to access command
	int		flags;			// Reserved
	int		x,				// For light bars: x-coord
			y,				// Y-coord
			order;			// Sequence number
} menukeyrec_t;

/* A menu is basically a collection of keys, a menu screen, and a prompt. */
typedef struct menurec
{
	unsigned int	menu_id;
	char			identifier[50];		// Identifier for loading via LN, etc.
	char			fn[255];			// Menu filename
	unsigned int	flags;
	char			throbsequence[255];	// Space-seperated sequence
	char			prompt[255];		// Menu prompt string...
	menukeyrec_t	keys[255];			// Menu keys...
} menurec_t;

/* Due to resource utilization, I've found it necessary to limit
 * the number of nodes available - which seems ludicrous considering
 * a 386 running desqview could run a four-node BBS without breaking
 * a sweat.
 *
 * These noderecs are for tracking individual node activity, as well
 * as providing some additional controls over resources for sysops.
 * Mostly, defining a node that only admin-types can use, or that's
 * for local use only - so sysops can get in and clean things up if
 * something goes awry, and the other nodes are all locked up.
 */
typedef struct noderec noderec_t;
struct noderec
{
	unsigned int	node_id;	// Current node ID
	int				user_id;	// Currently active user
	unsigned char	*activity;	// What's currently happening... ptr to strings[] entry
	unsigned int	flags;
	noderec_t		*next;
	noderec_t		*prev;
};

/* Each "sl" has some defined limits... */
typedef struct slrec slrec_t;
struct slrec
{
	int			sl,				// Security level
				timeday,		// Time allowed on per day
				timecall,		// Time allowed on per call
				postsday,		// Maximum posts per day
				postscall;		// Maximum posts per call
	slrec_t		*next;			// And, of course, doubly-linked list ptrs
	slrec_t		*prev;
};

/* External plugins provide user-enhanced technologies! */
/* menurec.cmd links in here; when a user selects a function
 * from the menu, we walk this list to find matching cmdcodes,
 * then execute the defined callbacks.
 */
typedef struct pluginrec pluginrec_t;
struct pluginrec
{
	char		cmdcode[3];		// Two-letter command code, for menu use
	void		(*callback)( int, char** );	// Function to call from dynamic library
	int			flags;			// Reserved
	pluginrec_t	*next;			// Because this is a linked list.
	pluginrec_t	*prev;			// Doubly so.
};

typedef struct mcicoderec mcicoderec_t;
struct mcicoderec
{
	char		code[3];		// The MCI code in question
	char*		(*mcidecoder)( char * );	// Function to handle the MCI code
	mcicoderec_t	*next;
};

typedef struct fieldrec fieldrec_t;
struct fieldrec
{
	char		field[20],		// System-referenced record name
				name[20],		// Human-readable record name
				text[20],		// Textual representation, if !ansi or !inputscr
				data[80];		// Default entry, or valid choices if FIELD_ONEKEY
	int			length,			// Max length of input
				x,				// X-coord for input start
				y,				// Y-coord for input start
				flags;			// Reserved
	fieldrec_t	*next;			// Linked list pointer
	fieldrec_t	*prev;			// Linked list pointer
};

typedef struct formrec formrec_t;
struct formrec
{
	char		name[20],		// Internal form identifier
				inputscr[255],	// Filename for form data entry
				outputscr[255];	// Filename for form data display
	int			flags;			// Reserved
	fieldrec_t	fields[255];	// Individual input field definitions
};

#define SYS_CLOSED		0x01

#define USER_DELETED	0x01
#define USER_BANNED		0x02
#define USER_SYSOP		0x04

#define MENU_THROBBER	0x01
#define MENU_HOTKEY		0x02
#define	MENU_LIGHTBAR	0x04
#define MENU_LB_HORIZ	0x08
#define MENU_LB_VERT	0x10

#define KEY_ARROW_UP	1
#define KEY_ARROW_DOWN	2
#define KEY_ARROW_LEFT	3
#define KEY_ARROW_RIGHT	4
#define KEY_HOME		5
#define KEY_END			6
#define KEY_INS			7
#define KEY_DEL			9

#define NODE_ACTIVE		0x01
#define NODE_ENABLED	0x02
#define NODE_ADMINONLY	0x04
#define NODE_LOCALONLY	0x08

#define FORM_NOGRAPHIC	0x01	// Forcibly ignore graphic forms
#define FORM_WIZARD		0x02	// Graphic form is multi-page "wizard style"

#define FIELD_REQUIRED	0x01	// Field is required for validation
#define FIELD_ONEKEY	0x02	// Field is hotkeyed
#define FIELD_HIDDEN	0x04	// Field input should be hidden (ie: passwords)

#define PLUGIN_EXEC		0x01

#endif // __VARDEC_H

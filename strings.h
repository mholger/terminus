#ifndef __STRINGS_H
#define __STRINGS_H

#ifdef _DEFINE_GLOBALS_

char strings[MAXSTRINGS][512] = {
	"XX\0",									// S_HIDDEN_FILL
	//" ±\0",									// S_MPL_FILL
	" .\0",									// S_MPL_FILL
	//"|08[|20|12\0",							// S_MPL_PREFIX
	//"|16|08]|20|15\0",						// S_MPL_SUFFIX
	"|08[|15\0",							// S_MPL_PREFIX
	"|08]|15\0",						// S_MPL_SUFFIX
	"|12\0",									// S_MPL_FILL_COLOR
	"|15\0",									// S_MPL_TEXT_COLOR
	"Detecting ANSi...\0",					// S_DETECT_ANSI
	"|14A|06NSi |14D|06etected|NL\0",			// S_ANSI_DETECTED
	"No ANSi Support Detected|NL\0",			// S_NO_ANSI
	"|08P|01r|09oc|15eed with ANSi Graphi|07cs|08? \0",// S_KEEP_ANSI
	"|14l|06ogin|08: \0",						// S_LOGIN
	"|14p|06assword|08: \0",					// S_PASSWORD
	"|14r|06eading |14e|06mail|08...|NL\0",	// S_EMAIL_READ
	"|14u|06nknown |14c|06ommand '|14%s|06'|NL\0",	// S_CMD_UNKNOWN
	"|15Y|07e|08s|NL\0",						// S_YES
	"|09N|01o|NL\0",							// S_NO
	"|08[|15CR|07/|15Y|08=|15Yes|08, |15N|08=|15No|08] \0",// S_PROMPT_YESNO
	"|08[|15CR|07/|15Y|08=|15Yes|08, |15N|08=|15No|08, |15Q|08=|15Quit|08] \0",// S_PROMPT_YESNOQUIT
	"|08[|15CR|07/|15N|08=|15No|08, |15Y|08=|15Yes|08] \0",// S_PROMPT_NOYES
	"|08[|15CR|07/|15N|08=|15No|08, |15Y|08=|15Yes|08, |15Q|08=|15Quit|08] \0",// S_PROMPT_NOYESQUIT
	"|07-|07=|07+|07=\0",				// S_THROBBER_DEFAULT
	"|05[PAUSE]\0",					// S_PROMPT_PAUSE_BASIC
	//"|08-|08-|07-|08-|07-|15-|08[-|07-|15-|08[ -|07-|15-|08[ |08-|07-|15-|14pa|06us|08e>",			// S_PROMPT_PAUSE_THROB
	"|08 [ pause ]|08 [ |15p|08ause ]|08 [ |15pa|08use ]|08 [ |07p|15au|08se ]|08 [ |07pa|15us|08e ]|08 [ |07pau|15se|08 ]|08 [ p|07aus|15e|08 ]|08 [ pa|07use|08 ]|08 [ pau|07se|08 ]|08 [ paus|07e|08 ]|08 [ pause ]|08 [ pause ]|08 [ pause ]|08 [ pause ]|08 [ pause ]|08 [ pause ]\0",
	"\0",
};

#endif // _DEFINE_GLOBALS_

enum _string_index {
	S_HIDDEN_FILL,
	S_MPL_FILL,
	S_MPL_PREFIX,
	S_MPL_SUFFIX,
	S_MPL_FILL_COLOR,
	S_MPL_TEXT_COLOR,
	S_DETECT_ANSI,
	S_ANSI_DETECTED,
	S_NO_ANSI,
	S_KEEP_ANSI,
	S_LOGIN,
	S_PASSWORD,
	S_EMAIL_READ,
	S_CMD_UNKNOWN,
	S_YES,
	S_NO,
	S_PROMPT_YESNO,
	S_PROMPT_YESNOQUIT,
	S_PROMPT_NOYES,
	S_PROMPT_NOYESQUIT,
	S_THROBBER_DEFAULT,
	S_PROMPT_PAUSE_BASIC,
	S_PROMPT_PAUSE_THROB,
};

#endif // __STRINGS_H

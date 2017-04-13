#ifndef __STRINGS_H
#define __STRINGS_H

#ifdef _DEFINE_GLOBALS_

char strings[MAXSTRINGS][512] = {
	"XX\0",									// S_HIDDEN_FILL
	//" ±\0",									// S_MPL_FILL
	" .\0",									// S_MPL_FILL
	"|08[|20|12\0",							// S_MPL_PREFIX
	"|16|08]|20|15\0",						// S_MPL_SUFFIX
	//"|08[|15\0",							// S_MPL_PREFIX
	//"|08]|15\0",						// S_MPL_SUFFIX
	"|15\0",									// S_MPL_FILL_COLOR
	"|15\0",									// S_MPL_TEXT_COLOR
	"Detecting ANSi...\0",					// S_MSG_DETECT_ANSI
	"|14A|06NSi |14D|06etected|NL\0",			// S_MSG_ANSI_DETECTED
	"No ANSi Support Detected|NL\0",			// S_MSG_NO_ANSI
	"|08P|01r|09oc|15eed with ANSi Graphi|07cs|08? \0",// S_MSG_KEEP_ANSI
	"|NL|14E|06nt|08er |06User Name|08, |06User Number|08, or '|06NEW|08'|NL   |14l|06ogin|08: \0",						// S_MSG_LOGIN
	"|14p|06assword|08: \0",					// S_MSG_PASSWORD
	"|14r|06eading |14e|06mail|08...|NL\0",	// S_MSG_EMAIL_READ
	"|14u|06nknown |14c|06ommand '|14%s|06'|NL\0",	// S_MSG_CMD_UNKNOWN
	"|15L|07o|08g|09O|01f|08f? \0",	// S_PROMPT_LOGOFF
	"|15Y|07e|08s|NL\0",						// S_YES
	"|09N|01o|NL\0",							// S_NO
	"|09Q|01uit|NL\0",							// S_QUIT
	"|08[|15CR|07/|15Y|08=|15Yes|08, |15N|08=|15No|08] \0",// S_PROMPT_YESNO
	"|08[|15CR|07/|15Y|08=|15Yes|08, |15N|08=|15No|08, |15Q|08=|15Quit|08] \0",// S_PROMPT_YESNOQUIT
	"|08[|15CR|07/|15N|08=|15No|08, |15Y|08=|15Yes|08] \0",// S_PROMPT_NOYES
	"|08[|15CR|07/|15N|08=|15No|08, |15Y|08=|15Yes|08, |15Q|08=|15Quit|08] \0",// S_PROMPT_NOYESQUIT
	"|07-|07/|07||07\\\0",				// S_THROBBER_DEFAULT
	"|05[PAUSE]\0",					// S_PROMPT_PAUSE_BASIC
	"|08 [ pause ]|08 [ |15p|08ause ]|08 [ |15pa|08use ]|08 [ |07p|15au|08se ]|08 [ |07pa|15us|08e ]|08 [ |07pau|15se|08 ]|08 [ p|07aus|15e|08 ]|08 [ pa|07use|08 ]|08 [ pau|07se|08 ]|08 [ paus|07e|08 ]|08 [ pause ]|08 [ pause ]|08 [ pause ]|08 [ pause ]|08 [ pause ]|08 [ pause ]\0", // S_PROMPT_PAUSE_THROB

	"Password Mismatch!  Try again.",	// S_PROMPT_PASSMISMATCH,

	// New User Forms and Prompts
	"Enter a Username: ", // S_PROMPT_NEWUSER_USERNAME
	"First Name: ", // S_PROMPT_NEWUSER_FIRSTNAME
	"Last Name: ", // S_PROMPT_NEWUSER_LASTNAME
	"Gender: ", // S_PROMPT_NEWUSER_GENDER
	"Password: ", // S_PROMPT_NEWUSER_PASSWORD
	"Re-Enter Password: ", // S_PROMPT_NEWUSER_PASSVERIFY
	"E-mail: ", // S_PROMPT_NEWUSER_EMAIL
	"URL: ", // S_PROMPT_NEWUSER_URL
	"Birthday: ", // S_PROMPT_NEWUSER_BIRTHDAY
	"I'm sorry, but that's not ok.  Goodbye.", // S_MSG_SORRY_BYE,
	"Invalid Username, try again.", // S_MSG_BADUSER,
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
	S_MSG_DETECT_ANSI,
	S_MSG_ANSI_DETECTED,
	S_MSG_NO_ANSI,
	S_MSG_KEEP_ANSI,
	S_MSG_LOGIN,
	S_MSG_PASSWORD,
	S_MSG_EMAIL_READ,
	S_MSG_CMD_UNKNOWN,
	S_PROMPT_LOGOFF,
	S_YES,
	S_NO,
	S_QUIT,
	S_PROMPT_YESNO,
	S_PROMPT_YESNOQUIT,
	S_PROMPT_NOYES,
	S_PROMPT_NOYESQUIT,
	S_THROBBER_DEFAULT,
	S_PROMPT_PAUSE_BASIC,
	S_PROMPT_PAUSE_THROB,
	S_PROMPT_PASSMISMATCH,
	S_PROMPT_NEWUSER_USERNAME,
	S_PROMPT_NEWUSER_FIRSTNAME,
	S_PROMPT_NEWUSER_LASTNAME,
	S_PROMPT_NEWUSER_GENDER,
	S_PROMPT_NEWUSER_PASSWORD,
	S_PROMPT_NEWUSER_PASSVERIFY,
	S_PROMPT_NEWUSER_EMAIL,
	S_PROMPT_NEWUSER_URL,
	S_PROMPT_NEWUSER_BIRTHDAY,
	S_MSG_SORRY_BYE,
	S_MSG_BADUSER,
};

#endif // __STRINGS_H

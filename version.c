
/*
 * version.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * BBS version strings and an output/display function.
 */

// $Id: version.c 24 2010-08-27 11:24:04Z mholger $

#include "vars.h"

char *bbs_version = "|14T|06erminus |14B|06BS v|140 |06(|14c|06)200|148 |15M|08. |15H|07o|08lger|07";
unsigned int bbs_build_num = 1;
char *bbs_date = __DATE__ ", " __TIME__;

char *bbs_long_name = "                             __  __  __\n  |_ __ __  _ _ ,        __ | _)| _)(__\n  | (-_|__)| ' )||`|(__)(__ |__)|__) __)\n  `--  `  `               -'\n";

void vers( int argc, char **argv )
{
	outstr( "%s|NL", bbs_version );

	return;
}


#include "vars.h"

/*
 * config.c - TerminusBBS (c)2010 M. Holger <mholger@optera.net>
 * All rights reseved.
 *
 * Description:
 * Functions supporting configuration loading/modifying/saving...
 */

void saveconfig(char *cf)
{
    int uf;
    char tmp[81];

    // Write Config
    sprintf(tmp, "%s", cf);
    uf = open(tmp, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (uf == -1) {
        printf("saveconfig() open(3) failed: %i\n", errno);
        bbsexit(100);
    }
    if (write(uf, &cfg, sizeof(configrec_t)) == -1) {
        printf("saveconfig() write(3) failed: %i\n", errno);
        bbsexit(101);
    }
}

void loadconfig(char *cf)
{
    int uf;
    char tmp[81];

    sprintf(tmp, "%s", cf);
    uf = open(tmp, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (uf == -1) {
        printf("loadconfig() open(3) failed: %i\n", errno);
        bbsexit(100);
    }

    if(read(uf, &cfg, sizeof(configrec_t))) {
        logger(3, "loadconfig(): Success!");
    }
}

/* configdump( argc, argv )
 * mnemonic callback to save user state and perform logoff procedures
 */
void configdump( int argc, char **argv )
{
    pnl();
    outstr("BBS Name: %s|NL", cfg.bbsname);
    outstr("Sysop: %s|NL", cfg.sysop);
    outstr("Sysop Email: %s|NL", cfg.email);
    outstr("Data Path: %s|NL", cfg.datapath);
    outstr("Plugin Path: %s|NL", cfg.pluginpath);
    outstr("Log Path: %s|NL", cfg.logpath);
    outstr("Text Path: %s|NL", cfg.textpath);
    outstr("Tmp Path: %s|NL", cfg.tmppath);
    outstr("Initial Menu: %s|NL", cfg.menu);
}

#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_BOOT_FILE "./config_boot.ini"

#include <stdio.h>

typedef enum {
    LANG_DEFAULT,
    LANG_EN = 1,
    LANG_IT = 2,
    LANG_NL = 3
} CFG_LANG;

extern char *cfg_boot_set[];
extern char *cfg_boot_set_lang[];
extern char *cfg_boot_set_style[];

char *CFG_BootFile_BoxRedirect(CFG_LANG lang);

FILE *CFG_BootFile_Init(CFG_LANG lang);
FILE *CFG_BootFile_LoadConfig(CFG_LANG lang);

int CFG_BootFile_ReadLine(FILE *fd, char *buffer);

int CFG_BootFile_ReadSet(char *buffer, char *dest);
int CFG_BootFile_ReadValue(char *buffer, char *dest);

int CFG_BootFile_ConfigRule(char *set, char *value);

#endif // CONFIG_H

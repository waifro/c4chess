#include <stdio.h>
#include <string.h>

#include "security/debug.h"
#include "lang.h"
#include "config.h"
#include "global.h"

char *cfg_boot_set[] = {
    "lang",
    "style",
    "background"
};

char *cfg_boot_set_lang[] = {
    "it",
    "en",
	"nl"
};

char *cfg_boot_set_style[] = {
    "boxed",
    "rounded"
};

char *CFG_BootFile_BoxRedirect(CFG_LANG lang) {
    switch(lang) {
    case LANG_DEFAULT:
        return (lang_boot_en);
    case LANG_EN:
        return (lang_boot_en);
    case LANG_IT:
        return (lang_boot_it);
    case LANG_NL:
        return (lang_boot_nl);
    }

    return (lang_boot_en);
}

FILE* CFG_BootFile_Init(CFG_LANG lang) {
    FILE *fd = fopen(CONFIG_BOOT_FILE, "w");

    fprintf(fd, "%s\n\n", CFG_BootFile_BoxRedirect(lang));

    FILE *nfd = freopen(CONFIG_BOOT_FILE, "r", fd);
    fclose(fd);

    return (nfd);
}

FILE *CFG_BootFile_LoadConfig(CFG_LANG lang) {

    FILE *fd = fopen(CONFIG_BOOT_FILE, "r");
    if (fd == NULL) fd = CFG_BootFile_Init(lang);

    char buffer[256];
    for (int n = 0; n < 256; n++) {
        if (CFG_BootFile_ReadLine(fd, buffer) < 2) continue;

        char set[256]; char value[256];
        CFG_BootFile_ReadSet(buffer, set);
        CFG_BootFile_ReadValue(buffer, value);

        CFG_BootFile_ConfigRule(set, value);
    }

    return (fd);
}


// reads the set and changes behavior on value
int CFG_BootFile_ReadLine(FILE *fd, char *buffer) {

    int let;
    for (int i = 0; i < 255; i++) {
        let = fgetc(fd);

        if (let == EOF) {
            buffer[i] = '\0';
            break;
        }

        if (let == '\n') {
            buffer[i] = '\0';
            break;
        }

        if (let == '#') {
            buffer[i] = '\0';
            while(1) {
                if (fgetc(fd) == '\n') break;
            }

            break;
        }

        buffer[i] = let;
    }

    return (strlen(buffer));
}

int CFG_BootFile_ReadSet(char *buffer, char *dest) {

    for (int n = 0; n < (int)strlen(buffer); n++)
        if (buffer[n] == '=') {
            strncpy(dest, buffer, n);
            break;
        }

    return (0);
}

int CFG_BootFile_ReadValue(char *buffer, char *dest) {

    for (int n = 0; n < (int)strlen(buffer); n++)
        if (buffer[n] == '=') {
            strncpy(dest, &buffer[++n], strlen(buffer));
            break;
        }

    return (0);
}


int CFG_BootFile_ConfigRule(char *set, char *value) {

    DEBUG_PrintBox(1, "CFG_BootFile_ConfigRule:");

    if (strncmp(set, cfg_boot_set[0], strlen(cfg_boot_set[0])) == 0) {

        CFG_LANG lang = LANG_ConfigLanguage(value);
        LANG_SetLanguage(lang);
        DEBUG_PrintBox(1, "  language set as: %s", value);

    } else if (strncmp(set, cfg_boot_set[1], strlen(cfg_boot_set[1])) == 0) {
        DEBUG_PrintBox(1, "  style set as: %s", value);
    }

    return (0);
}

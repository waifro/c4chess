#include <stdio.h>
#include <string.h>

#include "config.h"
#include "global.h"

char *_cfg_boot_set[] = {
    "lang",
    "style",
    "background"
};

char *_cfg_boot_set_lang[] = {
    "it",
    "en"
};

char *_cfg_boot_set_style[] = {
    "boxed",
    "rounded"
};


char *_cfg_boot_msg = {
    "# # # # # # # # # # # # # # GENERAL CONFIGURATION # # # # # # # # # # # #\n"
    "# This file was generated on first time boot.				 #\n"
    "#									 #\n"
    "# Here you can configure the game dinamics as you wish, 		 #\n"
    "# Every change's applied on the game will be saved here for next boot.	 #\n"
    "# Any applicable changes are set during boot				 #\n"
    "#									 #\n"
    "# For more information visit https://github.com/waifro/c4chess		 #\n"
    "# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #"
};

char *_lang_cfg_it[] = {
    "Errore", "Attenzione", "C'� qualcosa che non va", "Riprova", "Si", "No", "Okay", "Torna nel gioco", "Esci", "Continua",
    "Gioca", "Info", "Impostazioni", "Statistiche", "Prossimamente", "Sei Sicuro?", "Premi",
    "stile", "personalizza", "immetti", "sfondo immagine", "suoni", "musica", "locale", "globale", "giocatori",
    "lobby trovate", "indirizzo Ip", "per uscire", "per entrare", "in chat"
};

char *_lang_cfg_en[] = {
    "Error", "Attention", "There is something wrong", "Retry", "Yes", "No", "Okay", "Back to the game", "Exit", "Continue",
    "Play", "Info", "Settings", "Statistics", "Coming soon", "Are you sure?", "Press",
    "style", "personalize", "insert", "background", "sounds", "music", "local", "global", "players",
    "lobby found", "address Ip", "to exit", "to enter", "in chat"
};

FILE* CFG_BootFile_Init(void) {
    FILE *fd = fopen(CONFIG_BOOT_FILE, "w");

    fprintf(fd, "%s\n\n", _cfg_boot_msg);

    FILE *nfd = freopen(CONFIG_BOOT_FILE, "r", fd);
    fclose(fd);

    return (nfd);
}

FILE *CFG_BootFile_LoadConfig(void) {

    FILE *fd = fopen(CONFIG_BOOT_FILE, "r");
    if (fd == NULL) fd = CFG_BootFile_Init();

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

    printf("CFG_BootFile_ConfigRule:\n");

    if (strncmp(set, _cfg_boot_set[0], strlen(_cfg_boot_set[1])) == 0) {
        printf("  language set as: %s\n", value);
    } else if (strncmp(set, _cfg_boot_set[1], strlen(_cfg_boot_set[1])) == 0) {
        printf("  style set as: %s\n", value);
    }

    return (0);
}

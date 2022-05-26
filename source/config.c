#include <stdio.h>

#include "config.h"
#include "global.h"

char *_cfg_boot_set[] = {
    "language",
    "style",
    "workdir",
    "background"
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
    "Errore", "Attenzione", "C'è qualcosa che non va", "Riprova", "Si", "No", "Okay", "Torna nel gioco", "Esci", "Continua",
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

    char *buffer = NULL;
    for (int n = 0; n < 256; n++) {
        buffer = CFG_BootFile_ReadLine(fd, _cfg_boot_set[n]);
        if (buffer != NULL) printf("buf\t[%s]\n", buffer);
    }

    return (fd);
}


// reads the set and changes behavior on value
char *CFG_BootFile_ReadLine(FILE *fd, char *set) {

    static char buffer[256];

    int let;
    for (int i = 0; i < 255; i++) {
        let = fgetc(fd);

        if (let == '#') {
            buffer[i] = '\0';
            while(1) {
                let = fgetc(fd);
                if (let == '\n') break;
            } break;
        }

        if (let == EOF || let == '\n') {
            buffer[i] = '\0';
            break;
        }

        buffer[i] = let;
    }

    if (strlen(buffer) > 1) return (buffer);
        else return (NULL);
}

char *CFG_BootFile_ReadValue(char *buffer) {

    //

    return;
}


int CFG_BootFile_ConfigRule(char *set) {

    switch(set) {

    case _cfg_boot_set[0]:



        break;
    case _cfg_boot_set[1]:

        break;
    case _cfg_boot_set[2]:

        break;
    case _cfg_boot_set[3]:

        break;

    }

    return (0);
}

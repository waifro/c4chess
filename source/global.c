// third-party libraries
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

// local libraries
#include "global.h"
#include "pp4m/pp4m.h"

// declared global variables
SDL_Window *glo_window = NULL;
SDL_Renderer *glo_render = NULL;

int glo_screen_w = 1280;
int glo_screen_h = 720;

#define CONFIG_BOOT_FILE "./config_boot.ini"

char *_cfg_boot_msg = "# # # # # # # # # # # # # # GENERAL CONFIGURATION # # # # # # # # # # # #\n# This file was generated on first time boot.				#\n#									#\n# Here you can configure the game dinamics as you wish, 		#\n# Every change's applied on the game will be saved here for next boot.	#\n# Any applicable changes are set during boot				#\n#									#\n# For more information visit https://github.com/waifro/c4chess		#\n# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #";

char *_cfg_boot_set[] = {
    "language",
    "style",
    "workdir",
    "background"
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

FILE* GLOBAL_CfgBoot_Init(void) {
    FILE *fd = fopen(CONFIG_BOOT_FILE, "w");

    fprintf(fd, "%s\n\n", _cfg_boot_msg);

    FILE *nfd = freopen(CONFIG_BOOT_FILE, "r", fd);
    fclose(fd);

    return nfd;
}

void GLOBAL_CfgBoot_LoadConfig(void) {

    FILE *fd = fopen(CONFIG_BOOT_FILE, "r");
    if (fd == NULL) fd = GLOBAL_CfgBoot_Init();

    fclose(fd);
    return;
}

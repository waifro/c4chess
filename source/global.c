// third-party libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

// local libraries
#include "global.h"
#include "pp4m/pp4m.h"
#include "gen.h"

// declared global variables
SDL_Window *glo_window = NULL;
SDL_Renderer *glo_render = NULL;

int glo_screen_w = 1280;
int glo_screen_h = 720;

USER_STRUCT glo_user;

int GLOBAL_DestrUser(USER_STRUCT *user) {
    if (user == NULL) return -1;

    user->len = 0;
    free(user->username);
    user->username = NULL;

    return 0;
}

int GLOBAL_InitUser(USER_STRUCT *user, char *buffer) {
    if (user == NULL) return -1;

    int len = strlen(buffer);
    char *buf = malloc(sizeof(char) * len + 1);

    memset(buf, 0x00, len);
    strncpy(buf, buffer, len);

    user->username = buf;
    user->len = strlen(buf);

    return 0;
}

int GLOBAL_InitUser_Guest(USER_STRUCT *user) {
    if (user == NULL) return -1;

    char *buf = malloc(sizeof(char) * 17);

    char *guest = "guest_";
    char *random = GEN_PseudoGen_String(10);

    int n = 0;
    for (int i = 0; i < 17; i++) {
        if (i < 6)
            buf[i] = guest[i];

        else if (i > 5)
            buf[i] = random[n++];
    }

    user->username = buf;
    user->len = 16;

    return 0;
}

#ifndef GLOBAL_H
#define GLOBAL_H

// third-party libraries
#include <stdlib.h>
#include <errno.h>
#include <SDL2/SDL.h>

#include "lang.h"

// definitions
#define _SIZEOFARRAY(a) sizeof(a)/sizeof(*a)
#define BUF_MAX 0x10000 /* 65536 bytes */

#define DEBUG_LEVEL 2

typedef struct {
    int len;
    char *username;
    // more to come
} USER_STRUCT;

// declared extern variables
extern SDL_Window *glo_window;
extern SDL_Renderer *glo_render;

extern int glo_screen_w;
extern int glo_screen_h;

extern USER_STRUCT glo_user;

extern char **glo_lang;

// prototypes
int GLOBAL_DestrUser(USER_STRUCT *user);
int GLOBAL_InitUser(USER_STRUCT *user, char *buffer);
int GLOBAL_InitUser_Guest(USER_STRUCT *user);

#endif

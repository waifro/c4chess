#ifndef GLOBAL_H
#define GLOBAL_H

// third-party libraries
#include <stdlib.h>
#include <SDL2/SDL.h>

// definitions
#define SIZEOF(a) sizeof(a)/sizeof(*a)
#define BUF_MAX 0x10000
    /* 65536 bytes */

// declared extern variables
extern SDL_Window *glo_window;
extern SDL_Renderer *glo_render;

extern int glo_screen_w;
extern int glo_screen_h;

FILE* GLOBAL_CfgBoot_Init(void);
void GLOBAL_CfgBoot_LoadConfig(void);

#endif

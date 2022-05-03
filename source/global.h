#ifndef GLOBAL_H
#define GLOBAL_H

// third-party libraries
#include <SDL2/SDL.h>

// definitions
#define SIZEOF(a) sizeof(a)/sizeof(*a)

// declared extern variables
extern SDL_Window *glo_window;
extern SDL_Renderer *glo_render;

extern int glo_screen_w;
extern int glo_screen_h;

#endif

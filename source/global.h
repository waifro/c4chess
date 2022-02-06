#ifndef GLOBAL_H
#define GLOBAL_H

// third-party libraries
#include <SDL2/SDL.h>

// declared extern variables
extern SDL_Window *glo_window;
extern SDL_Renderer *glo_render;

extern char glo_current_dir[256];

extern int glo_screen_w;
extern int glo_screen_h;

#endif

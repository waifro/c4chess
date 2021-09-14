#ifndef TOUCH_H
#define TOUCH_H

#include <SDL2/SDL.h>

typedef struct {
    int x;
    int y;
    int iner;
} TOUCH_POS;

TOUCH_POS TOUCH_MouseState(SDL_Event *event);

#endif

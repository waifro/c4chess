#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>

int ANIM_UpdateRect(float deltaTime, int quantity, float time, SDL_Rect *src, SDL_Rect dest);

#endif

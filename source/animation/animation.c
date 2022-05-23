#include "animation.h"

#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>

int ANIM_UpdateRect(float deltaTime, int quantity, SDL_Rect *src, SDL_Rect dest) {

    if (src->x != dest.x || src->y != dest.y) {

        if (src->x < dest.x) src->x += quantity * (int)deltaTime / CLOCKS_PER_SEC;
        if (src->x > dest.x) src->x -= quantity * (int)deltaTime / CLOCKS_PER_SEC;
        if (src->y < dest.y) src->y += quantity * (int)deltaTime / CLOCKS_PER_SEC;
        if (src->y > dest.y) src->y -= quantity * (int)deltaTime / CLOCKS_PER_SEC;

        return -1;

    }

    return 0;
}

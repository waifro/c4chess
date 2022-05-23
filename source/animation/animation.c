#include "animation.h"

#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>

int ANIM_UpdateRect(float deltaTime, int quantity, SDL_Rect *src, SDL_Rect dest) {

    printf("x %d, y %d\n", src->x, src->y);

    if (src->x != dest.x && src->y != dest.y) {

        src->x += quantity * deltaTime / CLOCKS_PER_SEC;
        src->y += quantity * deltaTime / CLOCKS_PER_SEC;
        return -1;

    }

    return 0;
}

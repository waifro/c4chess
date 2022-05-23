#include "animation.h"

#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

int ANIM_UpdateRect(float deltaTime, int quantity, float time, SDL_Rect *src, SDL_Rect dest) {

    /*
    if (src->x != dest.x || src->y != dest.y) {

        if (src->x < dest.x) src->x += quantity * (int)deltaTime / CLOCKS_PER_SEC;
        if (src->x > dest.x) src->x -= quantity * (int)deltaTime / CLOCKS_PER_SEC;
        if (src->y < dest.y) src->y += quantity * (int)deltaTime / CLOCKS_PER_SEC;
        if (src->y > dest.y) src->y -= quantity * (int)deltaTime / CLOCKS_PER_SEC;

        return -1;

    }
    */

    static SDL_Rect src_bak;
    static bool door = false;
    if (door == false) {
        src_bak = *src;
        door = true;
    }

    static float x1 = 0.0f;
    static float y1 = 0.0f;
    static int n = 0;

    if (src->x != dest.x || src->y != dest.y) {

        if (src->x > dest.x || src->x < dest.x) {
            x1 += deltaTime / CLOCKS_PER_SEC + ((src_bak.x > dest.x ? src_bak.x - dest.x : src_bak.x + dest.x) / time);
        }

        if (src->y > dest.y || src->y < dest.y) {
            y1 += deltaTime / CLOCKS_PER_SEC + ((src_bak.y > dest.y ? src_bak.y - dest.y : src_bak.y + dest.y) / time);
        }

        printf("%d %f %d %f: %d\n", (int)x1, x1, (int)y1, y1, n);

        if (x1 > 1.0f) {
            if (src->x < dest.x) src->x += (int)x1;
            else if (src->x > dest.x) src->x -= (int)x1;
            x1 = 0.0f;
        }

        if (y1 > 1.0f) {
            if (src->y < dest.y) src->y += (int)y1;
            else if (src->y > dest.y) src->y -= (int)y1;
            y1 = 0.0f;
        }


        n++;
        return -1;
    }

    src_bak.x = 0;
    src_bak.y = 0;
    src_bak.w = 0;
    src_bak.h = 0;
    door = false;
    n = 0;

    return 0;
}

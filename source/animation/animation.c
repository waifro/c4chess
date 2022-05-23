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

    static int delta_x;
    static int delta_y;

    if (door == false) {

        src_bak = *src;

        door = true;

    }

    float x1 = 0.0f;
    float y1 = 0.0f;
    static int n = 0;


    if (src->x != dest.x || src->y != dest.y) {

        /*
        if (src->x > dest.x || src->x < dest.x) {
            x1 += ((src_bak.x > dest.x ? src_bak.x - dest.x : src_bak.x + dest.x) * deltaTime / CLOCKS_PER_SEC) / time;
        }

        if (src->y > dest.y || src->y < dest.y) {
            y1 += ((src_bak.y > dest.y ? src_bak.y - dest.y : src_bak.y + dest.y) * deltaTime / CLOCKS_PER_SEC) / time;
        }
        */

        if (src->x > dest.x || src->x < dest.x) {
            delta_x = (src_bak.x > dest.x ? src_bak.x - dest.x : src_bak.x + dest.x) / time;
            x1 = delta_x * deltaTime / time;
        }

        if (src->y > dest.y || src->y < dest.y) {
            delta_y = (src_bak.y > dest.y ? src_bak.y - dest.y : src_bak.y + dest.y) / time;
            y1 = delta_y * deltaTime / time;
        }

        printf("%d %f %d %f: %d\n", (int)x1, x1, (int)y1, y1, n);

        if (src->x < dest.x) src->x += (int)x1;
        else if (src->x > dest.x) src->x -= (int)x1;

        if (src->y < dest.y) src->y += (int)y1;
        else if (src->y > dest.y) src->y -= (int)y1;

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

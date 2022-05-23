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

    static float delta_time_x = 0;
    static float delta_time_y = 0;

    if (door == false) {

        src_bak = *src;

        if (src_bak.x != dest.x) delta_time_x = (src_bak.x > dest.x ? src_bak.x - dest.x : src_bak.x + dest.x) / time;
        if (src_bak.y != dest.y) delta_time_y = (src_bak.y > dest.y ? src_bak.y - dest.y : src_bak.y + dest.y) / time;

        printf("delta_x %f, delta_y %f, deltaTime %f\n", delta_time_x, delta_time_y, deltaTime);

        door = true;

    }

    static float x1 = 0.0f;
    static float y1 = 0.0f;
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

        x1 += delta_time_x / (deltaTime / time); // / CLOCKS_PER_SEC;
        y1 += delta_time_y / (deltaTime / time); // / CLOCKS_PER_SEC;


        printf("%d %f %d %f: %d\n", (int)x1, x1, (int)y1, y1, n);

        if (src->x < dest.x) src->x += (int)x1;
        else if (src->x > dest.x) src->x -= (int)x1;
         if (x1 > delta_time_x) x1 = delta_time_x;

        if (src->y < dest.y) src->y += (int)y1;
        else if (src->y > dest.y) src->y -= (int)y1;
        if (y1 > delta_time_y) y1 = delta_time_y;

        if (++n >= time) return 0;
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

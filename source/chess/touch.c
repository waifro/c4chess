#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "touch.h"

TOUCH_POS TOUCH_MouseState(SDL_Event *event) {

    static bool once = false;
    TOUCH_POS foo; foo.iner = -1;

    if (once == false && event->type == SDL_MOUSEBUTTONDOWN) {

        printf("TOUCH_MouseState:\n  touch detected\n");
        SDL_GetMouseState(&foo.x, &foo.y);
        foo.iner = 1;
        once = true;

    } else if (once == true && event->type == SDL_MOUSEBUTTONUP) {

        printf("TOUCH_MouseState:\n  touch undetected\n");
        foo.iner = -1;
        once = false;

    }

    return foo;
}

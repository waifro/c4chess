#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <sys/select.h>
#endif // _WIN32

// third-party libraries
#include <SDL2/SDL.h>

#include "pp4m_input.h"

int pp4m_INPUT_KeyboardHit(void) {

    #ifdef _WIN32
    int val = _kbhit();
    if (val == 1)
        val = getch();
    return (val);
    #else

    /*
    Linux (POSIX) implementation of _kbhit().
    Morgan McGuire, morgan@cs.brown.edu
    */
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;

    #endif // _WIN32
}



void pp4m_INPUT_GetMouseState(SDL_Event *event, PP4M_INPUT_POS *foo) {

    foo->iner = -1;

    SDL_GetMouseState(&foo->x, &foo->y);

    if (foo->iner == 0 && event->type == SDL_MOUSEBUTTONDOWN)
        foo->iner = 0;

    else if (foo->iner == 1 && event->type == SDL_MOUSEBUTTONDOWN)
        foo->iner = 0;

    else if (event->type == SDL_MOUSEBUTTONDOWN)
        foo->iner = 1;

    else
        foo->iner = -1;

    return;
}

PP4M_INPUT_POS pp4m_INPUT_MouseState(SDL_Event *event) {

    static bool once = false;
    PP4M_INPUT_POS foo; foo.iner = -1;

    if (once == false && event->type == SDL_MOUSEBUTTONDOWN) {

        //printf("TOUCH_MouseState:\n  touch detected\n");
        SDL_GetMouseState(&foo.x, &foo.y);
        foo.iner = 1;
        once = true;

    } else if (once == true && event->type == SDL_MOUSEBUTTONUP) {

        //printf("TOUCH_MouseState:\n  touch undetected\n");
        foo.iner = -1;
        once = false;

    }

    return foo;
}

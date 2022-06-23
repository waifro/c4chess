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

int pp4m_INPUT_ConvertSdlKey_number(int sdlk) {
    int result = -1;

    switch(sdlk) {
    default:
        break;

    case SDLK_0:
        result = '0';
        break;
    case SDLK_1:
        result = '1';
        break;
    case SDLK_2:
        result = '2';
        break;
    case SDLK_3:
        result = '3';
        break;
    case SDLK_4:
        result = '4';
        break;
    case SDLK_5:
        result = '5';
        break;
    case SDLK_6:
        result = '6';
        break;
    case SDLK_7:
        result = '7';
        break;
    case SDLK_8:
        result = '8';
        break;
    case SDLK_9:
        result = '9';
        break;
    }

    return result;
}

int pp4m_INPUT_ConvertSdlKey_letter(int sdlk) {
    int result = -1;

    switch(sdlk) {
    default:
        break;

    case SDLK_SPACE:
        result = ' ';
        break;
    case SDLK_a:
        result = 'a';
        break;
    case SDLK_b:
        result = 'b';
        break;
    case SDLK_c:
        result = 'c';
        break;
    case SDLK_d:
        result = 'd';
        break;
    case SDLK_e:
        result = 'e';
        break;
    case SDLK_f:
        result = 'f';
        break;
    case SDLK_g:
        result = 'g';
        break;
    case SDLK_h:
        result = 'h';
        break;
    case SDLK_i:
        result = 'i';
        break;
    case SDLK_j:
        result = 'j';
        break;
    case SDLK_k:
        result = 'k';
        break;
    case SDLK_l:
        result = 'l';
        break;
    case SDLK_m:
        result = 'm';
        break;
    case SDLK_n:
        result = 'n';
        break;
    case SDLK_o:
        result = 'o';
        break;
    case SDLK_p:
        result = 'p';
        break;
    case SDLK_q:
        result = 'q';
        break;
    case SDLK_r:
        result = 'r';
        break;
    case SDLK_s:
        result = 's';
        break;
    case SDLK_t:
        result = 't';
        break;
    case SDLK_u:
        result = 'u';
        break;
    case SDLK_v:
        result = 'v';
        break;
    case SDLK_w:
        result = 'w';
        break;
    case SDLK_x:
        result = 'x';
        break;
    case SDLK_y:
        result = 'y';
        break;
    case SDLK_z:
        result = 'z';
        break;
    }

    return result;
}

int pp4m_INPUT_ConvertSdlKey_symbol(int sdlk) {
    int result = -1;

    switch (sdlk) {
    default:
        break;

    case SDLK_EXCLAIM:
        result = '!';
        break;
    case SDLK_QUESTION:
        result = '?';
        break;
    case SDLK_COLON:
        result = ':';
        break;
    case SDLK_LEFTPAREN:
        result = '(';
        break;
    case SDLK_RIGHTPAREN:
        result = ')';
        break;
    }

    return result;
}

int pp4m_INPUT_SdlSpecialKey(int sdlk) {
    int result = -1;

    switch(sdlk) {
    default:
        break;

    case SDLK_BACKSPACE: // backspace key
        result = -2;
        break;
    case SDLK_RETURN: // enter key
        result = -3;
        break;
    case SDLK_LSHIFT: // left shift
        result = -4;
        break;
    case SDLK_RSHIFT: // right shift
        result = -5;
        break;
    case SDLK_ESCAPE: // escape key
        result = -6;
        break;
    case SDLK_DELETE: // backspace key (macOS)
        result = -7;
        break;
    }

    return result;
}

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

int pp4m_INPUT_SdlKeyboardKey(int sdlk) {
    int result = -1;

    result = pp4m_INPUT_SdlSpecialKey(sdlk);
    if (result != -1) return result;

    result = pp4m_INPUT_ConvertSdlKey_letter(sdlk);
    if (result != -1) return result;

    result = pp4m_INPUT_ConvertSdlKey_symbol(sdlk);
    if (result != -1) return result;

    result = pp4m_INPUT_ConvertSdlKey_number(sdlk);
    if (result != -1) return result;

    return 0;
}

int pp4m_INPUT_SdlKeyboard(SDL_Event *event) {
    int result = 0;

    while(SDL_PollEvent(event)) {
        if (event->type == SDL_QUIT) result = -1;
        else if (event->type == SDL_KEYDOWN) result = pp4m_INPUT_SdlKeyboardKey(event->key.keysym.sym);
    }

    return result;
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

        printf("TOUCH_MouseState:  touch detected\n");
        SDL_GetMouseState(&foo.x, &foo.y);
        foo.iner = 1;
        once = true;

    } else if (once == true && event->type == SDL_MOUSEBUTTONUP) {

        printf("TOUCH_MouseState:  touch undetected\n");
        foo.iner = -1;
        once = false;

    }

    return foo;
}

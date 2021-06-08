/* Private Project Four Me */

#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "pp4m.h"
#include "pp4m_image.h"
#include "pp4m_ttf.h"

SDL_Color PP4M_BLACK = {0, 0, 0, 255};
SDL_Color PP4M_WHITE = {255, 255, 255, 255};

SDL_Color PP4M_RED = {255, 0, 0, 255};
SDL_Color PP4M_GREEN = {0, 255, 0, 255};
SDL_Color PP4M_BLUE = {0, 0, 255, 255};

SDL_Color PP4M_YELLOW = {255, 255, 0, 255};
SDL_Color PP4M_PINK = {255, 0, 255, 255};
SDL_Color PP4M_CYAN = {0, 255, 255, 255};

SDL_Renderer *pp4m_Init(SDL_Window *window, char title[256], int width, int height) {

    SDL_Init(SDL_INIT_EVERYTHING);
    pp4m_IMG_Init();
    pp4m_TTF_Init();

    SDL_Renderer *renderer = NULL;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    return renderer;

}


void pp4m_Quit(void) {

    pp4m_TTF_Quit();
    pp4m_IMG_Quit();
    SDL_Quit();

    return;

}

void pp4m_GetDateAndTime(char *dst) {

    time_t date;
    time(&date);
    strcpy(dst, ctime(&date));

    return;
}

int pp4m_SecondsTick(void) {

    static int result = 0;
    static bool copy_once = false;
    static clock_t ticks_dif;

    clock_t ticks_cur = clock();

    if (copy_once == false) {
        ticks_dif = ticks_cur;
        copy_once = true;
    }

    if ((ticks_cur / 1000) != (ticks_dif / 1000)) {
        copy_once = false;
        result ^= 1;
    }

    return result;
}

int pp4m_Framerate(void) {

    static int count = 0;
    int result = 0;

    // count each frame after rendering
    static int frame = 0;
    frame += 1;

    // get ticks per milliseconds
    unsigned int ticks;
    ticks = clock();

    if (pp4m_SecondsTick() != count) {
        count ^= 1;
        result = (frame / (ticks / 1000));
    }

    return result;
}

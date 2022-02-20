/* Private Project Four Me */

#ifndef _PP4M_H
#define _PP4M_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Color PP4M_BLACK;
extern SDL_Color PP4M_GREY;
extern SDL_Color PP4M_GREY_DARK;
extern SDL_Color PP4M_WHITE;

extern SDL_Color PP4M_RED;
extern SDL_Color PP4M_GREEN;
extern SDL_Color PP4M_BLUE;

extern SDL_Color PP4M_YELLOW;
extern SDL_Color PP4M_PINK;
extern SDL_Color PP4M_CYAN;

typedef struct {
    char text[1024];
    SDL_Rect rect;
    SDL_Color color;
    TTF_Font *font;
    SDL_Surface *surface;
    SDL_Texture *texture;
} PP4M_SDL;

typedef enum {

    WINDOW,
    WINDOW_FULLSCREEN,
    WINDOW_RESIZABLE

} PP4M_WINDOW_SIZE;

SDL_Renderer *pp4m_Init(SDL_Window *window, char *title, int width, int height, PP4M_WINDOW_SIZE size);
void pp4m_Quit(SDL_Window *window, SDL_Renderer *renderer);

void pp4m_GetDateAndTime(char *dst);

int pp4m_SecondsTick(void) ;
int pp4m_Framerate(void);

#endif // _PP4M_H

/* 2022 @waifro */
/* Private Project Four Me */

#ifndef _PP4M_DRAW_H
#define _PP4M_DRAW_H

#include <SDL2/SDL.h>

typedef struct {

    int x;
    int y;
    int x2;
    int y2;

} PP4M_PointToPoint;

SDL_Texture *pp4m_DRAW_CreateTexture(SDL_Renderer *renderer, int width, int height);
void pp4m_DRAW_TextureDrawLine(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, PP4M_PointToPoint *ptp, int x, int y, int x2, int y2);
void pp4m_DRAW_TextureDrawPoint(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, int x, int y, int radius);
SDL_Texture *pp4m_DRAW_TextureRect(SDL_Renderer *renderer, SDL_Color color, SDL_Rect *rect, float x, float y, float w, float h);

#endif // _PP4M_DRAW_H

/* 2021 @richardwaifro */

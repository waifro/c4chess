#ifndef DOT_H
#define DOT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct
{
    SDL_Texture *texture;
    SDL_Rect rect;
    bool state;
} CHESS_DOT;

extern CHESS_DOT glo_chess_dot[64];

void DOT_InitTexture(int tile);
void DOT_InitGlobalDot(void);

void DOT_StateGlobalDot(int tile);
void DOT_StateGlobalDotReset(void);

#endif

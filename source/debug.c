#include <stdio.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_ttf.h"

#include "main.h"
#include "debug.h"

PP4M_SDL DebugInfo[64];
char *DEBUG_FONT = "resources/OpenSans-Regular.ttf";

void DEBUG_WriteTextureFont(char *text, int colomn) {

    int x = DEBUG_START_POSITION_X;
    int y = DEBUG_START_POSITION_Y;

    int width = 0; int height = 0;

    SDL_QueryTexture(DebugInfo[colomn-1].texture, NULL, NULL, &width, &height);
    y += (height + 5);

    DebugInfo[colomn].texture = pp4m_TTF_TextureFont(global_renderer, DebugInfo[colomn].texture, DEBUG_FONT, PP4M_WHITE, 24, &DebugInfo[colomn].rect, x, y, text);

    return;
}

void DEBUG_Print(int colomn) {

    SDL_RenderCopy(global_renderer, DebugInfo[colomn].texture, NULL, &DebugInfo[colomn].rect);

    return;
}

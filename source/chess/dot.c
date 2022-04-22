#include <stdio.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m.h"
#include "../global.h"

#include "dot.h"
#include "core.h"

CHESS_DOT glo_chess_dot[64];

void DOT_InitTexture(int tile) {

    glo_chess_dot[tile].texture = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_SetTextureBlendMode(glo_chess_dot[tile].texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(glo_chess_dot[tile].texture, 75);

    pp4m_DRAW_TextureDrawCircle_Filled(glo_render, glo_chess_dot[tile].texture, PP4M_BLACK, (glo_chess_dot[tile].rect.w / 2), (glo_chess_dot[tile].rect.h / 2), 15);

    return;
}

void DOT_GlobalDot_Init(void) {

    for (int n = 0; n < 64; n++) {

        glo_chess_dot[n].rect = glo_chess_core_tile[n].rect;
        DOT_InitTexture(n);

    }

    return;
}

void DOT_StateGlobalDot(int tile) {
    if (glo_chess_dot[tile].state == true) { SDL_RenderCopy(glo_render, glo_chess_dot[tile].texture, NULL, &glo_chess_dot[tile].rect); }
    return;
}

void DOT_StateGlobalDotReset(void) {
    for (int n = 0; n < 64; n++) {
        if (glo_chess_dot[n].state == true) glo_chess_dot[n].state = false;
    }
    return;
}

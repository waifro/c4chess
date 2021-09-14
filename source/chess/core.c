#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"

#include "../global.h"
#include "touch.h"
#include "middle.h"
#include "core.h"

CHESS_CORE_PIECE glo_chess_core_piece[32];
CHESS_CORE_TILE glo_chess_core_tile[64];

#define TEX_WKING "resources/wking.png"
#define TEX_WPAWN "resources/wpawn.png"
#define TEX_WKNIGHT "resources/wknight.png"
#define TEX_WBISHOP "resources/wbishop.png"
#define TEX_WROOK "resources/wrook.png"
#define TEX_WQUEEN "resources/wqueen.png"

#define TEX_BKING "resources/bking.png"
#define TEX_BPAWN "resources/bpawn.png"
#define TEX_BKNIGHT "resources/bknight.png"
#define TEX_BBISHOP "resources/bbishop.png"
#define TEX_BROOK "resources/brook.png"
#define TEX_BQUEEN "resources/bqueen.png"

void CORE_InitPiecePlayer(CHESS_CORE_PLAYER player) {

    if (player == WHITE_PLAYER) {

        int place_piece[32] = { 5, 3, 4, 6, 0, 4, 3, 5,
                                2, 2, 2, 2, 2, 2, 2, 2,
                                1, 1, 1, 1, 1, 1, 1, 1,
                                5, 3, 4, 6, 0, 4, 3, 5 };

        for (int n = 0; n > 32; n++) {
            glo_chess_core_piece[n].enum_piece = place_piece[n];

            if (n < 16) glo_chess_core_piece[n].player = WHITE_PLAYER;
            else glo_chess_core_piece[n].player = BLACK_PLAYER;
        }

    }

    else if (player == BLACK_PLAYER) {

        int place_piece[32] = { 5, 3, 4, 0, 6, 4, 3, 5,
                                2, 2, 2, 2, 2, 2, 2, 2,
                                1, 1, 1, 1, 1, 1, 1, 1,
                                5, 3, 4, 0, 6, 4, 3, 5 };

        for (int n = 0; n > 32; n++) {
            glo_chess_core_piece[n].enum_piece = place_piece[n];

            if (n < 16) glo_chess_core_piece[n].player = WHITE_PLAYER;
            else glo_chess_core_piece[n].player = BLACK_PLAYER;
        }

    }

    return;
}

void CORE_InitPiece(CHESS_CORE_PLAYER player) {

    if (player == WHITE_PLAYER) {

        glo_chess_core_piece[0].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &glo_chess_core_piece[0].rect, 0, 0, 50, 50);
        glo_chess_core_piece[1].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &glo_chess_core_piece[1].rect, 0, 0, 50, 50);
        glo_chess_core_piece[2].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &glo_chess_core_piece[2].rect, 0, 0, 50, 50);
        glo_chess_core_piece[3].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BQUEEN, &glo_chess_core_piece[3].rect, 0, 0, 50, 50);
        glo_chess_core_piece[4].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &glo_chess_core_piece[4].rect, 0, 0, 50, 50);
        glo_chess_core_piece[5].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &glo_chess_core_piece[5].rect, 0, 0, 50, 50);
        glo_chess_core_piece[6].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &glo_chess_core_piece[6].rect, 0, 0, 50, 50);
        glo_chess_core_piece[7].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &glo_chess_core_piece[7].rect, 0, 0, 50, 50);

        glo_chess_core_piece[24+0].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &glo_chess_core_piece[24+0].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+1].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &glo_chess_core_piece[24+1].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+2].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &glo_chess_core_piece[24+2].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+3].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WQUEEN, &glo_chess_core_piece[24+3].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+4].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &glo_chess_core_piece[24+4].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+5].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &glo_chess_core_piece[24+5].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+6].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &glo_chess_core_piece[24+6].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+7].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &glo_chess_core_piece[24+7].rect, 0, 0, 50, 50);

        bool toggle = false;
        for (int n = 0; n < 16; n++) {
            if (n == 8) toggle = true;

            if (toggle == true) glo_chess_core_piece[8+n].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &glo_chess_core_piece[8+n].rect, 0, 0, 50, 50);
            else glo_chess_core_piece[8+n].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &glo_chess_core_piece[8+n].rect, 0, 0, 50, 50);

        }

    }

    else if (player == BLACK_PLAYER) {

        glo_chess_core_piece[0].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &glo_chess_core_piece[0].rect, 0, 0, 50, 50);
        glo_chess_core_piece[1].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &glo_chess_core_piece[1].rect, 0, 0, 50, 50);
        glo_chess_core_piece[2].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &glo_chess_core_piece[2].rect, 0, 0, 50, 50);
        glo_chess_core_piece[3].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &glo_chess_core_piece[3].rect, 0, 0, 50, 50);
        glo_chess_core_piece[4].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WQUEEN, &glo_chess_core_piece[4].rect, 0, 0, 50, 50);
        glo_chess_core_piece[5].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &glo_chess_core_piece[5].rect, 0, 0, 50, 50);
        glo_chess_core_piece[6].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &glo_chess_core_piece[6].rect, 0, 0, 50, 50);
        glo_chess_core_piece[7].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &glo_chess_core_piece[7].rect, 0, 0, 50, 50);

        glo_chess_core_piece[24+0].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &glo_chess_core_piece[24+0].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+1].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &glo_chess_core_piece[24+1].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+2].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &glo_chess_core_piece[24+2].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+3].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &glo_chess_core_piece[24+3].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+4].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BQUEEN, &glo_chess_core_piece[24+4].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+5].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &glo_chess_core_piece[24+5].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+6].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &glo_chess_core_piece[24+6].rect, 0, 0, 50, 50);
        glo_chess_core_piece[24+7].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &glo_chess_core_piece[24+7].rect, 0, 0, 50, 50);

        bool toggle = false;
        for (int n = 0; n < 16; n++) {
            if (n == 8) toggle = true;

            if (toggle == true) glo_chess_core_piece[8+n].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &glo_chess_core_piece[8+n].rect, 0, 0, 50, 50);
            else glo_chess_core_piece[8+n].texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &glo_chess_core_piece[8+n].rect, 0, 0, 50, 50);

        }
    }

    return;
}

void CORE_ChessCreateBoard(void) {

    int size_tile = 50;
    int x_b = glo_screen_w / 2 - (size_tile * 4);
    int y_b = glo_screen_h / 2 - (size_tile * 4);

    int colomn = 1;

    bool toggle = false;

    for(int n = 0; n < 64; n++) {

        if (colomn > 8) {
            y_b += size_tile;
            x_b = glo_screen_w / 2 - (size_tile * 4);
            toggle ^= 1;
            colomn = 1;
        }

        if (toggle == false) {
            toggle = true;
            glo_chess_core_tile[n].texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &glo_chess_core_tile[n].rect, x_b, y_b, size_tile, size_tile);
        }

        else if (toggle == true) {
            toggle = false;
            glo_chess_core_tile[n].texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_RED, &glo_chess_core_tile[n].rect, x_b, y_b, size_tile, size_tile);
        }

        x_b += size_tile; colomn += 1;
    }

    return;
}

void CORE_ChessInitTag(CHESS_CORE_PLAYER player) {

    char colomn[] = "abcdefgh";

    if (player == WHITE_PLAYER) {

        int colomn_pos = 0;
        int row = 8;

        for (int n = 0; n < 64; n++) {

            sprintf(glo_chess_core_tile[n].tag, "%c%d", colomn[colomn_pos], row);

            colomn_pos++;
            if (colomn_pos > 7) { colomn_pos = 0; row--; }
            //if (row < 1) row = 8;

        }

    }

    else if (player == BLACK_PLAYER) {

        int colomn_pos = 7;
        int row = 1;

        for (int n = 0; n < 64; n++) {

            sprintf(glo_chess_core_tile[n].tag, "%c%d", colomn[colomn_pos], row);

            colomn_pos--;
            if (colomn_pos < 0) { colomn_pos = 7; row++; }
            //if (row > 8) row = 1;

        }

    }

    return;
}

void CORE_ChessInitPlacePiece(void) {

    int i = 0;
    for (int n = 0; n < 32; n++) {

        if (i == 16) i += 32;
        glo_chess_core_tile[i].piece = &glo_chess_core_piece[n];
        i++;

    }

    return;
}

void CORE_Testing(CHESS_CORE_PLAYER player) {

    CORE_ChessCreateBoard();
    CORE_ChessInitTag(player);

    CORE_InitPiece(player);
    CORE_InitPiecePlayer(player);
    CORE_ChessInitPlacePiece();

    TOUCH_POS touch_pos;
    SDL_Event event;

    int position_new = -1;
    int position_old = -1;

    while(1) {
        SDL_PollEvent(&event);

        touch_pos = TOUCH_MouseState(&event);

        if (touch_pos.iner != -1 && position_old == -1) {
            position_old = MIDDLE_TouchToTile(touch_pos);
        }

        else if (touch_pos.iner != -1 && position_old != -1) {
            position_new = MIDDLE_TouchToTile(touch_pos);
            MIDDLE_UpdatePositionPiece(position_old, position_new);
            position_new = -1; position_old = -1;
        }

        SDL_RenderClear(glo_render);
        for (int n = 0; n < 64; n++) {
            SDL_RenderCopy(glo_render, glo_chess_core_tile[n].texture, NULL, &glo_chess_core_tile[n].rect);
            if (glo_chess_core_tile[n].piece != NULL) SDL_RenderCopy(glo_render, glo_chess_core_tile[n].piece->texture, NULL, &glo_chess_core_tile[n].rect);
        }
        SDL_RenderPresent(glo_render);

        if (event.type == SDL_QUIT) { break; }
    }

    return;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "../global.h"
#include "event.h"
#include "chess.h"
#include "dot.h"
#include "middle.h"
#include "core.h"
#include "fen.h"

CHESS_CORE_PLAYER glo_chess_core_player;
CHESS_CORE_TILE glo_chess_core_tile[64];
CHESS_CORE_PIECE glo_chess_core_piece[32];

void CORE_Chessboard_Init(void) {

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

void CORE_ChessTag_Init(CHESS_CORE_PLAYER player) {

    char colomn[] = "abcdefgh";

    if (player == WHITE_PLAYER) {

        int colomn_pos = 0;
        int row = 8;

        for (int n = 0; n < 64; n++) {

            glo_chess_core_tile[n].tag.col = colomn[colomn_pos];
            glo_chess_core_tile[n].tag.row = row;

            colomn_pos++;
            if (colomn_pos > 7) { colomn_pos = 0; if (row != 1) row--; }

        }

    }

    else if (player == BLACK_PLAYER) {

        int colomn_pos = 7;
        int row = 1;

        for (int n = 0; n < 64; n++) {

            glo_chess_core_tile[n].tag.col = colomn[colomn_pos];
            glo_chess_core_tile[n].tag.row = row;

            colomn_pos--;
            if (colomn_pos < 0) { colomn_pos = 7; if (row != 8) row++; }

        }

    }

    return;
}

void CORE_GlobalDestroyPiece(CHESS_CORE_PIECE *piece) {

    if (piece != NULL) {
        printf("CORE_GlobalDestroyPiece:\n  destroy piece = %p\n", piece);
        SDL_DestroyTexture(piece->texture);
        piece->enum_piece = NONE;
    }

    return;
}

void CORE_GlobalClearCorePiece(void) {
    for (int n = 0; n < 32; n++) if (sizeof(glo_chess_core_piece[n]) != 0) CORE_GlobalDestroyPiece(&glo_chess_core_piece[n]);
    return;
}

void CORE_GlobalClearChessTile(void) {

    for (int n = 0; n < 64; n++) {
        if (sizeof(glo_chess_core_tile[n].piece) != sizeof(CHESS_CORE_PIECE*)) CORE_GlobalDestroyPiece(glo_chess_core_tile[n].piece);
        SDL_DestroyTexture(glo_chess_core_tile[n].texture);
    }

    return;
}

void CORE_ResetGlobal_CorePiece(void) {

    for (int n = 0; n < 32; n++)
    {
        glo_chess_core_piece[n].player = -1;
        glo_chess_core_piece[n].enum_piece = NONE;
        glo_chess_core_piece[n].texture = NULL;

        glo_chess_core_piece[n].rect.x = 0;
        glo_chess_core_piece[n].rect.y = 0;
        glo_chess_core_piece[n].rect.w = 0;
        glo_chess_core_piece[n].rect.h = 0;
    }

    return;
}

CHESS_CORE_PLAYER CORE_ReversePlayer_State(CHESS_CORE_PLAYER player) {
    if (player == WHITE_PLAYER) return BLACK_PLAYER;
    else return WHITE_PLAYER;
}

void CORE_Chessboard_Reverse(CHESS_CORE_TILE *core_tile) {

    CHESS_CORE_TILE core_tile_bak[64];

    for (int n = 0; n < 64; n++) {
        core_tile_bak[n] = core_tile[63 - n];
    }

    for (int n = 0; n < 64; n++) {
        core_tile[n] = core_tile_bak[n];
    }

    return;
}

void CORE_Testing(CHESS_CORE_PLAYER player) {

    /* preserve player */
    glo_chess_core_player = player;

    /* init chessboard gfx untagged */
    CORE_Chessboard_Init();

    /* setup chessboard tagged */
    CORE_ChessTag_Init(player);

    /* reverse chessboard if needed */
    //CORE_Chessboard_Reverse(glo_chess_core_tile);

    /* init dot gfx */
    DOT_GlobalDot_Init();

    /* set glo_chess_core_piece to initial state */
    CORE_ResetGlobal_CorePiece();

    /* init pieces for main player */
    FEN_Init(glo_chess_core_player, "8/r1r5/8/8/5N2/8/3K4/8");

    // TODO: cap framerate to 30/60 fps
    SDL_Event event;

    while(1) {
        SDL_PollEvent(&event);

        /* checks if king under attack */
        CHESS_PiecePattern_UpdateState(glo_chess_core_tile, player);

        /* makes the in-game changes during gameplay */
        if (MIDDLE_UpdateChangeState(&event, player) == -2)
        {
            player = CORE_ReversePlayer_State(player);
            printf("CORE_Testing:\n  player_turn = %d\n", player);
        }

        SDL_RenderClear(glo_render);

        /* renders everything chessboard releated */
        for (int n = 0; n < 64; n++) {
            SDL_RenderCopy(glo_render, glo_chess_core_tile[n].texture, NULL, &glo_chess_core_tile[n].rect);
            if (glo_chess_core_tile[n].piece != NULL) SDL_RenderCopy(glo_render, glo_chess_core_tile[n].piece->texture, NULL, &glo_chess_core_tile[n].rect);

            DOT_StateGlobalDot(n);
        }

        SDL_RenderPresent(glo_render);

        if (event.type == SDL_QUIT) break;
    }

    return;
}

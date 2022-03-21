#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"

#include "../global.h"
#include "event.h"
#include "chess.h"
#include "dot.h"
#include "touch.h"
#include "middle.h"
#include "core.h"
#include "fen.h"

CHESS_CORE_PLAYER glo_chess_core_player;
CHESS_CORE_TILE glo_chess_core_tile[64];
CHESS_CORE_PIECE glo_chess_core_piece[32];

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

    for (int n = 0; n < 32; n++) {

        glo_chess_core_piece[n].player = -1;
        glo_chess_core_piece[n].enum_piece = NONE;
        glo_chess_core_piece[n].texture = NULL;

        glo_chess_core_piece[n].rect.x = 0;
        glo_chess_core_piece[n].rect.y = 0;
        glo_chess_core_piece[n].rect.w = 0;
        glo_chess_core_piece[n].rect.h = 0;

        glo_chess_core_piece[n].lock = -1;

    }

    return;
}

void CORE_Testing(CHESS_CORE_PLAYER player) {

    glo_chess_core_player = player;

    CORE_ChessCreateBoard();
    CORE_ChessInitTag(player);

    DOT_InitGlobalDot();

    // set glo_chess_core_piece to initial state
    CORE_ResetGlobal_CorePiece();

    // init pieces for main player
    FEN_Init(glo_chess_core_player, "4k3/6p1/8/8/8/8/3K/8");

    printf("finish FEN notation\n");

    SDL_Event event;
    player = WHITE_PLAYER;

    while(1) {
        SDL_PollEvent(&event);

        EVENT_CheckPieceLayer(player);

        // TODO: fix bug where touching first tile, triggers this function
        if (MIDDLE_UpdateChangeState(&event, player) == 0) { player ^= 1; printf("CORE_Testing:\n  player_turn = %d\n", player); }

        SDL_RenderClear(glo_render);
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

// obsolete
/*
char *chess_initpiece[8] = {
 "        ",
 "      P ",
 "   K    ",
 "        ",
 "        ",
 "    pp  ",
 " k      ",
 "        "
};
*/
/*


int CORE_ReadArrayInitPiece(char *array[], CHESS_CORE_PLAYER player) {

    int result = -1;

    CHESS_CORE_PLAYER pl_bak = player;
    volatile int count_core_piece;

    if (pl_bak == WHITE_PLAYER) {

        int j = 0;

        for (int n = 0; n < 8; n++) {

            for (int i = 0; i < 8; i++) {

                if (array[n][i] != ' ') {
                    switch(array[n][i]) {
                        case 'K':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'k':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'P':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BPAWN, player);
                        count_core_piece++;
                        break;
                        case 'p':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, PAWN, player);
                        count_core_piece++;
                        break;
                        case 'N':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'n':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'B':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'b':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'R':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'r':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'Q':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                        case 'q':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                    }

                    if (result != EXIT_SUCCESS) return (EXIT_FAILURE);
                }
                j += 1;
            }
        }
    }

    else if (pl_bak == BLACK_PLAYER) {

        int j = 63;

        for (int n = 0; n < 8; n++) {
            for (int i = 0; i < 8; i++) {

                if (array[n][i] != ' ') {
                    switch(array[n][i]) {
                        case 'K':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'k':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KING, player);
                        count_core_piece++;
                        break;
                        case 'P':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, PAWN, player);
                        count_core_piece++;
                        break;
                        case 'p':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BPAWN, player);
                        count_core_piece++;
                        break;
                        case 'N':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'n':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, KNIGHT, player);
                        count_core_piece++;
                        break;
                        case 'B':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'b':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, BISHOP, player);
                        count_core_piece++;
                        break;
                        case 'R':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'r':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, ROOK, player);
                        count_core_piece++;
                        break;
                        case 'Q':
                        player = BLACK_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                        case 'q':
                        player = WHITE_PLAYER;
                        result = CORE_InitPiece(&glo_chess_core_piece[count_core_piece], j, QUEEN, player);
                        count_core_piece++;
                        break;
                    }

                    if (result != EXIT_SUCCESS) return (EXIT_FAILURE);
                }
                j -= 1;
            }
        }
    }

    return (EXIT_SUCCESS);
}
*/

CHESS_CORE_ENUM_PIECE CORE_CaseEnumPiece(CHESS_CORE_PLAYER player, char character) {

    switch(character)
    {
        case 'K':
        return KING;
        break;

        case 'k':
        return BKING;
        break;

        case 'P':
        return PAWN;
        break;

        case 'p':
        return BPAWN;
        break;

        case 'N':
        return KNIGHT;
        break;

        case 'n':
        return BKNIGHT;
        break;

        case 'B':
        return BISHOP;
        break;

        case 'b':
        return BBISHOP;
        break;

        case 'R':
        return ROOK;
        break;

        case 'r':
        return BROOK;
        break;

        case 'Q':
        return QUEEN;
        break;

        case 'q':
        return BQUEEN;
        break;

        default:
        return NONE;
        break;
    }
}

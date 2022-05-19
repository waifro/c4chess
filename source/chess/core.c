#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "../dashboard/gui.h"
#include "../dashboard/gui_alias.h"
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
            glo_chess_core_tile[n].texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &glo_chess_core_tile[n].rect, x_b, y_b, size_tile, size_tile);
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

    SDL_Texture *background = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_GREY_DARK, 255, NULL, 0, 0, glo_screen_w, glo_screen_h);

    // TODO: cap framerate to 30/60 fps
    SDL_Event event;

    // texture (tmp fix to make snapshot)
    SDL_Texture *txr_snapshot = NULL;

    int running = 0;
    while(running == 0) {

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = -1;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {

                int result = 0;
                txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);

                while (1) {

                    if (running == -1) break;

                    PP4M_HOOK *hook_list_pw = GUI_PopupWindow_Init(440, 180);

                    GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, -1, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 15, 410, 70);
                    GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, -2, "Esci dal gioco", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 95, 410, 70);

                    result = GUI_PopupWindow_Core(hook_list_pw, txr_snapshot);

                    if (result == -1) break;
                    if (result == -3) { running = -1; break; }

                    SDL_Texture *txr_snapshot2 = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
                    PP4M_HOOK *hook_list_pw_exit = GUI_PopupWindow_Init(400, 165);

                    GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, -1, "Annulla", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 85, 185, 70);
                    GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, -2, "Okay", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 205, 85, 185, 70);

                    if (GUI_PopupWindow_Core(hook_list_pw_exit, txr_snapshot2) == -2) running = -1;

                    hook_list_pw_exit = NULL;

                }
            }
        }

        /* checks if king under attack */
        CHESS_PiecePattern_UpdateState(glo_chess_core_tile, player);

        /* makes the in-game changes during gameplay */
        MIDDLE_UpdateChangeState(&event, &player);

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, background, NULL, NULL);

        /* renders everything chessboard releated */
        for (int n = 0; n < 64; n++) {
            SDL_RenderCopy(glo_render, glo_chess_core_tile[n].texture, NULL, &glo_chess_core_tile[n].rect);
            if (glo_chess_core_tile[n].piece != NULL) SDL_RenderCopy(glo_render, glo_chess_core_tile[n].piece->texture, NULL, &glo_chess_core_tile[n].rect);

            DOT_StateGlobalDot(n);
        }

        SDL_RenderPresent(glo_render);
    }

    SDL_DestroyTexture(background);

    return;
}

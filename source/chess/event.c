#include <stdio.h>

#include "event.h"
#include "core.h"
#include "chess.h"

bool glo_chess_event_layer[64];
bool glo_chess_event_king_uatk;

void EVENT_BlankLayer(void) {
    for (int n = 0; n < 64; n++) {
        glo_chess_event_layer[n] = false;
    }
    return;
}

void EVENT_BlankLayer_Piece(void) {
    for (int n = 0; n < 64; n++) {
        if (glo_chess_core_tile[n].piece != NULL) {
            for (int i = 0; i < 64; i++) glo_chess_core_tile[n].piece->range[i] = false;
        }
    }

    return;
}

void EVENT_CheckDrawState(void) {

    for (int n = 0; n < 32; n++) {
        if (glo_chess_core_piece[n].texture != 0 && glo_chess_core_piece[n].enum_piece == NONE) { printf("  dead mouse = %d\n\n", n); }
    }

    return;
}

void EVENT_CheckKingState(CHESS_CORE_PLAYER player) {

    for (int n = 0; n < 64; n++) {

        if (glo_chess_core_tile[n].piece != NULL && glo_chess_core_tile[n].piece->player == player) {
            if (glo_chess_core_tile[n].piece->enum_piece == KING || glo_chess_core_tile[n].piece->enum_piece == BKING) {

                if (glo_chess_event_layer[n] == true) {
                    printf("\n\nKING UNDER ATTACK\n\n\n"); //check enabled, a func;

                    //if (player == WHITE_PLAYER) glo_chess_event_wking_uatk = true;
                    //else if (player == BLACK_PLAYER) glo_chess_event_bking_uatk = true;
                    glo_chess_event_king_uatk = true;
                    return;
                }

                //if (player == WHITE_PLAYER) glo_chess_event_wking_uatk = false;
                //else if (player == BLACK_PLAYER) glo_chess_event_bking_uatk = false;
                glo_chess_event_king_uatk = false;
                break;
            }
        }
    }

    return;
}

void EVENT_InterposeAttack(void) {

    return;
}

void EVENT_CheckPieceLayer(CHESS_CORE_PLAYER player) {

    static CHESS_CORE_PLAYER pl_bak;

    static bool check_state;
    if (!check_state) {

        if (player == WHITE_PLAYER) pl_bak = BLACK_PLAYER;
        else pl_bak = WHITE_PLAYER;

        check_state = true;
    }

    if (pl_bak != player) {

        EVENT_BlankLayer();
        EVENT_BlankLayer_Piece();

        for (int n = 0; n < 64; n++)
        {
            // piece range copy
            if (glo_chess_core_tile[n].piece != NULL && glo_chess_core_tile[n].piece->player != player) {

                CHESS_RedirectPiecePattern(n, pl_bak, true);

                for (int i = 0; i < 64; i++) {

                    if (glo_chess_core_tile[n].piece->range[i] == true) {
                        glo_chess_event_layer[i] = true;
                    }

                    // better to create a visible layer
                    //printf("EVENT_CheckPieceLayer: piece[%c%d] range[%c%d] = piece[%d] layer[%d]\n", glo_chess_core_tile[n].tag.col, glo_chess_core_tile[n].tag.row, glo_chess_core_tile[i].tag.col, glo_chess_core_tile[i].tag.row, glo_chess_core_tile[n].piece->range[i], glo_chess_event_layer[i]);
                }
            }
        }

        pl_bak = player;
        EVENT_CheckDrawState();
        EVENT_CheckKingState(player);
    }

    return;
}

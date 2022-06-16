#include <stdio.h>

#include "event.h"
#include "core.h"
#include "chess.h"

bool glo_chess_event_layer[64];
bool glo_chess_event_king_uatk;

void EVENT_BlankLayer_Global(void) {
    for (int n = 0; n < 64; n++) {
        glo_chess_event_layer[n] = false;
    }
    return;
}

// instead of searching for eveery tile, search for glo_chess_core_piece[]
void EVENT_BlankLayer_Piece(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {
    for (int n = 0; n < 64; n++) {
        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player) {
            for (int i = 0; i < 64; i++) chess_tile[n].piece->range[i] = false;
        }
    }

    return;
}

void EVENT_UpdateState_ChessEvent(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player) {

    if (CHESS_Redirect_EnumPawn(chess_tile, position_old) == 0) {
        CHESS_CheckState_PawnEnPassant(chess_tile, position_old, position_new, player);
        CHESS_CheckState_PawnPromotion(chess_tile, position_old, position_new, player);
    } else if (CHESS_Redirect_EnumKing(chess_tile, position_old) == 0)
        CHESS_CheckState_KingCastling(chess_tile, position_old, position_new, player);
    else if (CHESS_Redirect_EnumRook(chess_tile, position_old) == 0)
        CHESS_CheckState_RookCastling(chess_tile, position_old, position_new, player);

    if (CHESS_Redirect_EnumPawn(chess_tile, position_old) != 0)
        _glo_chess_tile_passant = -1;

    return;
}

void EVENT_CheckKing_UnderAttack(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    for (int n = 0; n < 64; n++) {

        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player) {
            if (chess_tile[n].piece->enum_piece == KING || chess_tile[n].piece->enum_piece == BKING) {

                if (glo_chess_event_layer[n] == true) {
                    glo_chess_event_king_uatk = true;
                    return;
                }

                glo_chess_event_king_uatk = false;
                return;
            }
        }
    }

    return;
}

int EVENT_CheckPieceLayer(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    static CHESS_CORE_PLAYER pl_bak;

    static bool check_state;
    if (!check_state) {
        pl_bak = CORE_ReversePlayer_State(player);
        check_state = true;
    }

    if (pl_bak != player) {

        EVENT_BlankLayer_Global();

        for (int n = 0; n < 64; n++)
        {
            // piece range copy
            if (chess_tile[n].piece != NULL && chess_tile[n].piece->player != player) {

                for (int i = 0; i < 64; i++) {

                    if (chess_tile[n].piece->range[i] == true) {
                        glo_chess_event_layer[i] = true;
                    }

                    // better to create a visible layer
                    //printf("EVENT_CheckPieceLayer: piece[%c%d] range[%c%d] = piece[%d] layer[%d]\n", chess_tile[n].tag.col, chess_tile[n].tag.row, chess_tile[i].tag.col, chess_tile[i].tag.row, chess_tile[n].piece->range[i], glo_chess_event_layer[i]);
                }
            }
        }

        pl_bak = player;
        EVENT_CheckKing_UnderAttack(chess_tile, player);
    }

    if (glo_chess_event_king_uatk == true) return 2;
    return 0;
}

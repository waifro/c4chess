#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>
#include "core.h"

extern bool glo_chess_event_layer[64];
extern bool glo_chess_event_king_uatk;

void EVENT_BlankLayer_Global(void);
void EVENT_BlankLayer_Piece(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

void EVENT_UpdateState_ChessEvent(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player);
void EVENT_CheckKing_UnderAttack(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

int EVENT_CheckPieceLayer(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

#endif

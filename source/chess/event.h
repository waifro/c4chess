#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>
#include "core.h"

extern bool glo_chess_event_layer[64];
extern bool glo_chess_event_layer_[64];

void EVENT_BlankLayer_Global(void);
void EVENT_BlankLayer_Piece(CHESS_CORE_TILE *chess_tile);

void EVENT_CheckDrawState(void);
void EVENT_CheckKingState(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

int EVENT_CheckPieceLayer(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

#endif

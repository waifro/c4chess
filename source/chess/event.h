#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>
#include "core.h"

extern bool glo_chess_event_layer[64];

void EVENT_BlankLayer(void);
void EVENT_BlankPieceLayer(void);

void EVENT_CheckDrawState(void);
void EVENT_CheckKingState(CHESS_CORE_PLAYER player);

void EVENT_CheckPieceLayer(CHESS_CORE_PLAYER player);

#endif

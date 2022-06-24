#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>

#include "core.h"
#include "../pp4m/pp4m_input.h"

extern int glo_chess_event_availmo;

extern bool glo_chess_event_layer[64];
extern bool glo_chess_event_king_uatk;

extern PP4M_HOOK *glo_chess_event_hooklist;

void EVENT_BlankLayer_Global(void);
void EVENT_BlankLayer_Piece(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

void EVENT_UpdateState_ChessEvent(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player);
void EVENT_CheckKing_UnderAttack(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

int EVENT_CheckPieceLayer(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

int EVENT_HandlePopup_Pause(int *running);

int EVENT_HandleKingState(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player);

int EVENT_HandleKeyboard(SDL_Event *event, char *dest);

int EVENT_HandlePopup_Stalemate(char *comment);
int EVENT_HandlePopup_Checkmate(char *comment, CHESS_CORE_PLAYER player);

int EVENT_HookList_Render(void);
int EVENT_HookList_Update(PP4M_INPUT_POS input);
PP4M_HOOK *EVENT_HookList_Init(void);

#endif

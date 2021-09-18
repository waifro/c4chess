#ifndef MIDDLE_H
#define MIDDLE_H

#include <stdio.h>
#include <stdbool.h>

#include "core.h"
#include "touch.h"

int MIDDLE_TouchToTile(TOUCH_POS touch_pos);

int MIDDLE_TagToTile(CHESS_CORE_TILE_TAG tag);
CHESS_CORE_TILE_TAG MIDDLE_TileToTag(int tile);

int MIDDLE_ReturnRowTile(int tile);
int MIDDLE_ReturnColTile(int tile);

void MIDDLE_UpdatePositionPiece(int old, int new);
int MIDDLE_UpdateChangeState(SDL_Event *event, CHESS_CORE_PLAYER *player);

#endif

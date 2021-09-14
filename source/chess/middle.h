#ifndef MIDDLE_H
#define MIDDLE_H

#include <stdio.h>
#include "touch.h"

int MIDDLE_TouchToTile(TOUCH_POS foo);

int MIDDLE_TagToTile(char *tag);
void MIDDLE_TileToTag(int tile, char tag[2]);

void MIDDLE_UpdatePositionPiece(int old, int new);

#endif

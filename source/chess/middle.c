#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "../global.h"
#include "core.h"
#include "touch.h"
#include "middle.h"

int MIDDLE_TouchToTile(TOUCH_POS foo) {

    int result = -1;
    for (int n = 0; n < 64; n++) {

        if (foo.x >= glo_chess_core_tile[n].rect.x && foo.x <= (glo_chess_core_tile[n].rect.x + glo_chess_core_tile[n].rect.w)
        && foo.y >= glo_chess_core_tile[n].rect.y && foo.y <= (glo_chess_core_tile[n].rect.y + glo_chess_core_tile[n].rect.h)) {
            printf("MIDDLE_TouchToTile:\n  tile = [%d]\n  piece = %p\n", n, glo_chess_core_tile[n].piece); result = n; break;
        }

    }

    return (result);
}

int MIDDLE_TagToTile(char *tag) {
    int tile = -1;

    for (int n = 0; n < 64; n++) {
        if (strcmp(tag, glo_chess_core_tile[n].tag) == 0) { tile = n; break; }
    }

    // what happends if nothing compares?
    return (tile);
}

void MIDDLE_TileToTag(int tile, char tag[2]) {

    sprintf(tag, "%s", glo_chess_core_tile[tile].tag);

    return;
}

void MIDDLE_UpdatePositionPiece(int old, int new) {

    printf("MIDDLE_UpdatePosPiece:\n  glo_chess_core_tile[old] = %p\n  glo_chess_core_tile[new] = %p\n", glo_chess_core_tile[old].piece, glo_chess_core_tile[new].piece);

    if (glo_chess_core_tile[old].piece == NULL) return;

    if (old == new) return;

    if ((old < 0 || old > 63) || (new < 0 || old > 63)) return;

    glo_chess_core_tile[new].piece = glo_chess_core_tile[old].piece;
    glo_chess_core_tile[old].piece = NULL;

    glo_chess_core_tile[new].piece->rect = glo_chess_core_tile[new].rect;

    return;
}

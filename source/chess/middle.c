#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "../global.h"
#include "core.h"
#include "touch.h"
#include "middle.h"

int MIDDLE_TouchToTile(CHESS_CORE_PLAYER player, TOUCH_POS foo) {

    int result = -1;

    for (int n = 0; n < 64; n++) {

        if (foo.x >= glo_chess_core_tile[n].rect.x && foo.x <= (glo_chess_core_tile[n].rect.x + glo_chess_core_tile[n].rect.w)
        && foo.y >= glo_chess_core_tile[n].rect.y && foo.y <= (glo_chess_core_tile[n].rect.y + glo_chess_core_tile[n].rect.h)) {

            printf("MIDDLE_TouchToTile:\n  piece = %p\n tile[%d] tag[%s]", glo_chess_core_tile[n].piece, n, glo_chess_core_tile[n].tag);
            if (glo_chess_core_tile[n].piece != NULL) printf(" name[%d] player[%d]\n", glo_chess_core_tile[n].piece->enum_piece, glo_chess_core_tile[n].piece->player);
            else printf("\n");

            result = n; break;
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

void MIDDLE_UpdatePositionPiece(CHESS_CORE_PLAYER player, int old, int new) {

    if (old == new) return;
    if ((old < 0 || old > 63) || (new < 0 || old > 63)) return;
    if (glo_chess_core_tile[old].piece == NULL) return;

    printf("MIDDLE_UpdatePosPiece:\n  glo_chess_core_tile[old] = %p\n  glo_chess_core_tile[new] = %p\n", glo_chess_core_tile[old].piece, glo_chess_core_tile[new].piece);

    if (glo_chess_core_tile[new].piece != NULL) CORE_GlobalDestroyPiece(glo_chess_core_tile[new].piece);

    glo_chess_core_tile[new].piece = glo_chess_core_tile[old].piece;
    glo_chess_core_tile[new].piece->rect = glo_chess_core_tile[new].rect;
    glo_chess_core_tile[old].piece = NULL;

    return;
}

int MIDDLE_UpdateChangeState(SDL_Event *event, CHESS_CORE_PLAYER *player) {

    int result = -1;
    static int position_old = -1;
    static int position_new = -1;
    TOUCH_POS touch_pos;

    touch_pos = TOUCH_MouseState(event);

    if (touch_pos.iner != -1 && position_old == -1) {
        result = MIDDLE_TouchToTile((*player), touch_pos);
        if (result != -1 && glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player == (*player)) position_old = result;
    }

    else if (touch_pos.iner != -1 && position_old != -1) {
        result = MIDDLE_TouchToTile((*player), touch_pos);

        if (result != -1) {

            if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[position_old].piece->player == glo_chess_core_tile[result].piece->player) {
                position_old = -1;
            } else {

                position_new = result;
                MIDDLE_UpdatePositionPiece((*player), position_old, position_new);
                position_new = -1; position_old = -1; result = 0;

            }
        }
    }

    return (result);
}

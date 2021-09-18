#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../global.h"
#include "chess.h"
#include "dot.h"
#include "core.h"
#include "touch.h"
#include "middle.h"

int MIDDLE_TouchToTile(TOUCH_POS touch_pos) {

    int result = -1;

    for (int n = 0; n < 64; n++) {

        if (touch_pos.x >= glo_chess_core_tile[n].rect.x && touch_pos.x <= (glo_chess_core_tile[n].rect.x + glo_chess_core_tile[n].rect.w)
        && touch_pos.y >= glo_chess_core_tile[n].rect.y && touch_pos.y <= (glo_chess_core_tile[n].rect.y + glo_chess_core_tile[n].rect.h)) {

            printf("MIDDLE_TouchToTile:\n  piece = %p\n  tile[%d] tag[%c%d]", glo_chess_core_tile[n].piece, n, glo_chess_core_tile[n].tag.col, glo_chess_core_tile[n].tag.row);
            if (glo_chess_core_tile[n].piece != NULL) printf(" name[%d] player[%d]\n", glo_chess_core_tile[n].piece->enum_piece, glo_chess_core_tile[n].piece->player);
            else printf("\n");

            result = n; break;
        }

    }

    return (result);
}

int MIDDLE_TagToTile(CHESS_CORE_TILE_TAG tag) {
    int tile = -1;
    for (int n = 0; n < 64; n++) {
        if (memcmp(&tag, &glo_chess_core_tile[n].tag, sizeof(tag)) == 0) { tile = n; break; }
    }

    return (tile);
}

CHESS_CORE_TILE_TAG MIDDLE_TileToTag(int tile) {
    CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
    return (tag);
}

int MIDDLE_ReturnRowTile(int tile) {
    int result;
    result = glo_chess_core_tile[tile].tag.row;
    return (result);
}

int MIDDLE_ReturnColTile(int tile) {

    int col_pos;
    char alpha[] = "abcdefgh";

    //result = glo_chess_core_tile[tile].tag.col;

    for (int n = 0; n < 8; n++) {
        if (alpha[n] == glo_chess_core_tile[tile].tag.col) { col_pos = n; break; }
    }

    return (col_pos);
}

void MIDDLE_UpdatePositionPiece(int old, int new) {

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
        result = MIDDLE_TouchToTile(touch_pos);
        if (result != -1 && glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player == (*player)) {

            position_old = result;
            CHESS_RedirectPiecePattern(result, (*player));

        }
    }

    else if (touch_pos.iner != -1 && position_old != -1) {
        result = MIDDLE_TouchToTile(touch_pos);

        if (result != -1) {

            if (glo_chess_dot[result].state == true) {

                // if is a valid move, start changing piece state
                position_new = result;
                MIDDLE_UpdatePositionPiece(position_old, position_new);
                DOT_StateGlobalDotReset();

                position_new = -1; position_old = -1; result = 0;


            } else if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[position_old].piece->player == glo_chess_core_tile[result].piece->player) {

                DOT_StateGlobalDotReset();
                position_old = result;
                CHESS_RedirectPiecePattern(result, (*player));

            } else if ((glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[position_old].piece->player != glo_chess_core_tile[result].piece->player) || glo_chess_core_tile[result].piece == NULL) {

                DOT_StateGlobalDotReset();
                position_old = -1;

            }

        }
    }

    return (result);
}

#include <stdbool.h>

#include "event.h"
#include "chess.h"
#include "middle.h"
#include "core.h"
#include "dot.h"

void CHESS_RedirectPiecePattern(int tile, CHESS_CORE_PLAYER player, bool check) {

    switch(glo_chess_core_tile[tile].piece->enum_piece) {
        case KING: CHESS_PiecePattern_King(tile, player, check);
        break;
        case BKING: CHESS_PiecePattern_King(tile, player, check);
        break;
        case PAWN: CHESS_PiecePattern_Pawn(tile, player, check);
        break;
        case BPAWN: CHESS_PiecePattern_BPawn(tile, player, check);
        break;
        case KNIGHT: CHESS_PiecePattern_Knight(tile, player, check);
        break;
        case BKNIGHT: CHESS_PiecePattern_Knight(tile, player, check);
        break;
        case BISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case BBISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case ROOK: CHESS_PiecePattern_Rook(tile, player, check);
        break;
        case BROOK: CHESS_PiecePattern_Rook(tile, player, check);
        break;
        case QUEEN: CHESS_PiecePattern_Queen(tile, player, check);
        break;
        case BQUEEN: CHESS_PiecePattern_Queen(tile, player, check);
        break;
        default:
        break;
    }

    return;
}

void CHESS_PiecePattern_PawnAttack(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    int row = MIDDLE_ReturnRowTile(tile) + 1;
    int col_pos = MIDDLE_ReturnColTile(tile) - 1;

    // if piece is already at (ex.) e8, exit
    if (row > 7) return;

    CHESS_CORE_TILE_TAG tag;
    tag.row = row;
    tag.col = alpha[col_pos];

    int result = -1;
    for (int n = 0; n < 3; n++) {

        if (col_pos < 0) { col_pos += 1; tag.col = alpha[col_pos]; continue; }
        else if (col_pos > 7) { break; }

        if (n == 1) { col_pos += 1; tag.col = alpha[col_pos]; continue; }

        result = MIDDLE_TagToTile(tag);

        if (check == true) {
            glo_chess_core_tile[tile].piece->range[result] = true;
        } else {
            if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                glo_chess_dot[result].state = true;
            }
        }

        col_pos += 1; tag.col = alpha[col_pos];
    }

    return;
}

void CHESS_PiecePattern_BPawnAttack(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    int row = MIDDLE_ReturnRowTile(tile) - 1;
    int col_pos = MIDDLE_ReturnColTile(tile) - 1;

    // if piece is already at (ex.) e1, exit
    if (row < 0) return;

    CHESS_CORE_TILE_TAG tag;
    tag.row = row;
    tag.col = alpha[col_pos];

    int result = -1;
    for (int n = 0; n < 3; n++) {

        if (col_pos < 0) { col_pos += 1; tag.col = alpha[col_pos]; continue; }
        else if (col_pos > 7) { break; }

        if (n == 1) { col_pos += 1; tag.col = alpha[col_pos]; continue; }

        result = MIDDLE_TagToTile(tag);

        if (check == true) {
            glo_chess_core_tile[tile].piece->range[result] = true;
        } else {
            if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                glo_chess_dot[result].state = true;
            }
        }

        col_pos += 1; tag.col = alpha[col_pos];
    }

    return;
}

void CHESS_PiecePattern_King(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    CHESS_CORE_TILE_TAG tag;

    int col_pos = MIDDLE_ReturnColTile(tile) - 1;
    tag.row = MIDDLE_ReturnRowTile(tile) - 1;
    tag.col = alpha[col_pos];

    int result = -1;

    for (int n = 0; n < 9; n++) {

        if (n == 4) { tag.row += 1; continue; }
        if (n == 3 || n == 6) { col_pos += 1; tag.row -= 3; }

        if (col_pos < 0) { tag.row += 1; continue; }
        if (col_pos > 7) { tag.row += 1; continue; }
        if (tag.row < 1) { tag.row += 1; continue; }
        if (tag.row > 8) { tag.row += 1; continue; }

        tag.col = alpha[col_pos];
        result = MIDDLE_TagToTile(tag);

        if (check == true) {
            if (glo_chess_core_tile[result].piece != NULL) {
                if (glo_chess_event_layer[result] == false) {
                    glo_chess_core_tile[tile].piece->range[result] = true;
                }
            }

            else if (glo_chess_core_tile[result].piece == NULL) {
                if (glo_chess_event_layer[result] == false) glo_chess_core_tile[tile].piece->range[result] = true;
            }
        }

        else if (check == false) {
            if (glo_chess_core_tile[result].piece != NULL) {
                if (glo_chess_core_tile[result].piece->player != player) {

                    if (glo_chess_event_layer[result] == false) {
                        glo_chess_dot[result].state = true;
                    }
                }
            }

            else if (glo_chess_core_tile[result].piece == NULL) {
                if (glo_chess_event_layer[result] == false) glo_chess_dot[result].state = true;
            }
        }

        tag.row += 1;
    }

    return;
}

void CHESS_PiecePattern_Pawn(int tile, CHESS_CORE_PLAYER player, bool check) {

    if (check == false) {

        CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
        int result = -1;

        if (tag.row == 8) return;

        if (tag.row == 2) {

            tag.row += 1;
            if (tag.row == 8) return;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) {
                glo_chess_dot[result].state = true;

                tag.row += 1;
                if (tag.row == 8) return;
                result = MIDDLE_TagToTile(tag);

                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
            }
        } else {

            tag.row += 1;
            if (tag.row == 8) return;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;

        }

    }

    CHESS_PiecePattern_PawnAttack(tile, player, check);

    return;
}

void CHESS_PiecePattern_BPawn(int tile, CHESS_CORE_PLAYER player, bool check) {

    if (check == false) {

        CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
        int result = -1;

        if (tag.row == 1) return;

        if (tag.row == 7) {

            tag.row -= 1;
            if (tag.row == 1) return;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) {
                glo_chess_dot[result].state = true;

                tag.row -= 1;
                if (tag.row == 1) return;
                result = MIDDLE_TagToTile(tag);

                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
            }
        } else {

            tag.row -= 1;
            if (tag.row == 1) return;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;

        }

    }

    CHESS_PiecePattern_BPawnAttack(tile, player, check);

    return;
}

void CHESS_PiecePattern_Knight(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    CHESS_CORE_TILE_TAG tag;

    int col_pos = MIDDLE_ReturnColTile(tile) - 2;
    tag.row = MIDDLE_ReturnRowTile(tile) - 2;
    tag.col = alpha[col_pos];

    int result = -1;

    // might want to change this function
    for (int i = 0; i < 4; i++) {

        col_pos += 1;
        tag.col = alpha[col_pos];
        result = MIDDLE_TagToTile(tag);

        if (result == -1) continue;

        // temporary fix to standardize locking piece through lock variable(?)
        if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

        if (i == 0 || i == 2)
        {
            if (check == true)
            {
                glo_chess_core_tile[tile].piece->range[result] = true;
            }

            else {
                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
            }
        }
    }

    for (int i = 0; i < 4; i++) {

        tag.row += 1;
        result = MIDDLE_TagToTile(tag);

        if (result == -1) continue;

        // temporary fix to standardize locking piece through lock variable(?)
        if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

        if (i == 0 || i == 2)
        {
            if (check == true)
            {
                glo_chess_core_tile[tile].piece->range[result] = true;
            }

            else {
                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
            }
        }
    }

    for (int i = 0; i < 4; i++) {

        col_pos -= 1;
        tag.col = alpha[col_pos];
        result = MIDDLE_TagToTile(tag);

        if (result == -1) continue;

        // temporary fix to standardize locking piece through lock variable(?)
        if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

        if (i == 0 || i == 2)
        {
            if (check == true)
            {
                glo_chess_core_tile[tile].piece->range[result] = true;
            }

            else {
                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
            }
        }
    }

    for (int i = 0; i < 4; i++) {

        tag.row -= 1;
        result = MIDDLE_TagToTile(tag);

        if (result == -1) continue;

        // temporary fix to standardize locking piece through lock variable(?)
        if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

        if (i == 0 || i == 2)
        {
            if (check == true)
            {
                glo_chess_core_tile[tile].piece->range[result] = true;
            }

            else {
                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
            }
        }
    }

    return;
}

void CHESS_PiecePattern_Bishop(int tile, CHESS_CORE_PLAYER player, bool check) {

    CHESS_CORE_TILE_TAG tag;

    // save current state on col_pos
    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile);

    int result = -1;
    for (int n = 0; n < 4; n++)
    {
        col_pos = MIDDLE_ReturnColTile(tile);
        tag.row = MIDDLE_ReturnRowTile(tile);
        tag.col = alpha[col_pos];

        for (int i = 0; i < 8; i++)
        {
            if      (n == 0) { col_pos -= 1; tag.row += 1; }
            else if (n == 1) { col_pos += 1; tag.row += 1; }
            else if (n == 2) { col_pos -= 1; tag.row -= 1; }
            else if (n == 3) { col_pos += 1; tag.row -= 1; }
            tag.col = alpha[col_pos];

            result = MIDDLE_TagToTile(tag);

            if (result == -1) continue;

            // temporary fix to standardize locking piece through lock variable(?)
            if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

            if (check == true) {
                glo_chess_core_tile[tile].piece->range[result] = true;

                if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                    if (glo_chess_core_tile[result].piece->enum_piece != KING && glo_chess_core_tile[result].piece->enum_piece != BKING) break;
                }
            }

            else {
                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                else if (glo_chess_core_tile[result].piece != NULL)
                {
                    if (glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                    break;
                }
            }
        }
    }

    return;
}

void CHESS_PiecePattern_Rook(int tile, CHESS_CORE_PLAYER player, bool check) {

    CHESS_CORE_TILE_TAG tag;

    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile);

    int result = -1;
    for (int n = 0; n < 4; n++)
    {
        col_pos = MIDDLE_ReturnColTile(tile);
        tag.row = MIDDLE_ReturnRowTile(tile);
        tag.col = alpha[col_pos];

        for (int i = 0; i < 8; i++)
        {
            if      (n == 0) { tag.row += 1; }
            else if (n == 1) { col_pos += 1; }
            else if (n == 2) { tag.row -= 1; }
            else if (n == 3) { col_pos -= 1; }
            tag.col = alpha[col_pos];

            result = MIDDLE_TagToTile(tag);

            if (result == -1) continue;

            // temporary fix to standardize locking piece through lock variable(?)
            if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

            if (check == true) {
                glo_chess_core_tile[tile].piece->range[result] = true;

                if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                    if (glo_chess_core_tile[result].piece->enum_piece != KING && glo_chess_core_tile[result].piece->enum_piece != BKING) break;
                }
            }

            else {
                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                else if (glo_chess_core_tile[result].piece != NULL)
                {
                    if (glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                    break;
                }
            }
        }
    }


    return;
}

void CHESS_PiecePattern_Queen(int tile, CHESS_CORE_PLAYER player, bool check) {

    CHESS_CORE_TILE_TAG tag;

    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile);

    int result = -1;
    for (int n = 0; n < 2; n++)
    {
        for (int i = 0; i < 4; i++)
        {
            col_pos = MIDDLE_ReturnColTile(tile);
            tag.row = MIDDLE_ReturnRowTile(tile);
            tag.col = alpha[col_pos];

            for (int u = 0; u < 8; u++)
            {
                if (n == 0)
                {
                    if      (i == 0) { tag.row += 1; }
                    else if (i == 1) { col_pos += 1; }
                    else if (i == 2) { tag.row -= 1; }
                    else if (i == 3) { col_pos -= 1; }
                }

                else if (n == 1)
                {
                    if      (i == 0) { col_pos -= 1; tag.row += 1; }
                    else if (i == 1) { col_pos += 1; tag.row += 1; }
                    else if (i == 2) { col_pos -= 1; tag.row -= 1; }
                    else if (i == 3) { col_pos += 1; tag.row -= 1; }
                }

                tag.col = alpha[col_pos];

                result = MIDDLE_TagToTile(tag);

                if (result == -1) continue;

                // temporary fix to standardize locking piece through lock variable(?)
                if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

                if (check == true) {
                    glo_chess_core_tile[tile].piece->range[result] = true;

                    if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                        if (glo_chess_core_tile[result].piece->enum_piece != KING && glo_chess_core_tile[result].piece->enum_piece != BKING) break;
                    }
                }

                else {
                    if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                    else if (glo_chess_core_tile[result].piece != NULL)
                    {
                        if (glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                        break;
                    }
                }
            }
        }
    }

    return;
}

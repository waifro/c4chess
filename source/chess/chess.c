#include <stdbool.h>

#include "event.h"
#include "chess.h"
#include "middle.h"
#include "core.h"
#include "dot.h"

void CHESS_RedirectPiecePattern(int tile, CHESS_CORE_PLAYER player, bool check) {

    if (glo_chess_core_player == WHITE_PLAYER) {
        switch(glo_chess_core_tile[tile].piece->enum_piece) {
            case KING: CHESS_PiecePatternKing(tile, player, check);
            break;
            case PAWN: CHESS_PiecePatternPawn(tile, player, check);
            break;
            case BPAWN: CHESS_PiecePatternBPawn(tile, player, check);
            break;
            case KNIGHT: //CHESS_PiecePatternKnight(tile, player, check);
            break;
            case BISHOP: //CHESS_PiecePatternBishop(tile, player, check);
            break;
            case ROOK: //CHESS_PiecePatternRook(tile, player, check);
            break;
            case QUEEN: //CHESS_PiecePatternQueen(tile, player, check);
            break;
        }
    }

    else if (glo_chess_core_player == BLACK_PLAYER){
        switch(glo_chess_core_tile[tile].piece->enum_piece) {
            case KING: CHESS_PiecePatternKing(tile, player, check);
            break;
            case PAWN: CHESS_PiecePatternBPawn(tile, player, check);
            break;
            case BPAWN: CHESS_PiecePatternPawn(tile, player, check);
            break;
            case KNIGHT: //CHESS_PiecePatternKnight(tile, player, check);
            break;
            case BISHOP: //CHESS_PiecePatternBishop(tile, player, check);
            break;
            case ROOK: //CHESS_PiecePatternRook(tile, player, check);
            break;
            case QUEEN: //CHESS_PiecePatternQueen(tile, player, check);
            break;
        }
    }

    return;
}

void CHESS_PiecePatternPawnAttack(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    int row = MIDDLE_ReturnRowTile(tile) + 1;
    int col_pos = MIDDLE_ReturnColTile(tile) - 1;

    // if piece is already at (ex.) e8, exit
    if (row > 7) return;

    CHESS_CORE_TILE_TAG tag;
    tag.row = row;
    tag.col = alpha[col_pos];

    int result;
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

void CHESS_PiecePatternBPawnAttack(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    int row = MIDDLE_ReturnRowTile(tile) - 1;
    int col_pos = MIDDLE_ReturnColTile(tile) - 1;

    // if piece is already at (ex.) e1, exit
    if (row < 0) return;

    CHESS_CORE_TILE_TAG tag;
    tag.row = row;
    tag.col = alpha[col_pos];

    int result;
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

void CHESS_PiecePatternKing(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    CHESS_CORE_TILE_TAG tag;

    int col_pos = MIDDLE_ReturnColTile(tile) - 1;
    tag.row = MIDDLE_ReturnRowTile(tile) - 1;
    tag.col = alpha[col_pos];

    int result;

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

void CHESS_PiecePatternPawn(int tile, CHESS_CORE_PLAYER player, bool check) {

    if (check == false) {

        CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
        int buffer;

        if (tag.row == 8) return;

        if (tag.row == 2) {

            tag.row += 1;
            if (tag.row == 8) return;
            buffer = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[buffer].piece == NULL) {
                glo_chess_dot[buffer].state = true;

                tag.row += 1;
                if (tag.row == 8) return;
                buffer = MIDDLE_TagToTile(tag);

                if (glo_chess_core_tile[buffer].piece == NULL) glo_chess_dot[buffer].state = true;
            }
        } else {

            tag.row += 1;
            if (tag.row == 8) return;
            buffer = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[buffer].piece == NULL) glo_chess_dot[buffer].state = true;

        }

    }

    CHESS_PiecePatternPawnAttack(tile, player, check);

    return;
}

void CHESS_PiecePatternBPawn(int tile, CHESS_CORE_PLAYER player, bool check) {

    if (check == false) {

        CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
        int buffer;

        if (tag.row == 1) return;

        if (tag.row == 7) {

            tag.row -= 1;
            if (tag.row == 1) return;
            buffer = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[buffer].piece == NULL) {
                glo_chess_dot[buffer].state = true;

                tag.row -= 1;
                if (tag.row == 1) return;
                buffer = MIDDLE_TagToTile(tag);

                if (glo_chess_core_tile[buffer].piece == NULL) glo_chess_dot[buffer].state = true;
            }
        } else {

            tag.row -= 1;
            if (tag.row == 1) return;
            buffer = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[buffer].piece == NULL) glo_chess_dot[buffer].state = true;

        }

    }

    CHESS_PiecePatternBPawnAttack(tile, player, check);

    return;
}

void CHESS_PiecePatternKnight(int tile, CHESS_CORE_PLAYER player, bool check) {

    char alpha[] = "abcdefgh";
    CHESS_CORE_TILE_TAG tag;

    int col_pos = MIDDLE_ReturnColTile(tile) - 2;
    tag.row = MIDDLE_ReturnRowTile(tile);
    tag.col = alpha[col_pos];

    int result;
    for (int n = 0; n < 5; n++) {

        result = MIDDLE_TagToTile(tag);
        if (result == -1) {continue;}

        if (n == 2) { continue; }

        for (int i = 0; i < 2; i++) {

            if (n == 1 || n == 3) {

                col_pos += 1;
                tag.col = alpha[col_pos];

                result = MIDDLE_TagToTile(tag);
                if (result == -1) {continue;}
            }

        }
    }

    return;
}

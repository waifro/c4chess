#include "chess.h"
#include "middle.h"
#include "core.h"
#include "dot.h"

void CHESS_RedirectPiecePattern(int tile, CHESS_CORE_PLAYER player) {

    switch(glo_chess_core_tile[tile].piece->enum_piece) {
        case KING: //CHESS_PiecePatternKing(tile, player);
        break;
        case PAWN: CHESS_PiecePatternPawn(tile, player);
        break;
        case BPAWN: CHESS_PiecePatternBPawn(tile, player);
        break;
        case KNIGHT: //CHESS_PiecePatternKnight(tile, player);
        break;
        case BISHOP: //CHESS_PiecePatternBishop(tile, player);
        break;
        case ROOK: //CHESS_PiecePatternRook(tile, player);
        break;
        case QUEEN: //CHESS_PiecePatternQueen(tile, player);
        break;
    }

    return;
}

void CHESS_PiecePatternPawnAttack(int tile, CHESS_CORE_PLAYER player) {

    char alpha[] = "abcdefgh";
    int row = MIDDLE_ReturnRowTile(tile) + 1;
    int col_pos = MIDDLE_ReturnColTile(tile) - 1;

    // if piece is already at (ex.) e8, exit
    if (row > 8) return;

    CHESS_CORE_TILE_TAG tag;
    tag.row = row;
    tag.col = alpha[col_pos];

    int result;
    for (int n = 0; n < 3; n++) {

        if (col_pos < 0) { col_pos += 1; tag.col = alpha[col_pos]; continue; }
        else if (col_pos > 7) { break; }

        if (n == 1) { col_pos += 1; tag.col = alpha[col_pos]; continue; }

        result = MIDDLE_TagToTile(tag);

        if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;

        col_pos += 1; tag.col = alpha[col_pos];
    }

    return;
}

void CHESS_PiecePatternBPawnAttack(int tile, CHESS_CORE_PLAYER player) {

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

        if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;

        col_pos += 1; tag.col = alpha[col_pos];
    }

    return;
}

void CHESS_PiecePatternPawn(int tile, CHESS_CORE_PLAYER player) {

    if (glo_chess_core_tile[tile].tag.row == 8) return;

    if (glo_chess_core_tile[tile].tag.row == 2) {

        if (glo_chess_core_tile[tile-8].piece == NULL) glo_chess_dot[tile-8].state = true;
        if (glo_chess_core_tile[tile-16].piece == NULL) glo_chess_dot[tile-16].state = true;

    }

    else {
        if (glo_chess_core_tile[tile-8].piece == NULL) glo_chess_dot[tile-8].state = true;
    }

    CHESS_PiecePatternPawnAttack(tile, player);

    return;
}

void CHESS_PiecePatternBPawn(int tile, CHESS_CORE_PLAYER player) {

    if (glo_chess_core_tile[tile].tag.row == 1) return;

    if (glo_chess_core_tile[tile].tag.row == 7) {

        if (glo_chess_core_tile[tile+8].piece == NULL) glo_chess_dot[tile+8].state = true;
        if (glo_chess_core_tile[tile+16].piece == NULL) glo_chess_dot[tile+16].state = true;

    }

    else {
        if (glo_chess_core_tile[tile+8].piece == NULL) glo_chess_dot[tile+8].state = true;
    }

    CHESS_PiecePatternBPawnAttack(tile, player);

    return;
}
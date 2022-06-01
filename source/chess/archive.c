#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "chess.h"
#include "archive.h"

char *glo_chess_archive_flow = NULL;

void ARCHIVE_Notation_EnrollMove(CHESS_CORE_TILE *chess_tile, int position_old, int position_new) {

    if (!glo_chess_archive_flow)
        glo_chess_archive_flow = malloc(sizeof(char*));

    ARCHIVE_NOTATION_PIECE piece;
    piece = ARCHIVE_Redirect_StateMove(chess_tile, position_new);

    if (piece == NONE_A) ;
    if (piece == KING_A) ;
    if (piece == PAWN_A) ;
    if (piece == BISHOP_A) ;
    if (piece == KNIGHT_A) ;
    if (piece == ROOK_A) ;
    if (piece == QUEEN_A) ;

    return;
}


ARCHIVE_NOTATION_PIECE ARCHIVE_Redirect_StateMove(CHESS_CORE_TILE *chess_tile, int slot) {

    if (CHESS_Redirect_EnumKing(chess_tile, slot) == 0) return KING_A;
    else if (CHESS_Redirect_EnumPawn(chess_tile, slot) == 0) return PAWN_A;
    else if (CHESS_Redirect_EnumBishop(chess_tile, slot) == 0) return BISHOP_A;
    else if (CHESS_Redirect_EnumKnight(chess_tile, slot) == 0) return KNIGHT_A;
    else if (CHESS_Redirect_EnumRook(chess_tile, slot) == 0) return ROOK_A;
    else if (CHESS_Redirect_EnumQueen(chess_tile, slot) == 0) return QUEEN_A;
    else return NONE_A;
}

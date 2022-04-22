#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>

#include "core.h"

typedef enum {
    ATTACK = 0,
    CHECK = 1,
    CHECK_KING = 2
} CHESS_PIECE_ATK;

int CHESS_RedirectPiecePattern(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);

int CHESS_PiecePattern_King(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);
int CHESS_PiecePattern_Pawn(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);
int CHESS_PiecePattern_BPawn(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);
int CHESS_PiecePattern_Knight(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);
int CHESS_PiecePattern_Bishop(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);
int CHESS_PiecePattern_Rook(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);
int CHESS_PiecePattern_Queen(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);

// pawns have special attacks, separated moves
int CHESS_PiecePattern_PawnAttack(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);
int CHESS_PiecePattern_BPawnAttack(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);

#endif

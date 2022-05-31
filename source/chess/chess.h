#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>

#include "core.h"

typedef enum {
    ATTACK = 0,
    CHECK = 1,
    CHECK_KING = 2
} CHESS_PIECE_ATK;

int CHESS_PiecePattern_UpdateState(CHESS_CORE_TILE *core_tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_RangeReset(CHESS_CORE_TILE *core_tile, int tile);
int CHESS_PiecePattern_RangeAllowed(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_RedirectPiecePattern(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);

int CHESS_PiecePattern_King(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Pawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_BPawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Knight(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Bishop(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Rook(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Queen(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check);

// pawns have special attacks, separated moves
int CHESS_PiecePattern_PawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_BPawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);

#endif

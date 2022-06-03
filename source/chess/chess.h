#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>

#include "core.h"

int CHESS_PiecePattern_UpdateState(CHESS_CORE_TILE *core_tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_RangeReset(CHESS_CORE_TILE *core_tile, int tile);
int CHESS_PiecePattern_RangeAllowed(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_Redirect_PiecePattern(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);

int CHESS_Redirect_EnumKing(CHESS_CORE_TILE *chess_tile, int slot);
int CHESS_Redirect_EnumPawn(CHESS_CORE_TILE *chess_tile, int slot);
int CHESS_Redirect_EnumBishop(CHESS_CORE_TILE *chess_tile, int slot);
int CHESS_Redirect_EnumKnight(CHESS_CORE_TILE *chess_tile, int slot);
int CHESS_Redirect_EnumRook(CHESS_CORE_TILE *chess_tile, int slot);
int CHESS_Redirect_EnumQueen(CHESS_CORE_TILE *chess_tile, int slot);

int CHESS_PiecePattern_King(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Pawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_BPawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Knight(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Bishop(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Rook(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player);
int CHESS_PiecePattern_Queen(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player);

// pawns have special attacks, separated moves
int CHESS_PiecePattern_PawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, int king_check);
int CHESS_PiecePattern_BPawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, int king_check);

#endif

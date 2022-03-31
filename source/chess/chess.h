#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>

#include "core.h"

void CHESS_RedirectPiecePattern(int tile, CHESS_CORE_PLAYER player, bool check);

void CHESS_PiecePattern_King(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePattern_Pawn(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePattern_BPawn(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePattern_Knight(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePattern_Bishop(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePattern_Rook(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePattern_Queen(int tile, CHESS_CORE_PLAYER player, bool check);

// pawns have special attacks, separated moves
void CHESS_PiecePattern_PawnAttack(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePattern_BPawnAttack(int tile, CHESS_CORE_PLAYER player, bool check);

#endif

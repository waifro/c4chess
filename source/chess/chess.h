#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>

#include "core.h"

void CHESS_RedirectPiecePattern(int tile, CHESS_CORE_PLAYER player, bool check);

void CHESS_PiecePatternPawnAttack(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePatternBPawnAttack(int tile, CHESS_CORE_PLAYER player, bool check);

void CHESS_PiecePatternKing(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePatternPawn(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePatternBPawn(int tile, CHESS_CORE_PLAYER player, bool check);
void CHESS_PiecePatternKnight(int tile, CHESS_CORE_PLAYER player, bool check);

#endif

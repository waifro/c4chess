#ifndef FEN_H
#define FEN_H

#include "chess.h"

int FEN_Init(CHESS_CORE_PLAYER player, char *fen_board);
int FEN_InitPieceCase(CHESS_CORE_PLAYER player, CHESS_CORE_PIECE *piece, CHESS_CORE_ENUM_PIECE name, int tile);
CHESS_CORE_ENUM_PIECE FEN_CaseEnumPiece(CHESS_CORE_PLAYER player, char character);

#endif // FEN

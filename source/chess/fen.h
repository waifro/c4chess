#ifndef FEN_H
#define FEN_H

#include "chess.h"

int FEN_Init(CHESS_CORE_PLAYER *init_player, char *fen_notation);

int FEN_StrTrunk_TagToTile(char *str);
void FEN_StrTrunk(char *restrict str, char *restrict a, char *restrict b, char *restrict c, char *restrict d, int *restrict e, int *restrict f);
int FEN_PlayerTurn(int *init_player, char fen_play);

int FEN_InitBoard(CHESS_CORE_PLAYER init_player, char *fen_board);
int FEN_InitPiece(CHESS_CORE_PLAYER player, CHESS_CORE_PIECE *piece, CHESS_CORE_ENUM_PIECE name, int tile);

CHESS_CORE_ENUM_PIECE FEN_EnumPiece(char character);

#endif // FEN

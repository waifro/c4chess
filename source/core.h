#ifndef _CORE_H
#define _CORE_H

#include <stdbool.h>

#include "pp4m/pp4m.h"

#include "game.h"

/****************************************/

typedef struct {

  PP4M_SDL pp4m; // pp4m texture, rect, etc..
  bool toggle; // when INPUT_.. is detected it switches this toggle for next interaction
  int colomn;
  char row;
  SDL_Color color;
  GAME_PIECE *piece;

} CORE_TILE;

/****************************************/

typedef enum {

  UP = 1,
  UP_RIGHT = 2,

  RIGHT = 3,
  RIGHT_DOWN = 4,

  DOWN = 5,
  DOWN_LEFT = 6,

  LEFT = 7,
  LEFT_UP = 8

} CORE_MOVEMENT;

/****************************************/

typedef enum {

    NONE = 1,

    LONG = 2,
    SHORT = 3,
    BOTH = 4,

    DISABLE = 5

} CORE_CASTLING;

/****************************************/

extern CORE_TILE tile[64];
extern CORE_TILE point[64];
extern CORE_CASTLING global_whitecastling;
extern CORE_CASTLING global_blackcastling;

int CORE_ReturnTilePosition(int colomn, char row);
int CORE_ReturnValidTilePosition(int colomn, char row);
int CORE_ReturnRowPosition(char row);

int CORE_CheckPieceMovement(int pos);
int CORE_UpdateMovementPieceFromPoint(int pos);
int CORE_SwitchPlayerTurn(int player);

int CORE_CheckCapturePiece_DarkPawn(int pos);
int CORE_CheckCapturePiece_Pawn(int pos);
int CORE_CheckCapturePiece(int colomn, char row);

int GAME_UpdateRookCastling(int pos);
int CORE_UpdateValidCastling(int new, int old);
int CORE_CheckKingCastling(CORE_CASTLING castle, int pos);

int CORE_CreatePatternDarkPawn(int pos);
int CORE_CreatePatternPawn(int pos);
int CORE_CreatePatternKnight(int pos);
int CORE_CreatePatternBishop(int pos);
int CORE_CreatePatternRook(int pos);
int CORE_CreatePatternQueen(int pos);
int CORE_CreatePatternKing(int pos);

void CORE_InitializationPoint(void);

#endif

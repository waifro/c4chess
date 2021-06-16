#ifndef _CORE_H
#define _CORE_H

#include <stdbool.h>

#include "pp4m/pp4m.h"

#include "game.h"

typedef struct {

  PP4M_SDL pp4m; // pp4m texture, rect, etc..
  bool toggle; // when INPUT_.. is detected it switches this toggle for next interaction
  int colomn;
  char row;
  SDL_Color color;
  GAME_PIECE *piece;

} CORE_TILE;

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

/*
typedef struct {

  int colomn;
  char row;

} CORE_POSITION;
*/
extern CORE_TILE tile[64];

int CORE_ReturnTilePosition(int colomn, char row);
int CORE_CheckMovementKPawn(GAME_PIECE *piece, CORE_MOVEMENT movement, int space);
int CORE_CheckMovementDarkPawn(GAME_PIECE *piece, CORE_MOVEMENT movement, int space);
int CORE_CheckMovementKnight(GAME_PIECE *piece, CORE_MOVEMENT movement, int space);

#endif

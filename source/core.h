#ifndef _CORE_H
#define _CORE_H

#include "pp4m/pp4m.h"

#define BLACK_PAWN      "resources/dpawn.png"
#define BLACK_KNIGHT    "resources/dknight.png"
#define BLACK_BISHOP    "resources/dbishop.png"
#define BLACK_ROOK      "resources/drook.png"
#define BLACK_QUEEN     "resources/dqueen.png"
#define BLACK_KING      "resources/dking.png"

#define WHITE_PAWN      "resources/wpawn.png"
#define WHITE_KNIGHT    "resources/wknight.png"
#define WHITE_BISHOP    "resources/wbishop.png"
#define WHITE_ROOK      "resources/wrook.png"
#define WHITE_QUEEN     "resources/wqueen.png"
#define WHITE_KING      "resources/wking.png"

typedef struct {

  PP4M_SDL pp4m;
  int toggle;
  int colomn;
  char row;
  SDL_Color color;

} CORE_TILE;

#endif

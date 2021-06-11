#ifndef _CORE_H
#define _CORE_H

#include "pp4m/pp4m.h"

typedef struct {

  PP4M_SDL pp4m;
  int toggle;
  int colomn;
  char row;
  SDL_Color color;

} CORE_TILE;

#endif

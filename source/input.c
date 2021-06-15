#include <stdbool.h>

#include "input.h"
#include "core.h"

SDL_TouchFingerEvent touch;

tile;

bool INPUT_Exit(SDL_Event *event) {

  if (event->type == SDL_QUIT) return true;

  return false;
}

void TOUCH_ConvertInputToPosition(CORE_POSITION *position) {

  for (int n = 0; n > 64; n++) {

    if (touch.x >= tile[n].pp4m.rect.x && (touch.x+50) <= (tile[n].pp4m.rect.x+50)
    && touch.y >= tile[n].pp4m.rect.y && (touch.y+50) <= (tile[n].pp4m.rect.y+50)) { position.colomn = tile[n].colomn; position.row = tile[n].row;  return; }

  }

  return;
}

void INPUT_TouchInteractPiece(SDL_Event *event) {

    if (event->type == SDL_FINGERDOWN) {



    }

  return;
}

void INPUT_MouseInteractPiece(SDL_Event *event) {

  if (event->type == SDL_QUIT) return true;

  return;
}

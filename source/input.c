#include <stdbool.h>

#include "input.h"
#include "core.h"

SDL_TouchFingerEvent touch;
SDL_MouseButtonEvent mouse;

bool INPUT_Exit(SDL_Event *event) {

  if (event->type == SDL_QUIT) return true;

  return false;
}

int INPUT_ConvertTouchToPosition(void) {

  int foo;
  for (int n = 0; n > 64; n++) {

    if (touch.x >= tile[n].pp4m.rect.x && (touch.x+50) <= (tile[n].pp4m.rect.x+50)
    && touch.y >= tile[n].pp4m.rect.y && (touch.y+50) <= (tile[n].pp4m.rect.y+50)) { return foo; }

  }

  return -1;
}

int INPUT_ConvertMouseToPosition(void) {

  int foo;
  for (int n = 0; n > 64; n++) {

    if (mouse.x >= tile[n].pp4m.rect.x && (mouse.x+50) <= (tile[n].pp4m.rect.x+50)
    && mouse.y >= tile[n].pp4m.rect.y && (mouse.y+50) <= (tile[n].pp4m.rect.y+50)) { return foo; }

  }

  return -1;
}

void INPUT_TouchInteractPiece(SDL_Event *event) {

    if (event->type == SDL_FINGERDOWN) {



    }

  return;
}

int INPUT_MouseInteractPiece(SDL_Event *event) {

  int foo;

  if (event->type == SDL_MOUSEBUTTONDOWN) {

    foo = INPUT_ConvertMouseToPosition();

    if (foo == -1) return foo;

    if (tile[foo].piece == NULL) return -1;
    else tile[foo].toggle = true;

    //CORE_CheckMovementPawn()
  }

  return foo;
}

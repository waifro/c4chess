#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "input.h"
#include "core.h"

SDL_TouchFingerEvent touch;
SDL_MouseButtonEvent mouse;

bool INPUT_Exit(SDL_Event *event) {

  if (event->type == SDL_QUIT) return true;

  return false;
}

int INPUT_ConvertPositionToTile(int x, int y) {

  int z = 0;
  for (int n = 0; n < 8; n++) {

    if (y >= tile[z].pp4m.rect.y && y < (tile[z].pp4m.rect.y)+50)
    for (int i = 0; i < 8; i++) {

        if (x >= tile[z].pp4m.rect.x && x < (tile[z].pp4m.rect.x)+50) return z;

        z += 1;
      }
    z += 8;
  }

  return -1;
}

int INPUT_TouchInteractPiece(SDL_Event *event) {

    int foo = -1;
    if (event->type == SDL_FINGERDOWN) {

          int touch_x, touch_y;
          SDL_GetMouseState(&touch_x, &touch_y);

          foo = INPUT_ConvertPositionToTile(touch_x, touch_y);

          if (foo == -1) return foo;

          if (tile[foo].piece == NULL) return -1;
          else tile[foo].toggle = true;

          //CORE_CheckMovementPawn()
        }

  return foo;
}

int INPUT_MouseInteractPiece(SDL_Event *event) {

  int foo = -1;
  if (event->type == SDL_MOUSEBUTTONDOWN) {

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    foo = INPUT_ConvertPositionToTile(mouse_x, mouse_y);

    if (foo == -1) return foo;

    if (tile[foo].piece == NULL) return -1;
    else tile[foo].toggle = true;

    //CORE_CheckMovementPawn()
  }

  return foo;
}

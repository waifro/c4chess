#include <SDL2/SDL.h>
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

  int z;
  for (int n = 0; n < 8; n++) {

    if (touch.y >= tile[n].pp4m.rect.y && touch.y <= (tile[n].pp4m.rect.y+50)) {

      for (int i = 0; i < 8; i++) {

        if (touch.x >= tile[n].pp4m.rect.x && touch.x <= (tile[n].pp4m.rect.x+50)) return z;
        z += 1;

      }

    }

    z += 8;

  }

  return -1;
}

int INPUT_ConvertMouseToPosition(int x, int y) {

  int z;
  for (int n = 0; n < 8; n++) {

    if (y >= tile[n].pp4m.rect.y && y <= (tile[n].pp4m.rect.y+50)) {

      for (int i = 0; i < 8; i++) {

        if (x >= tile[n].pp4m.rect.x && x <= (tile[n].pp4m.rect.x+50)) return z;
        z += 1;

      }

    }

    z += 8;

  }

  return -1;
}

int INPUT_TouchInteractPiece(SDL_Event *event) {

    int foo = -1;

    if (event->type == SDL_FINGERDOWN) {

          foo = INPUT_ConvertTouchToPosition();

          if (foo == -1) return foo;

          if (tile[foo].piece == NULL) return -1;
          else tile[foo].toggle = true;

          //CORE_CheckMovementPawn()
        }

  return foo;
}

int INPUT_MouseInteractPiece(SDL_Event *event) {

  int foo = -1; bool door = false;
  if (event->type == SDL_MOUSEBUTTONDOWN && door == false) {

    door = true;
    printf("test\n");

    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    foo = INPUT_ConvertMouseToPosition(mouse_x, mouse_y);

    if (foo == -1) return foo;

    if (tile[foo].piece == NULL) return -1;
    else tile[foo].toggle = true;

    //CORE_CheckMovementPawn()
  }

  return foo;
}

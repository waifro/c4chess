#ifndef _INPUT_H
#define _INPUT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "core.h"

bool INPUT_Exit(SDL_Event *event);

int INPUT_ConvertTouchToPosition(void);
int INPUT_ConvertMouseToPosition(int x, int y);

int INPUT_TouchInteractPiece(SDL_Event *event);
int INPUT_MouseInteractPiece(SDL_Event *event);

#endif

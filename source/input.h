#ifndef _INPUT_H
#define _INPUT_H

#include <stdbool.h>
#include <SDL2/SDL.h>

bool INPUT_Exit(SDL_Event *event);

int INPUT_ConvertPositionToTile(int x, int y);

int INPUT_TouchInteractPiece(SDL_Event *event);
int INPUT_MouseInteractPiece(SDL_Event *event);

#endif

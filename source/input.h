#ifndef _INPUT_H
#define _INPUT_H

#include <stdbool.h>

bool INPUT_Exit(SDL_Event *event);
void TOUCH_ConvertInputToPosition(CORE_POSITION *position);
void INPUT_TouchInteractPiece(SDL_Event *event);
void INPUT_MouseInteractPiece(SDL_Event *event);

#endif

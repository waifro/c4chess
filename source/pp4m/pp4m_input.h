#ifndef PP4M_INPUT_H
#define PP4M_INPUT_H

#include <SDL2/SDL.h>

typedef struct {
    int x;
    int y;
    int iner;
} PP4M_INPUT_POS;

int pp4m_INPUT_ConvertSdlKey_number(int sdlk);
int pp4m_INPUT_ConvertSdlKey_letter(int sdlk);
int pp4m_INPUT_ConvertSdlKey_symbol(int sdlk);
int pp4m_INPUT_SdlSpecialKey(int sdlk);

int pp4m_INPUT_KeyboardHit(void);

int pp4m_INPUT_SdlKeyboardKey(int sdlk);
int pp4m_INPUT_SdlKeyboard(SDL_Event *event);
void pp4m_INPUT_GetMouseState(SDL_Event *event, PP4M_INPUT_POS *foo);
PP4M_INPUT_POS  pp4m_INPUT_MouseState(SDL_Event *event);

#endif // PP4M_INPUT_H

#ifndef POPUP_H
#define POPUP_H

#include <SDL2/SDL.h>
#include "../pp4m/pp4m.h"

PP4M_HOOK *GUI_PopupWindow_Init(int w, int h);

int GUI_PopupWindow_Title(PP4M_HOOK *head, char *path, char *title, SDL_Color color, int point);
int GUI_PopupWindow_Button(PP4M_HOOK *head, char *path, int OBJ_FLAG, char *title, SDL_Color color_text, int point, SDL_Color color_button, int x_pp, int y_pp, int w, int h);

int GUI_PopupWindow_Core(PP4M_HOOK *head, SDL_Texture *background);

#endif // POPUP_H

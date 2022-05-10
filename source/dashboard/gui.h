#ifndef GUI_H
#define GUI_H

#include "gui_alias.h"

#define OPENSANS_REGULAR "./resources/OpenSans-Regular.ttf"

/* prototypes */

SDL_Texture *GUI_PopupWindow_Title(char *title, SDL_Rect *rect, SDL_Color color, SDL_Rect window_pos);
GUI_TextureAlias GUI_CreateTexture_BackgroundInit(SDL_Color color);
GUI_TextureAlias GUI_CreateTexture_BackgroundPolarize(SDL_Color color, int alpha);
GUI_TextureAlias GUI_CreateTexture_Button(char *title, SDL_Color color, int x, int y, int w, int h);

int GUI_PopupWindow_Core(PP4M_HOOK *list_hook, int x, int y, int w, int h, char *title);

void GUI_Testing(void);

#endif

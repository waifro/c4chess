#ifndef GUI_H
#define GUI_H

#include "gui_alias.h"

#define OPENSANS_REGULAR "./resources/OpenSans-Regular.ttf"

/* prototypes */

SDL_Texture *GUI_PopupWindow_Title(char title[256], SDL_Rect *rect, SDL_Rect window_pos);
GUI_TextureAlias GUI_CreateTexture_BackgroundPolarize(void);
GUI_TextureAlias GUI_CreateTexture_ButtonExit(int x, int y);

int GUI_PopupWindow_Core(int x, int y, int w, int h, char *text);

void GUI_Testing(void);

#endif

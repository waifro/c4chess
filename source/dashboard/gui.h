#ifndef GUI_H
#define GUI_H

#include "gui_alias.h"

/* prototypes */
int GUI_PopupWindow_Core(int x, int y, int w, int h, char *text);
GUI_TextureAlias GUI_CreateTexture_ButtonExit(int x, int y);
//SDL_Texture *GUI_PopupWindow_Title(char title[256], SDL_Rect window_size);

#endif

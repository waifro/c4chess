#ifndef GUI_H
#define GUI_H

int GUI_PopupWindow(int x, int y, int w, int h, char *text);
SDL_Texture *GUI_PopupWindow_Title(char title[256], SDL_Rect window_size);

#endif

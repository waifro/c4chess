#ifndef GUI_H
#define GUI_H

#include "gui_alias.h"

#define OPENSANS_REGULAR "./resources/OpenSans-Regular.ttf"

/* prototypes */

/*
GUI_TextureAlias *GUI_PopupWindow_Window(SDL_Color color, int x, int y, int w, int h) ;
SDL_Texture *GUI_PopupWindow_Title(char *title, SDL_Rect *rect, SDL_Color color, SDL_Rect window_pos);
GUI_TextureAlias *GUI_CreateTexture_BackgroundInit(SDL_Color color);
*/
GUI_TextureAlias *GUI_CreateTexture_BackgroundInit(GUI_TextureAlias *ttr_alias, SDL_Color color, int alpha);
/*
GUI_TextureAlias GUI_CreateTexture_Button(char *title, SDL_Color color, int x, int y, int w, int h);
int GUI_PopupWindow_Button(PP4M_HOOK *head, char *path, char *title, SDL_Color color_text, SDL_Color color_button, int point, int x_pp, int y_pp, int w, int h);
*/
PP4M_HOOK *GUI_PopupWindow_Init(int w, int h);
int GUI_PopupWindow_CoreTest(PP4M_HOOK *head, SDL_Texture *background);
/*
int GUI_PopupWindow_Core(PP4M_HOOK *list_hook, int x, int y, int w, int h);
void GUI_Testing(void);
*/
#endif

#ifndef GUI_H
#define GUI_H

#include "gui_alias.h"
#include "../pp4m/pp4m.h"

#define OPENSANS_REGULAR "./resources/OpenSans-Regular.ttf"

/* prototypes */
int GUI_PopupWindow_Title(PP4M_HOOK *head, char *path, char *title, SDL_Color color, int point);
int GUI_PopupWindow_Button(PP4M_HOOK *head, char *path, int FLAG_OBJ, char *title, SDL_Color color_text, int point, SDL_Color color_button, int x_pp, int y_pp, int w, int h);

PP4M_HOOK *GUI_PopupWindow_Init(int w, int h);
int GUI_PopupWindow_Core(PP4M_HOOK *head, SDL_Texture *background);

void GUI_Testing(void);

PP4M_HOOK *GUI_RenderWindow_Chat_Init(PP4M_HOOK *hook_list);

#endif

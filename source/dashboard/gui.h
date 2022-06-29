#ifndef GUI_H
#define GUI_H

#include "gui_alias.h"
#include "../pp4m/pp4m.h"

#define OPENSANS_REGULAR "./resources/OpenSans-Regular.ttf"

/* prototypes */

void GUI_Testing(void);

PP4M_HOOK *GUI_RenderWindow_Chat_Init(PP4M_HOOK *hook_list);

int GUI_HookLink_Render(PP4M_HOOK *link);
int GUI_HookList_Render(PP4M_HOOK *hook_list);
void GUI_HookList_Quit(PP4M_HOOK *hook_list);

int GUI_HookLink_Update(PP4M_HOOK *link, PP4M_INPUT_POS input, char *buffer, int key);
int GUI_HookList_Update(PP4M_HOOK *hook_list, PP4M_INPUT_POS input, int key);

#endif

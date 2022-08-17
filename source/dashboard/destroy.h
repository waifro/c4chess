#ifndef DESTROY_H
#define DESTROY_H

#include "gui.h"

#include "../pp4m/pp4m.h"

/* on success, destroy window's chat */
int GUI_Destroy_WindowChat(GUI_TextureAlias *window);

/* on success, destroy window's chat inner window */
int GUI_Destroy_WindowChat_InnerWindow(GUI_TextureAlias *inner_window);

/* on success, destroy window's chat list */
int GUI_Destroy_WindowChat_Chat(PP4M_HOOK *list);

/* on success, destroy textbox */
int GUI_Destroy_Textbox(GUI_TextureAlias *alias_ttr);

#endif

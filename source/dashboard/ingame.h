#ifndef INGAME_H
#define INGAME_H

#include <SDL2/SDL.h>
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_input.h"
#include "gui_alias.h"

// init OBJ_BUTTON_LINK_OFF which will contain windowed chat as link
int GUI_Ingame_ChatInit(PP4M_HOOK *hook_list);

// init obj containing structure of windowed chat
GUI_TextureAlias *GUI_Ingame_ChatInit_Window(GUI_TextureAlias *alias_button_chat);

// by using OBJ_WINDOW, where going to create a chat floating
PP4M_HOOK *GUI_Ingame_ChatInit_InnerWindow(GUI_TextureAlias *window_inner_oob);

int GUI_Ingame_ChatUpdate(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char **buffer);

// on success, returns a new buffer ready for a new check
char *GUI_Ingame_ChatUpdate_NewLine(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char *buf_user, char *buffer);

// on success, it creates a new object ready to be rendered
int GUI_Ingame_ChatUpdate_AddLine(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char *buf_user, char *buffer);

int GUI_Ingame_ChatUpdate_Scroll(GUI_TextureAlias *inner_window_oob);

int GUI_Ingame_ChatInit_RenderObj_Increase(GUI_TextureAlias *render_obj, GUI_TextureAlias *new_alias);

int GUI_Ingame_TimerInit_Player(PP4M_HOOK *hook_list);
int GUI_Ingame_TimerInit_OppositePlayer(PP4M_HOOK *hook_list);
int GUI_Ingame_TimerInit_Clock(GUI_TextureAlias *timer, int point);

#endif

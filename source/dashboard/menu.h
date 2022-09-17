#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>

#include "gui_alias.h"

#include "../pp4m/pp4m.h"

// main menu core
int MENU_Core(SDL_Texture *background);

// on success, update the rendering of the object's list
int MENU_Core_UpdateRender(SDL_Texture *bg, PP4M_HOOK *hook_list);

// on success, create a main list for objects
PP4M_HOOK *MENU_HookList_Init(void);

// on success, initializes the buttons and the properties of OBJ_LINK_PTR
int MENU_ListButtons_Init(PP4M_HOOK *hook_list);

// on success, create a button inside OBJ_LINK_PTR
int MENU_ListButtons_InitButton(GUI_TextureAlias *ttr_alias, char *path, GUI_ALIAS_OBJ obj_flag, char *title, SDL_Color color_text, int point, SDL_Color color_button);

#endif

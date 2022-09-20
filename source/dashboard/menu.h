#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>

#include "gui_alias.h"

#include "../pp4m/pp4m.h"

#include "../c4network/net_utils.h"

int MENU_HookListArr_Init(PP4M_HOOK **hook_list_arr, int val);

int MENU_HookList_Quit(PP4M_HOOK **hook_list_arr, int val);

// main menu core
int MENU_Core(SDL_Texture *background, cli_t *socket);

// on success, return to main menu, unless *index is less then 1
int MENU_HookLisk_ReturnToMenu(PP4M_HOOK **hook_list_arr, int *index);

int MENU_UpdateRedirect_HookLink(PP4M_HOOK **hook_list_arr, int *index, PP4M_INPUT_POS *input, int *socket);

PP4M_HOOK *MENU_Play_HookList(void);

PP4M_HOOK *MENU_Play_LoadingGame_Online_HookList(int *socket);

int MENUPtr_SEQ_AssignLobby(int *socket);

int MENU_Submenu_Play_LocalButton(PP4M_HOOK *hook_list);
int MENU_Submenu_Play_OnlineButton(PP4M_HOOK *hook_list);

// on success, update the rendering of the object's list
int MENU_Core_UpdateRender(SDL_Texture *bg, PP4M_HOOK *hook_list, int *timer);

// on success, create a main list for objects
PP4M_HOOK *MENU_HookList_Init(void);

// on success, initializes a logo in TTF
int MENU_ListInit_Logo(PP4M_HOOK *hook_list);

// on success, initializes the buttons and the properties of OBJ_LINK_PTR
int MENU_ListInit_Buttons(PP4M_HOOK *hook_list);

// on success, create a button inside OBJ_LINK_PTR
int MENU_ListButtons_InitButton(GUI_TextureAlias *ttr_alias, char *path, GUI_ALIAS_OBJ obj_flag, char *title, SDL_Color color_text, int point, SDL_Color color_button);

#endif

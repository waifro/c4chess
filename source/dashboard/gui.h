#ifndef GUI_H
#define GUI_H

#include "gui_alias.h"
#include "../pp4m/pp4m.h"

#define OPENSANS_REGULAR "./resources/OpenSans-Regular.ttf"
#define OPENSANS_EXTRABOLD "./resources/OpenSans-ExtraBold.ttf"

#define TEXTURE_C4_LOGO "./resources/c4chess-logo.jpg"
#define TEXTURE_LOBBYCHAT "resources/logo_mesg.png"
#define TEXTURE_LOBBYCHAT_SEND "resources/logo_direct.png"

/* prototypes */

void GUI_Testing(void);

PP4M_HOOK *GUI_RenderWindow_Chat_Init(PP4M_HOOK *hook_list);

int GUI_HookLink_RenderObj(GUI_TextureAlias *alias_ptr);

int GUI_HookLink_Render(PP4M_HOOK *link);

void GUI_HookList_Quit(PP4M_HOOK *hook_list);

int GUI_HookLink_Update(PP4M_HOOK *link, PP4M_INPUT_POS input, char **buf_arr, int key, int *code);

int GUI_HoverMouse_Highlight(PP4M_INPUT_POS *input, GUI_TextureAlias **alias_ttr, GUI_TextureAlias **alias_ptr, SDL_Color *color_bak);

#endif

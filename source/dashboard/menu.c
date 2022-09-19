#include "menu.h"
#include "gui.h"
#include "gui_alias.h"

#include "../global.h"
#include "../security/debug.h"

#include "../chess/core.h"

#include "../c4network/net.h"
#include "../c4network/client.h"
#include "../c4network/server.h"
#include "../c4network/net_utils.h"

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_image.h"

int MENU_HookListArr_Init(PP4M_HOOK **hook_list_arr, int val) {
	
	for (int i = 0; i < val; i++)
		hook_list_arr[i] = NULL;
	
	hook_list_arr[0] = MENU_HookList_Init();
	
	return 0;
}

int MENU_HookList_Quit(PP4M_HOOK **hook_list_arr, int val) {
	
	for (int i = 0; i < val; i++)
		hook_list_arr[i] = NULL;

	return 0;
}

int MENU_Core(SDL_Texture *background, cli_t *socket) {
	
	int result = 0;
	int index = 0;
	int fps_timer = 0;
	
	// to maintain the same loop for every submenu, popup, etc..
	// we are going to use a static hook_list array (with 32 lists Maximum), 
	// to switch from on hook_list to another in a fast way,
	// while keeping most of the loaded stuff still in memory,
	// without passing them into every function.
	
	PP4M_HOOK *hook_list_arr[32];
	MENU_HookListArr_Init(hook_list_arr, 32);
	
	PP4M_HOOK *listButtons_ptr = GUI_Alias_FindObj(hook_list_arr[index], OBJ_LINK_PTR);
	if (listButtons_ptr == NULL) return -1;
	
	SDL_Event event;
	PP4M_INPUT_POS input = pp4m_INPUT_InitInputPos();
	
	while(result != -1) {
		SDL_PollEvent(&event);
		pp4m_INPUT_GetMouseState(&event, &input);
		if (event.type == SDL_QUIT) break;
		
		MENU_Core_UpdateRender(background, hook_list_arr[index], &fps_timer);
		
		// hop into a new hook_list
		result = MENU_UpdateRedirect_HookLink(hook_list_arr, &index, &input, socket);
	}
	
	MENU_HookList_Quit(hook_list_arr, 32);

	return 0;
}

int MENU_UpdateRedirect_HookLink(PP4M_HOOK **hook_list_arr, int *index, PP4M_INPUT_POS *input, int *socket) {
	int result = -1;
	
	result = GUI_HookLink_Update(hook_list_arr[*index], *input, NULL, -1, (int*){&(int){-1}}, socket);
	
	if (result == -1) {
		GUI_HookList_Quit(hook_list_arr[*index]);
		*index -= 1;
	}
	
	else if (result == OBJ_BUTTON_PLAY) {
		*index += 1;
		hook_list_arr[*index] = MENU_Play_HookList();
	}
	
	else if (result == OBJ_BUTTON_PLAY_ONLINE) {
		*index += 1;
		hook_list_arr[*index] = MENU_Play_LoadingGame_Online_HookList(socket);
	}
	
	// exit if index is below zero
	if (*index < 0) result = -1;
	
	return result;
}

PP4M_HOOK *MENU_Play_HookList(void) {
	
	PP4M_HOOK *hook_list = pp4m_HOOK_Init();
	
	MENU_Submenu_Play_OnlineButton(hook_list);
	MENU_Submenu_Play_LocalButton(hook_list);	
	
	return hook_list;
}

PP4M_HOOK *MENU_Play_LoadingGame_Online_HookList(int *socket) {

	PP4M_HOOK *hook_list = pp4m_HOOK_Init();

	// a object to containing a function under callbacks
	GUI_TextureAlias *container = GUI_Alias_InitAlias();
	container->obj = OBJ_FUNC_CONTAINER;
	
	int (*func)(int *) = &MENUPtr_SEQ_AssignLobby;
	container->add = func;
	
	GUI_TextureAlias *alias = GUI_Alias_InitAlias();
	alias->obj = OBJ_NONE;
	alias->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_BLACK, 72, &alias->dst_rect, 0, glo_screen_h - 100, "loading...");
	
	pp4m_HOOK_Next(hook_list, container);
	pp4m_HOOK_Next(hook_list, alias);
	
	// meanwhile, we send an assign code
	char buffer[256];
	cl_redirect_clcode_REQ(CL_REQ_ASSIGN_LOBBY, buffer);
    NET_SendPacketToServer(socket, buffer, strlen(buffer)+1);
	
	return hook_list;
}

int MENUPtr_SEQ_AssignLobby(int *socket) {
	int result = 0;
	
	if (NET_DetectSignal(socket) > 0) {
	
		char buffer[256];
		
		result = cl_GrabPacket(socket, buffer);
		
		printf("buf recieved: [%s]\n", buffer);
		
		if (result != SV_LOBBY_POST_INIT)
			return -1;
			
		CHESS_CORE_PLAYER player;
		
		char *fen_notation = CORE_NET_ChessboardInit(&player, buffer);
		
		CORE_InitChess_Play(player, fen_notation, socket);
		
		return 1;
	}
	
	return 0;
}

int MENU_Submenu_Play_LocalButton(PP4M_HOOK *hook_list) {
	
	GUI_TextureAlias *play_button = NULL;
	GUI_TextureAlias *play_image = NULL;
	GUI_TextureAlias *play_comment = NULL;
	
	play_button = GUI_Alias_CreateButton(hook_list, OBJ_BUTTON_PLAY_LOCAL, PP4M_GREY_NORMAL, 255, (glo_screen_w / 3) - 150, glo_screen_h / 2 - 150, 300, 300);
	
	play_image = GUI_Alias_AddImage(play_button, TEXTURE_TWO_PEOPLE, 0, 0, 250, 250);
	play_comment = GUI_Alias_AddComment(play_button, OPENSANS_REGULAR, PP4M_GREY_LIGHT, 24, 0, 0, glo_lang[_LANG_SET_LOCAL]);
	
	GUI_Alias_AlignObject_Proportion(play_button, play_image, 50, 45);
	GUI_Alias_AlignObject_Proportion(play_button, play_comment, 50, 92);
	
	return 0;
}

int MENU_Submenu_Play_OnlineButton(PP4M_HOOK *hook_list) {
	
	GUI_TextureAlias *play_button = NULL;
	GUI_TextureAlias *play_image = NULL;
	GUI_TextureAlias *play_comment = NULL;
	
	play_button = GUI_Alias_CreateButton(hook_list, OBJ_BUTTON_PLAY_ONLINE, PP4M_GREY_NORMAL, 255, (glo_screen_w / 3) * 2 - 150, glo_screen_h / 2 - 150, 300, 300);
	
	play_image = GUI_Alias_AddImage(play_button, TEXTURE_GLOBE_WORLDWIDE, 0, 0, 200, 200);
	play_comment = GUI_Alias_AddComment(play_button, OPENSANS_REGULAR, PP4M_GREY_LIGHT, 24, 0, 0, glo_lang[_LANG_SET_GLOBAL]);
	
	GUI_Alias_AlignObject_Proportion(play_button, play_image, 50, 45);
	GUI_Alias_AlignObject_Proportion(play_button, play_comment, 50, 92);
	
	return 0;
}

int MENU_Core_UpdateRender(SDL_Texture *bg, PP4M_HOOK *hook_list, int *timer) {
	
	 if (pp4m_FramerateTimer(CLOCKS_PER_SEC / 60, timer, 0) == true) {
	
		SDL_RenderClear(glo_render);
		
		SDL_RenderCopy(glo_render, bg, NULL, NULL);
		GUI_HookLink_Render(hook_list);
		DEBUG_UpdateBox_Render();
	
		SDL_RenderPresent(glo_render);
	}
	
	return 0;
}

PP4M_HOOK *MENU_HookList_Init(void) {
	
	PP4M_HOOK *hook_list = pp4m_HOOK_Init();
	
	// init objects
	MENU_ListInit_Logo(hook_list);
	MENU_ListInit_Buttons(hook_list);
	
	return hook_list;
}

int MENU_ListInit_Logo(PP4M_HOOK *hook_list) {

	GUI_TextureAlias *c4_alias = GUI_Alias_InitAlias();
	c4_alias->obj = OBJ_NONE;
	c4_alias->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_C4_LOGO, &c4_alias->dst_rect, 250, 50, 250, 300);
	
	GUI_TextureAlias *chess_font = GUI_Alias_InitAlias();
	chess_font->obj = OBJ_NONE;
	chess_font->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_BLACK, 200, &chess_font->dst_rect, 0, 0, "chess");

	GUI_Alias_AlignObject_Proportion(c4_alias, chess_font, 260, 70);
	
	pp4m_HOOK_Next(hook_list, c4_alias);
	pp4m_HOOK_Next(hook_list, chess_font);
	
	return 0;
}

int MENU_ListInit_Buttons(PP4M_HOOK *hook_list) {
	
	GUI_TextureAlias *alias	= GUI_Alias_InitAlias();
	alias->obj = OBJ_LINK_PTR;
	
	// positioning a dinamyc rectangle to place the buttons inside
	alias->dst_rect.x = 50;
	alias->dst_rect.y = 350;
	alias->dst_rect.w = 400;
	alias->dst_rect.h = 50;
	
	// create a linked list to host the buttons
	alias->link = pp4m_HOOK_Init();
	
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_BUTTON_PLAY, glo_lang[_LANG_SET_PLAY], PP4M_BLACK, 26, PP4M_GREY_NORMAL);	
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_NONE, glo_lang[_LANG_SET_STATISTICS], PP4M_GREY_NORMAL, 26, PP4M_GREY_LIGHT);		
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_NONE, glo_lang[_LANG_SET_INFO], PP4M_GREY_NORMAL, 26, PP4M_GREY_LIGHT);	
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_NONE, glo_lang[_LANG_SET_SETTINGS], PP4M_GREY_NORMAL, 26, PP4M_GREY_LIGHT);		
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_BUTTON_RETURN, glo_lang[_LANG_SET_QUIT], PP4M_BLACK, 26, PP4M_GREY_NORMAL);	
	
	pp4m_HOOK_Next(hook_list, alias);
	
	return 0;
}

int MENU_ListButtons_InitButton(GUI_TextureAlias *ttr_alias, char *path, GUI_ALIAS_OBJ obj_flag, char *title, SDL_Color color_text, int point, SDL_Color color_button) {
	
	PP4M_HOOK *curr = ttr_alias->link;
	GUI_TextureAlias *last_alias = NULL;
	
	while (curr->next != NULL)
		curr = curr->next;
		
	if (curr->ptr == NULL)
		last_alias = ttr_alias;
	else last_alias = curr->ptr;
	
	GUI_TextureAlias *button = GUI_Alias_InitAlias();
	button->obj = obj_flag;
	button->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, color_button, 255, &button->dst_rect, last_alias->dst_rect.x, last_alias->dst_rect.y + last_alias->dst_rect.h + 5, ttr_alias->dst_rect.w, last_alias->dst_rect.h);
	
	GUI_Alias_WriteFontOnTop(button, path, color_text, point, title);
	
	pp4m_HOOK_Next(ttr_alias->link, button);
	
	return 0;
}



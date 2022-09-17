#include "menu.h"
#include "gui.h"
#include "gui_alias.h"

#include "../global.h"
#include "../security/debug.h"

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"

int MENU_Core(SDL_Texture *background) {

	PP4M_HOOK *hook_list = MENU_HookList_Init();
	
	PP4M_HOOK *listButtons_ptr = GUI_Alias_FindObj(hook_list, OBJ_LINK_PTR);
	if (listButtons_ptr == NULL) return -1;
	
	SDL_Event event;
	
	while(1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) break;
		
		MENU_Core_UpdateRender(background, hook_list);
	}
	
	GUI_HookList_Quit(hook_list);

	return 0;
}

int MENU_Core_UpdateRender(SDL_Texture *bg, PP4M_HOOK *hook_list) {
	
	SDL_RenderClear(glo_render);
		
	SDL_RenderCopy(glo_render, bg, NULL, NULL);
	GUI_HookLink_Render(hook_list);
	DEBUG_UpdateBox_Render();
	
	SDL_RenderPresent(glo_render);

	return 0;
}

PP4M_HOOK *MENU_HookList_Init(void) {
	
	PP4M_HOOK *hook_list = pp4m_HOOK_Init();
	
	// init objects
	MENU_ListButtons_Init(hook_list);
	
	return hook_list;
}

int MENU_ListButtons_Init(PP4M_HOOK *hook_list) {
	
	GUI_TextureAlias *alias	= GUI_Alias_InitAlias();
	alias->obj = OBJ_LINK_PTR;
	
	// positioning a dinamyc rectangle to place the buttons inside
	alias->dst_rect.x = 50;
	alias->dst_rect.y = 500;
	alias->dst_rect.w = 300;
	
	// create a linked list to host the buttons
	alias->link = pp4m_HOOK_Init();
	
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_MENU_PLAY, glo_lang[_LANG_SET_PLAY], PP4M_BLACK, 24, PP4M_GREY_LIGHT);	
	
	pp4m_HOOK_Next(hook_list, alias);
	
	return 0;
}

int MENU_ListButtons_InitButton(GUI_TextureAlias *ttr_alias, char *path, GUI_ALIAS_OBJ obj_flag, char *title, SDL_Color color_text, int point, SDL_Color color_button) {
	
	PP4M_HOOK *curr = ttr_alias->link;
	GUI_TextureAlias *last_alias = NULL;
	
	while (curr->ptr != NULL) {
		last_alias = curr->ptr;
		curr = curr->next;
	}
	
	if (last_alias == NULL) {
		last_alias = ttr_alias;
	}
	
	GUI_TextureAlias *button = GUI_Alias_InitAlias();
	button->obj = obj_flag;
	button->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, color_button, 255, &button->dst_rect, last_alias->dst_rect.x, last_alias->dst_rect.y + last_alias->dst_rect.h, ttr_alias->dst_rect.w, 30);
	
	GUI_Alias_WriteFontOnTop(button, path, color_text, point, title);
	
	pp4m_HOOK_Next(ttr_alias->link, button);
	
	return 0;
}



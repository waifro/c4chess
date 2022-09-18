#include "menu.h"
#include "gui.h"
#include "gui_alias.h"

#include "../global.h"
#include "../security/debug.h"

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_image.h"

int MENU_Core(SDL_Texture *background) {

	PP4M_HOOK *hook_list = MENU_HookList_Init();
	
	PP4M_HOOK *listButtons_ptr = GUI_Alias_FindObj(hook_list, OBJ_LINK_PTR);
	if (listButtons_ptr == NULL) return -1;
	
	SDL_Event event;
	PP4M_INPUT_POS input = pp4m_INPUT_InitInputPos();
	
	while(1) {
		SDL_PollEvent(&event);
		pp4m_INPUT_GetMouseState(&event, &input);
		if (event.type == SDL_QUIT) break;
		
		GUI_HookLink_Update(hook_list, input, NULL, -1, (int*){&(int){-1}});
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
	
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_MENU_PLAY, glo_lang[_LANG_SET_PLAY], PP4M_BLACK, 26, PP4M_GREY_NORMAL);	
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_NONE, glo_lang[_LANG_SET_STATISTICS], PP4M_GREY_NORMAL, 26, PP4M_GREY_LIGHT);		
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_NONE, glo_lang[_LANG_SET_INFO], PP4M_GREY_NORMAL, 26, PP4M_GREY_LIGHT);	
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_NONE, glo_lang[_LANG_SET_SETTINGS], PP4M_GREY_NORMAL, 26, PP4M_GREY_LIGHT);		
	MENU_ListButtons_InitButton(alias, OPENSANS_REGULAR, OBJ_MENU_EXIT, glo_lang[_LANG_SET_QUIT], PP4M_BLACK, 26, PP4M_GREY_NORMAL);	
	
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



#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../c4network/net_utils.h"
#include "../c4network/client.h"
#include "../c4network/server.h"

#include "../global.h"
#include "../security/debug.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "../chess/core.h"

#include "ingame.h"
#include "gui.h"
#include "gui_alias.h"
#include "textbox.h"

// init OBJ_BUTTON_LINK_OFF which will contain windowed chat as link
int GUI_Ingame_ChatInit(PP4M_HOOK *hook_list) {

    GUI_TextureAlias *alias_button_chat = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_button_chat->obj = OBJ_BUTTON_LINK_OFF;
    alias_button_chat->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT, &alias_button_chat->dst_rect, 850, 600, 30, 30);

    // save object containing properties of the chat
    alias_button_chat->link = GUI_Ingame_ChatInit_Window(alias_button_chat);

    // hook to main list
    pp4m_HOOK_Next(hook_list, alias_button_chat);

    return 0;
}

// init obj containing structure of windowed chat
GUI_TextureAlias *GUI_Ingame_ChatInit_Window(GUI_TextureAlias *alias_button_chat) {

    GUI_TextureAlias *window = GUI_Alias_InitAlias();
    window->obj = OBJ_WINDOW_CHAT;
    window->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &window->dst_rect, alias_button_chat->dst_rect.x + 20, alias_button_chat->dst_rect.y - 440, 300, 450);

    /* list of objects for windowed chat */

    GUI_TextureAlias *alias_blank_window = GUI_Alias_InitAlias();
    alias_blank_window->obj = OBJ_NONE;
    alias_blank_window->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &alias_blank_window->dst_rect, window->dst_rect.x + 10, window->dst_rect.y + 10, window->dst_rect.w - 20, window->dst_rect.h - 50);

    // init OBJ_WINDOW_INNER_OOB
    GUI_TextureAlias *alias_window_inner_oob = GUI_Alias_InitAlias();
    alias_window_inner_oob->obj = OBJ_WINDOW_INNER_OOB_CHAT;

    // initialize out of bounds rect where textures will be allocated
    alias_window_inner_oob->dst_rect.x = alias_blank_window->dst_rect.x + 5;
    alias_window_inner_oob->dst_rect.y = alias_blank_window->dst_rect.y + 5;
    alias_window_inner_oob->dst_rect.w = alias_blank_window->dst_rect.w - 10;
    alias_window_inner_oob->dst_rect.h = alias_blank_window->dst_rect.h - 10;

    // init inner window stuff
    alias_window_inner_oob->link = GUI_Ingame_ChatInit_InnerWindow(alias_window_inner_oob);

    GUI_TextureAlias *alias_textbox = GUI_Alias_InitAlias();
    alias_textbox->obj = OBJ_TEXTBOX_ALIAS;
    alias_textbox->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_WHITE, 255, &alias_textbox->dst_rect, alias_blank_window->dst_rect.x, alias_blank_window->dst_rect.y + alias_blank_window->dst_rect.h + 5, alias_blank_window->dst_rect.w - 30, 30);

    GUI_TextureAlias *alias_text = GUI_Alias_InitAlias();

    alias_textbox->link = alias_text;
    alias_text->obj = OBJ_TEXTBOX_INPUT_OFF;
    GUI_Textbox_InitAlias(alias_textbox, OPENSANS_REGULAR, PP4M_GREY_NORMAL, 18, glo_lang[_LANG_PROMPT_INPUT_TEXT]);

    GUI_TextureAlias *alias_button_send = GUI_Alias_InitAlias();
    alias_button_send->obj = OBJ_BUTTON_TXTBOX;
    alias_button_send->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT_SEND, &alias_button_send->dst_rect, alias_textbox->dst_rect.x + alias_textbox->dst_rect.w + 5, alias_textbox->dst_rect.y, 30, 30);

    // save list into obj
    PP4M_HOOK *window_obj_list = pp4m_HOOK_Init();

    pp4m_HOOK_Next(window_obj_list, alias_blank_window);
    pp4m_HOOK_Next(window_obj_list, alias_window_inner_oob);
    pp4m_HOOK_Next(window_obj_list, alias_textbox);
    pp4m_HOOK_Next(window_obj_list, alias_button_send);

    window->link = window_obj_list;

    return window;
}

// by using OBJ_WINDOW_CHAT, where going to create a chat floating
PP4M_HOOK *GUI_Ingame_ChatInit_InnerWindow(GUI_TextureAlias *window_inner_oob) {

    // initialize linked list for scrollable object + OBJ_WINDOW_OOB_RENDER containing list of chat
    PP4M_HOOK *init_list_struct = pp4m_HOOK_Init();

    //int scroll_size_delta = 3; // (it was used to create a space between objects for nicer ui)
    int scroll_size_width = 5;

    GUI_TextureAlias *scroll_vertical = GUI_Alias_InitAlias();
    scroll_vertical->obj = OBJ_SCROLL_VERTICAL;
    scroll_vertical->dst_rect.x = window_inner_oob->dst_rect.x + window_inner_oob->dst_rect.w - scroll_size_width;
    scroll_vertical->dst_rect.y = window_inner_oob->dst_rect.y;
    scroll_vertical->dst_rect.w = scroll_size_width;
    scroll_vertical->dst_rect.h = window_inner_oob->dst_rect.h;

    // initialize OBJ_WINDOW_OOB_RENDER for chat redirection
    GUI_TextureAlias *render_obj = GUI_Alias_InitAlias();
    render_obj->obj = OBJ_WINDOW_OOB_RENDER;
    render_obj->dst_rect.x = 0;
    render_obj->dst_rect.y = 0;
    render_obj->dst_rect.w = window_inner_oob->dst_rect.w;
    render_obj->dst_rect.h = 0;

    // save list of initialized chat to render_obj
    PP4M_HOOK *list_obj = pp4m_HOOK_Init();
    render_obj->link = list_obj;

    // completed structure of inner Window OOB
    pp4m_HOOK_Next(init_list_struct, scroll_vertical);
    pp4m_HOOK_Next(init_list_struct, render_obj);

    return init_list_struct;
}

int GUI_Ingame_ChatUpdate(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char **buf_arr) {
    if (buf_arr == NULL) return -1;
    else if (retrieve_code(buf_arr[0]) != SV_LOBBY_POST_MESG) return -2;

	if (strlen(buf_arr[0]) != 0) {

		// temporary fix of user
    	char buf_user[17];
    	int len_buf = 0;
    	sscanf(buf_arr[0], "%s %*s", buf_user);
    	len_buf = strlen(buf_user) + 1; // adding the space

		char *buf = &buf_arr[0][len_buf];

		while (strlen(buf) >= 35)
			buf = GUI_Ingame_ChatUpdate_NewLine(inner_window_oob, pathname, color, point, buf_user, buf);

    	GUI_Ingame_ChatUpdate_AddLine(inner_window_oob, pathname, color, point, buf_user, buf);
    	GUI_Ingame_ChatUpdate_Scroll(inner_window_oob);

	}

	if (strlen(buf_arr[1]) != 0) {

		// temporary fix of user
    	char buf_user[17];
    	int len_buf = 0;
    	sscanf(buf_arr[1], "%s %*s", buf_user);
    	len_buf = strlen(buf_user) + 1; // adding the space

		char *buf = &buf_arr[1][len_buf];

		while (strlen(buf) >= 35)
			buf = GUI_Ingame_ChatUpdate_NewLine(inner_window_oob, pathname, color, point, buf_user, buf);

    	GUI_Ingame_ChatUpdate_AddLine(inner_window_oob, pathname, color, point, buf_user, buf);
    	GUI_Ingame_ChatUpdate_Scroll(inner_window_oob);

	}

    return 0;
}

char *GUI_Ingame_ChatUpdate_NewLine(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char *buf_user, char *buf) {

    int len = strlen(buf);

	int i;
	for (i = 30; i < len; i++) {

		if (buf[i] != '\0' && buf[i] != ' ' && i < 33) continue;

		buf[i++] = '\0';
		GUI_Ingame_ChatUpdate_AddLine(inner_window_oob, pathname, color, point, buf_user, buf);

		break;
	}

    return (&buf[i]);
}

int GUI_Ingame_ChatUpdate_AddLine(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char *buf_user, char *buffer) {
    if (buffer == NULL) return -1;

    // get to last obj of list from innerWindow_OOB containing OBJ_WINDOW_OOB_RENDER
    PP4M_HOOK *tail = GUI_Alias_Tail(inner_window_oob);
    GUI_TextureAlias *render_obj = tail->ptr;

    if (render_obj->obj != OBJ_WINDOW_OOB_RENDER) return -2;

    // chat linked list
    PP4M_HOOK *head_chat = render_obj->link;
    PP4M_HOOK *tail_chat = GUI_Alias_Tail(render_obj);

    GUI_TextureAlias *new_alias = GUI_Alias_InitAlias();
    SDL_Rect rect = {0, 0, 0, 0};

    new_alias->obj = OBJ_CHAT_MESG;
    new_alias->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &new_alias->dst_rect, 0, 0, buffer);

    // grab last message height
    if (head_chat->ptr != NULL) {
        GUI_TextureAlias *alias_ptr = tail_chat->ptr;

        // grab (y + height) value of last message
        rect.y = alias_ptr->dst_rect.y + alias_ptr->dst_rect.h;
    }

    // message incoming from opponent
    if (strcmp(glo_user.username, buf_user) == 0)
        rect.x = inner_window_oob->dst_rect.w - new_alias->dst_rect.w;

    // (old) apply height to message
    new_alias->dst_rect.x = rect.x;
    new_alias->dst_rect.y = rect.y;

    GUI_Ingame_ChatInit_RenderObj_Increase(render_obj, new_alias);

    pp4m_HOOK_Next(head_chat, new_alias);

    return 0;
}

int GUI_Ingame_ChatUpdate_Scroll(GUI_TextureAlias *inner_window_oob) {

    PP4M_HOOK *obj_ptr = GUI_Alias_FindObj(inner_window_oob->link, OBJ_SCROLL_VERTICAL);
    if (obj_ptr == NULL) return -1;
    GUI_TextureAlias *scroll_obj = obj_ptr->ptr;

    obj_ptr = GUI_Alias_FindObj(inner_window_oob->link, OBJ_WINDOW_OOB_RENDER);
    if (obj_ptr == NULL) return -2;
    GUI_TextureAlias *render_obj = obj_ptr->ptr;

    if (render_obj->dst_rect.h > inner_window_oob->dst_rect.h) {

        // init texture of scroll
        if (scroll_obj->texture == NULL)
            scroll_obj->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_BLACK, &scroll_obj->dst_rect, scroll_obj->dst_rect.x, scroll_obj->dst_rect.y, scroll_obj->dst_rect.w, scroll_obj->dst_rect.h);

        // temporary fix untill src_rect & dst_rect
        int src_render_inner_y = render_obj->dst_rect.h - inner_window_oob->dst_rect.h;
        float size_diff_page = (float)render_obj->dst_rect.h / (float)inner_window_oob->dst_rect.h;
        float foo = 0;
        int bar = 0;

        for (int i = 0; i < inner_window_oob->dst_rect.h; i++) {
            foo += size_diff_page;

            if (foo < src_render_inner_y) {
                scroll_obj->dst_rect.y = inner_window_oob->dst_rect.y + i;
                bar = i;
                continue;
            } else if (foo < (src_render_inner_y + inner_window_oob->dst_rect.h + 1)) {
                scroll_obj->dst_rect.h = i - bar;
                continue;
            }

            break;
        }
    }

    return 0;
}

int GUI_Ingame_ChatInit_RenderObj_Increase(GUI_TextureAlias *render_obj, GUI_TextureAlias *new_alias) {
    if (render_obj->texture != NULL)
        SDL_DestroyTexture(render_obj->texture);

    int xw = new_alias->dst_rect.x + new_alias->dst_rect.w;
    int yh = new_alias->dst_rect.y + new_alias->dst_rect.h;

    if (xw > render_obj->dst_rect.w) render_obj->dst_rect.w = xw;
    if (yh > render_obj->dst_rect.h) render_obj->dst_rect.h = yh;

    // increase size of new obj
    SDL_Texture *texture = NULL;
    texture = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, render_obj->dst_rect.w, render_obj->dst_rect.h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    render_obj->texture = texture;

    return 0;
}

int GUI_Ingame_TimerInit_OppositePlayer(PP4M_HOOK *hook_list) {
	int point = 40;

    GUI_TextureAlias *timer = GUI_Alias_InitAlias();
    timer->obj = OBJ_DISPLAY_TIMER;
    timer->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &timer->dst_rect, 0, 0, 140, 70);

    // allocate player
    timer->add = (void*)malloc(sizeof(CHESS_CORE_PLAYER));
    int *foo = timer->add;
	*foo = CORE_ReversePlayer_State(glo_chess_core_player);

    GUI_Ingame_TimerInit_Clock(timer, point);

    // save object into main list
    pp4m_HOOK_Next(hook_list, timer);

    return 0;
}

int GUI_Ingame_TimerInit_Player(PP4M_HOOK *hook_list) {
	int point = 40;

    GUI_TextureAlias *timer = GUI_Alias_InitAlias();
    timer->obj = OBJ_DISPLAY_TIMER;
    timer->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &timer->dst_rect, glo_screen_w - 140, 0, 140, 70);

    // allocate player
    timer->add = (void*)malloc(sizeof(CHESS_CORE_PLAYER));
    int *foo = timer->add;
	*foo = glo_chess_core_player;

    GUI_Ingame_TimerInit_Clock(timer, point);

    // save object into main list
    pp4m_HOOK_Next(hook_list, timer);

    return 0;
}

int GUI_Ingame_TimerInit_Clock(GUI_TextureAlias *timer, int point) {

    PP4M_HOOK *list = pp4m_HOOK_Init();

    GUI_TextureAlias *blink_dots = GUI_Alias_InitAlias();
    blink_dots->obj = OBJ_DISPLAY_TIMER_BD;
    blink_dots->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_WHITE, point, &blink_dots->dst_rect, 0, 0, ":");

    GUI_TextureAlias *minutes = GUI_Alias_InitAlias();
    minutes->obj = OBJ_DISPLAY_TIMER_MIN;
    minutes->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_WHITE, point, &minutes->dst_rect, 0, 0, "00");

    GUI_TextureAlias *seconds = GUI_Alias_InitAlias();
    seconds->obj = OBJ_DISPLAY_TIMER_SEC;
    seconds->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_WHITE, point, &seconds->dst_rect, (blink_dots->dst_rect.x + blink_dots->dst_rect.w) + 5, blink_dots->dst_rect.y, "00");


    // modify position of clocks
    GUI_Alias_AlignObject_Middle(timer, blink_dots);
    GUI_Alias_AlignObject_Proportion(timer, minutes, 25, 50);
    GUI_Alias_AlignObject_Proportion(timer, seconds, 75, 50);
    
    /*
	blink_dots->dst_rect.x = (timer->dst_rect.x + timer->dst_rect.w) - (timer->dst_rect.w / 2) - (width / 2);
	blink_dots->dst_rect.y = (timer->dst_rect.h / 2) - (height / 2);

	minutes->dst_rect.x = blink_dots->dst_rect.x - width;
	minutes->dst_rect.y = blink_dots->dst_rect.y;

	seconds->dst_rect.x = (blink_dots->dst_rect.x + blink_dots->dst_rect.w);
	seconds->dst_rect.y = blink_dots->dst_rect.y;
	*/
	
    // save items inside list
    pp4m_HOOK_Next(list, minutes);
    pp4m_HOOK_Next(list, blink_dots);
    pp4m_HOOK_Next(list, seconds);
    timer->link = list;

    return 0;
}

int GUI_Ingame_TimerUpdate_Clock(GUI_TextureAlias *display) {
    if (display == NULL) return -1;

    PP4M_HOOK *min_ptr = GUI_Alias_FindObj(display->link, OBJ_DISPLAY_TIMER_MIN);
    if (min_ptr == NULL) return -2;

    PP4M_HOOK *sec_ptr = GUI_Alias_FindObj(display->link, OBJ_DISPLAY_TIMER_SEC);
    if (sec_ptr == NULL) return -3;

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../network/client.h"
#include "../network/server.h"

#include "../global.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "ingame.h"
#include "gui.h"
#include "gui_alias.h"

// init OBJ_BUTTON_LINK_OFF which will contain windowed chat as link
int GUI_Ingame_ChatInit(PP4M_HOOK *hook_list) {

    GUI_TextureAlias *alias_button_chat = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_button_chat->obj = OBJ_BUTTON_LINK_OFF;
    alias_button_chat->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT, &alias_button_chat->rect, 850, 600, 30, 30);

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
    window->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &window->rect, alias_button_chat->rect.x + 20, alias_button_chat->rect.y - 440, 300, 450);

    /* list of objects for windowed chat */

    GUI_TextureAlias *alias_blank_window = GUI_Alias_InitAlias();
    alias_blank_window->obj = OBJ_NONE;
    alias_blank_window->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &alias_blank_window->rect, window->rect.x + 10, window->rect.y + 10, window->rect.w - 20, window->rect.h - 50);

    // init OBJ_WINDOW_INNER_OOB
    GUI_TextureAlias *alias_window_inner_oob = GUI_Alias_InitAlias();
    alias_window_inner_oob->obj = OBJ_WINDOW_INNER_OOB_CHAT;

    // initialize out of bounds rect where textures will be allocated
    alias_window_inner_oob->rect.x = alias_blank_window->rect.x + 5;
    alias_window_inner_oob->rect.y = alias_blank_window->rect.y + 5;
    alias_window_inner_oob->rect.w = alias_blank_window->rect.w - 10;
    alias_window_inner_oob->rect.h = alias_blank_window->rect.h - 10;

    // init inner window stuff
    alias_window_inner_oob->link = GUI_Ingame_ChatInit_InnerWindow(alias_window_inner_oob);

    GUI_TextureAlias *alias_textbox = GUI_Alias_InitAlias();
    alias_textbox->obj = OBJ_TEXTBOX_ALIAS;
    alias_textbox->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_WHITE, 255, &alias_textbox->rect, alias_blank_window->rect.x, alias_blank_window->rect.y + alias_blank_window->rect.h + 5, alias_blank_window->rect.w - 30, 30);

    GUI_TextureAlias *alias_text = GUI_Alias_InitAlias();

    alias_textbox->link = alias_text;
    alias_text->obj = OBJ_TEXTBOX_INPUT_OFF;
    GUI_Alias_Textbox_InitAlias(alias_textbox, OPENSANS_REGULAR, PP4M_GREY_NORMAL, 18, "Input text here");

    GUI_TextureAlias *alias_button_send = GUI_Alias_InitAlias();
    alias_button_send->obj = OBJ_BUTTON_TXTBOX;
    alias_button_send->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT_SEND, &alias_button_send->rect, alias_textbox->rect.x + alias_textbox->rect.w + 5, alias_textbox->rect.y, 30, 30);

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

    int scroll_size_delta = 3;
    int scroll_size_width = 5;

    GUI_TextureAlias *scroll_vertical = GUI_Alias_InitAlias();
    scroll_vertical->obj = OBJ_NULL;
    scroll_vertical->rect.x = window_inner_oob->rect.x + window_inner_oob->rect.w - scroll_size_width - scroll_size_delta;
    scroll_vertical->rect.y = window_inner_oob->rect.y + scroll_size_delta;
    scroll_vertical->rect.w = scroll_size_width;
    scroll_vertical->rect.h = window_inner_oob->rect.h - (scroll_size_delta*2);

    // initialize OBJ_WINDOW_OOB_RENDER for chat redirection
    GUI_TextureAlias *render_obj = GUI_Alias_InitAlias();
    render_obj->obj = OBJ_WINDOW_OOB_RENDER;
    render_obj->rect.x = 0;
    render_obj->rect.y = 0;
    render_obj->rect.w = window_inner_oob->rect.w;
    render_obj->rect.h = 0;

    // save list of initialized chat to render_obj
    PP4M_HOOK *list_obj = pp4m_HOOK_Init();
    render_obj->link = list_obj;

    // completed structure of inner Window OOB
    pp4m_HOOK_Next(init_list_struct, scroll_vertical);
    pp4m_HOOK_Next(init_list_struct, render_obj);

    return init_list_struct;
}

int GUI_Ingame_ChatUpdate(PP4M_HOOK *list_window_chat_obj, char *pathname, SDL_Color color, int point, char **buffer) {
    if (*buffer == NULL) return -1;

    // properties of window_inner_oob
    PP4M_HOOK *link_inner_window_oob = GUI_Alias_FindObj(list_window_chat_obj, OBJ_WINDOW_INNER_OOB_CHAT);
    if (link_inner_window_oob == NULL) return -1;

    GUI_TextureAlias *inner_window_oob = link_inner_window_oob->ptr;

    // temporary fix of user
    char buf_user[17];
    int len_buf = 0;
    sscanf(*buffer, "%s %*s", buf_user);
    len_buf = strlen(buf_user) + 1; // adding the space

    char *buf = &(*buffer)[len_buf];

    // create a new texture for new lines
    while (strlen(buf) > 35) {
        buf = GUI_Ingame_ChatUpdate_NewLine(inner_window_oob, pathname, color, point, buf_user, buf);
        if (buf == NULL) break;
    }

    GUI_Ingame_ChatUpdate_AddLine(inner_window_oob, pathname, color, point, buf_user, buf);

    return 0;
}

char *GUI_Ingame_ChatUpdate_NewLine(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char *buf_user, char *buf) {

    int len = strlen(buf);

        int i;
        for (i = 30; i < len; i++) {

            if (buf[i] == '\0') return NULL;
            else if (buf[i] == ' ') {

                buf[i++] = '\0';
                GUI_Ingame_ChatUpdate_AddLine(inner_window_oob, pathname, color, point, buf_user, buf);

                break;
            }
        }

    return (&buf[i]);
}

int GUI_Ingame_ChatUpdate_AddLine(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char *buf_user, char *buffer) {

    // get to last obj of list from innerWindow_OOB containing OBJ_WINDOW_OOB_RENDER
    PP4M_HOOK *tail = GUI_Alias_Tail(inner_window_oob);
    GUI_TextureAlias *render_obj = tail->ptr;

    if (render_obj->obj != OBJ_WINDOW_OOB_RENDER) return -1;

    // chat linked list
    PP4M_HOOK *head_chat = render_obj->link;
    PP4M_HOOK *tail_chat = GUI_Alias_Tail(render_obj);

    GUI_TextureAlias *new_alias = GUI_Alias_InitAlias();
    SDL_Rect rect = {0, 0, 0, 0};

    new_alias->obj = OBJ_CHAT_MESG;
    new_alias->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &new_alias->rect, 0, 0, buffer);

    // grab last message height
    if (head_chat->ptr != NULL) {
        GUI_TextureAlias *alias_ptr = tail_chat->ptr;

        // grab (y + height) value of last message
        rect.y = alias_ptr->rect.y + alias_ptr->rect.h;
    }

    // message incoming from opponent
    if (strcmp(glo_user.username, buf_user) == 0)
        rect.x = inner_window_oob->rect.w - new_alias->rect.w;

    // (old) apply height to message
    new_alias->rect.x = rect.x;
    new_alias->rect.y = rect.y;

    GUI_Ingame_ChatInit_RenderObj_Increase(render_obj, new_alias);

    pp4m_HOOK_Next(head_chat, new_alias);

    return 0;
}

int GUI_Ingame_ChatInit_RenderObj_Increase(GUI_TextureAlias *render_obj, GUI_TextureAlias *new_alias) {
    if (render_obj->texture != NULL)
        SDL_DestroyTexture(render_obj->texture);

    int xw = new_alias->rect.x + new_alias->rect.w;
    int yh = new_alias->rect.y + new_alias->rect.h;

    if (xw > render_obj->rect.w) render_obj->rect.w = xw;
    if (yh > render_obj->rect.h) render_obj->rect.h = yh;

    // increase size of new obj
    SDL_Texture *texture = NULL;
    texture = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, render_obj->rect.w, render_obj->rect.h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    render_obj->texture = texture;

    return 0;
}

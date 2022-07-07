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

// init linked list containg objects for windowed chat
PP4M_HOOK *GUI_Ingame_ChatInit(PP4M_HOOK *hook_list) {

    PP4M_HOOK *chat_init_list = pp4m_HOOK_Init();

    GUI_TextureAlias *alias_button_chat = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_button_chat->obj = OBJ_BUTTON_LINK_OFF;
    alias_button_chat->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT, &alias_button_chat->rect, 850, 600, 30, 30);

    GUI_TextureAlias *alias_window = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_window->obj = OBJ_WINDOW_CHAT;
    alias_window->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &alias_window->rect, alias_button_chat->rect.x + 20, alias_button_chat->rect.y - 440, 300, 450);

    GUI_TextureAlias *alias_window_chat = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_window_chat->obj = OBJ_WINDOW_INNER_OOB_CHAT;
    alias_window_chat->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &alias_window_chat->rect, alias_window->rect.x + 10, alias_window->rect.y + 10, alias_window->rect.w - 20, alias_window->rect.h - 50);

    // init OBJ_WINDOW_INNER_OOB
    GUI_Ingame_Chat_InnerWindow_Init(alias_window_chat);

    GUI_TextureAlias *alias_textbox = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_textbox->obj = OBJ_TEXTBOX_ALIAS;
    alias_textbox->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_WHITE, 255, &alias_textbox->rect, alias_window_chat->rect.x, alias_window_chat->rect.y + alias_window_chat->rect.h + 5, alias_window_chat->rect.w - 30, 30);

    GUI_TextureAlias *alias_text = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));

    // save pointer of hooked Alias
    alias_textbox->link = alias_text;
    alias_text->obj = OBJ_TEXTBOX_INPUT_OFF;
    GUI_Alias_Textbox_InitAlias(alias_textbox, OPENSANS_REGULAR, PP4M_GREY_NORMAL, 18, "Input text here");

    GUI_TextureAlias *alias_button_send = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_button_send->obj = OBJ_BUTTON_TXTBOX;
    alias_button_send->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT_SEND, &alias_button_send->rect, alias_textbox->rect.x + alias_textbox->rect.w + 5, alias_textbox->rect.y, 30, 30);

    GUI_TextureAlias *alias_chat = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_chat->obj = OBJ_NULL;

    pp4m_HOOK_Next(chat_init_list, alias_button_chat);

    // save properties to button to open chat
    PP4M_HOOK *chat_ttr_list = pp4m_HOOK_Init();

    // save pointer of linked list of structure of windowed chat
    alias_button_chat->link = chat_ttr_list;

    pp4m_HOOK_Next(chat_ttr_list, alias_window);
    pp4m_HOOK_Next(chat_ttr_list, alias_window_chat);
    pp4m_HOOK_Next(chat_ttr_list, alias_textbox);
    pp4m_HOOK_Next(chat_ttr_list, alias_button_send);
    pp4m_HOOK_Next(chat_ttr_list, alias_chat);

    pp4m_HOOK_Next(hook_list, chat_init_list);

    return chat_ttr_list;
}

// by using OBJ_WINDOW, where going to create a chat floating
int GUI_Ingame_Chat_InnerWindow_Init(GUI_TextureAlias *window) {

    GUI_TextureAlias *window_inner_oob = malloc(sizeof(GUI_TextureAlias));
    window->link = window_inner_oob;

    // initialize out of bounds rect where textures will be allocated
    window_inner_oob->rect.x = window->rect.x - 5;
    window_inner_oob->rect.y = window->rect.y - 5;
    window_inner_oob->rect.w = window->rect.w - 5;
    window_inner_oob->rect.h = window->rect.h - 5;

    // initialize linked list for scrollable object + linked list for chat
    PP4M_HOOK *init_list_struct = pp4m_HOOK_Init();
    window_inner_oob->link = init_list_struct;

    int scroll_size_delta = 3;
    int scroll_size_width = 5;

    GUI_TextureAlias *scroll_vertical = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    scroll_vertical->obj = OBJ_NULL;
    scroll_vertical->rect.x = window_inner_oob->rect.x + window_inner_oob->rect.w - scroll_size_width - scroll_size_delta;
    scroll_vertical->rect.y = window_inner_oob->rect.y + scroll_size_delta;
    scroll_vertical->rect.w = scroll_size_width;
    scroll_vertical->rect.h = window_inner_oob->rect.h - (scroll_size_delta*2);

    // initialize OBJ_LINK_PTR for chat
    PP4M_HOOK *init_list_chat = pp4m_HOOK_Init();

    GUI_TextureAlias *chat_link = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    chat_link->obj = OBJ_LINK_PTR;
    chat_link->link = init_list_chat;

    // completed structure of inner Window OOB
    pp4m_HOOK_Next(init_list_struct, scroll_vertical);
    pp4m_HOOK_Next(init_list_struct, chat_link);

    return 0;
}

int GUI_Ingame_ChatUpdate(GUI_TextureAlias *inner_window_oob, char *pathname, SDL_Color color, int point, char **buffer) {
    if (*buffer == NULL) return -1;

    // get to last obj of list from innerWindow_OOB containing OBJ_LINK_PTR
    PP4M_HOOK *tail = GUI_Alias_Tail(inner_window_oob->link);
    GUI_TextureAlias *obj_link_list = tail->ptr;

    if (obj_link_list->obj != OBJ_LINK_PTR) return -1;

    PP4M_HOOK *head_chat = obj_link_list->link;
    PP4M_HOOK *tail_chat = GUI_Alias_Tail(obj_link_list->link);

    GUI_TextureAlias *alias_ptr = NULL;
    GUI_TextureAlias *new_alias = malloc(sizeof(GUI_TextureAlias));

    SDL_Rect rect = {
        inner_window_oob->rect.x,
        inner_window_oob->rect.y,
        0, 0
    };

    new_alias->obj = OBJ_CHAT_MESG;

    // temporary fix of user
    char buf_user[17];
    int len_buf = 0;
    sscanf(*buffer, "%s %*s", buf_user);
    len_buf = strlen(buf_user) + 1; // adding the space

    new_alias->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &new_alias->rect, 0, 0, &(*buffer)[len_buf]);

    // grab last message height
    if (tail_chat->ptr != head_chat->ptr) {
        alias_ptr = tail_chat->ptr;

        // new message is out of bounds of inner_window_oob
        if ((alias_ptr->rect.y + alias_ptr->rect.h + new_alias->rect.h) > (inner_window_oob->rect.y + inner_window_oob->rect.h)) {

            GUI_Ingame_ChatUpdate_ListUpdate(inner_window_oob);

            // grab (y + height) value of last message
            rect.y = alias_ptr->rect.y + alias_ptr->rect.h + 5;
        }
    }

    // message incoming from opponent
    if (strcmp(glo_user.username, buf_user) != 0) new_alias->rect.x = rect.x;
    else new_alias->rect.x = inner_window_oob->rect.x + inner_window_oob->rect.w - (new_alias->rect.w + 10);

    // apply height to message
    new_alias->rect.y = rect.y;

    pp4m_HOOK_Next(head_chat, new_alias);
    return 0;
}

int GUI_Ingame_ChatUpdate_ListUpdate(GUI_TextureAlias *inner_window) {

    PP4M_HOOK *head = inner_window->link;
    PP4M_HOOK *curr = head;

    GUI_TextureAlias *buf_alias = NULL;
    int val = pp4m_HOOK_Size(head);

    for (int i = 0; i < val; i++) {
        buf_alias = curr->ptr;
        curr = curr->next;

        if (i == 0) continue; // scrollable obj

        buf_alias->rect.y -= buf_alias->rect.h + 5;
    }

    return 0;
}

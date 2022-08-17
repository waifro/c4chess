#include "../lang.h"
#include "../global.h"
#include "../network/server.h"
#include "../network/client.h"

#include "textbox.h"
#include "gui.h"

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int GUI_Textbox_InitAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;

    // allocate space for buffer
    char *link_ptr = malloc(sizeof(char) * 255);
    memset(link_ptr, 0x00, 255);
    alias_ptr->link = link_ptr;

    // create texture
    GUI_Textbox_Empty(alias_ttr, pathname, color, point, buffer);
    GUI_Textbox_BlinkInit(alias_ttr);

    return 0;
}

int GUI_Textbox_UpdateAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char **buffer, int key, int *code) {

    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    GUI_TextureAlias *alias_blink = alias_ttr->add;

    GUI_Textbox_BlinkUpdate(alias_ttr);

    char *msg = alias_ptr->link;
    int msg_len = (int)strlen(msg);

    // nothing
    if (key == 0) {

        // nothing written
        if (msg_len == 0) {
            alias_blink->dst_rect.x = alias_ttr->dst_rect.x;
            GUI_Textbox_Empty(alias_ttr, pathname, PP4M_GREY_NORMAL, point, glo_lang[_LANG_PROMPT_INPUT_TEXT]);
        }

        return 0;
    }

    // backspace
    else if (key == -2) {

        if (msg_len > 0) GUI_Textbox_Backspace(msg);

    }

    // enter
    else if (key == -3) {

        // has been written something
        if (msg_len > 0) {
            int len = strlen(msg) + glo_user.len;
            char *buf_ptr = malloc(sizeof(char) * len + 1);

            memset(buf_ptr, 0x00, len + 1);
            snprintf(buf_ptr, 255, "%s %s", glo_user.username, msg);
            memset(msg, 0x00, len);

            *code = CL_LOBBY_POST_MESG;
            *buffer = buf_ptr;

        }

        return 0;
    }

    // possibly any printable key
    else if (isprint(key)){

        if (msg_len < 256) {
            msg[msg_len] = key;
            msg[msg_len + 1] = '\0';
        }
    }

    // any applicated changes to the message
    if ((int)strlen(msg) != msg_len) {

        SDL_DestroyTexture(alias_ptr->texture);
        GUI_Textbox_UpdateTexture(alias_ttr, pathname, color, point);
    }

    // follow any edit of texture from message
    alias_blink->dst_rect.x = alias_ttr->dst_rect.x + alias_ptr->dst_rect.w;
    alias_blink->timer = 0;

    return 0;
}

int GUI_Textbox_UpdateTexture(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    char *buffer = alias_ptr->link;
    SDL_Rect rect;

    alias_ptr->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &rect, 0, 0, buffer);

    alias_ptr->dst_rect.x = 0;
    alias_ptr->dst_rect.y = 0;
    alias_ptr->dst_rect.w = rect.w;
    alias_ptr->dst_rect.h = rect.h;

    if (rect.w > alias_ttr->dst_rect.w)
        alias_ptr->dst_rect.w = alias_ttr->dst_rect.w;

    return 0;
}

int GUI_Textbox_UpdateRect(GUI_TextureAlias *alias_ttr) {

    GUI_TextureAlias *alias_ptr = alias_ttr->link;

    int w, h;
    SDL_QueryTexture(alias_ptr->texture, NULL, NULL, &w, &h);

    alias_ptr->dst_rect.x = alias_ttr->dst_rect.x;
    alias_ptr->dst_rect.y = alias_ttr->dst_rect.y;

    alias_ptr->src_rect.x = 0;
    alias_ptr->src_rect.y = 0;
    alias_ptr->src_rect.w = w;
    alias_ptr->src_rect.h = h;

    if (w > alias_ttr->dst_rect.w) {
        alias_ptr->src_rect.x = w - alias_ttr->dst_rect.w;
        alias_ptr->dst_rect.w = alias_ttr->dst_rect.w;
    }

    return 0;
}

int GUI_Textbox_Backspace(char *buf) {

    int buf_len = strlen(buf);

    if (buf_len == 0) return 0;
    buf[buf_len-1] = '\0';

    return 1;
}

int GUI_Textbox_Empty(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    SDL_Rect rect;

    alias_ptr->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &rect, 0, 0, buffer);

    alias_ptr->dst_rect.x = 0;
    alias_ptr->dst_rect.y = 0;
    alias_ptr->dst_rect.w = rect.w;
    alias_ptr->dst_rect.h = rect.h;

    if (rect.w > alias_ttr->dst_rect.w)
        alias_ptr->dst_rect.x = rect.w - alias_ttr->dst_rect.w;

    return 0;
}

int GUI_Textbox_BlinkInit(GUI_TextureAlias *alias_ttr) {
    GUI_TextureAlias *alias_blink = GUI_Alias_InitAlias();

    alias_blink->obj = OBJ_TEXTBOX_BLINK;
    alias_blink->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_BLACK, 0, &alias_blink->dst_rect, alias_ttr->dst_rect.x, alias_ttr->dst_rect.y, 1, alias_ttr->dst_rect.h);

    alias_blink->src_rect.x = 0;
    alias_blink->src_rect.y = 0;
    alias_blink->src_rect.w = alias_blink->dst_rect.w;
    alias_blink->src_rect.h = alias_blink->dst_rect.h;

    alias_blink->interval = CLOCKS_PER_SEC / 2;
    alias_blink->timer = clock();

    alias_ttr->add = alias_blink;
    return 0;
}

int GUI_Textbox_BlinkUpdate(GUI_TextureAlias *alias_ttr) {
    GUI_TextureAlias *alias_blink = alias_ttr->add;

    if (GUI_Alias_FramerateSet(alias_blink->interval, &alias_blink->timer) == false)
        return -1;

    char val = 0;
    SDL_GetTextureAlphaMod(alias_blink->texture, &val);

    if (val == 0) SDL_SetTextureAlphaMod(alias_blink->texture, 255);
    else SDL_SetTextureAlphaMod(alias_blink->texture, 0);

    return 0;
}

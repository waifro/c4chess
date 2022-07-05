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

#include "gui.h"
#include "gui_alias.h"

int GUI_Alias_InputOnObj(PP4M_INPUT_POS input, SDL_Rect rect) {
    if (input.x >= rect.x && input.x <= (rect.x + rect.w) &&
        input.y >= rect.y && input.y <= (rect.y + rect.h)) return 1;
    else return -1;
}

int GUI_Alias_IsColor(SDL_Color *color) {
    if (color->r == 0 && color->g == 0 && color->b == 0 && color->a == 0) return 1;
    else return -1;
}

int GUI_Alias_ResetColor(SDL_Color *color) {
    color->r = 0; color->g = 0; color->b = 0; color->a = 0;
    return 0;
}

SDL_Texture *GUI_Alias_CreateSnapshot(SDL_Renderer *renderer, int width, int height) {

    SDL_Surface *snap = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, snap->pixels, snap->pitch);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, snap);

    SDL_FreeSurface(snap);

    return (texture);
}

int GUI_Alias_Textbox_Empty(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    SDL_Rect rect;

    alias_ptr->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &rect, 0, 0, buffer);

    alias_ptr->rect.x = 0;
    alias_ptr->rect.y = 0;
    alias_ptr->rect.w = rect.w;
    alias_ptr->rect.h = rect.h;

    if (rect.w > alias_ttr->rect.w)
        alias_ptr->rect.x = rect.w - alias_ttr->rect.w;

    return 0;
}

int GUI_Alias_Textbox_UpdateTexture(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    char *buffer = alias_ptr->link;
    SDL_Rect rect;

    alias_ptr->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &rect, 0, 0, buffer);

    alias_ptr->rect.x = 0;
    alias_ptr->rect.y = 0;
    alias_ptr->rect.w = rect.w;
    alias_ptr->rect.h = rect.h;

    if (rect.w > alias_ttr->rect.w)
        alias_ptr->rect.x = rect.w - alias_ttr->rect.w;

    return 0;
}

int GUI_Alias_Textbox_UpdateRect(GUI_TextureAlias *alias_ttr, SDL_Rect *rect) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;

    rect->x = alias_ttr->rect.x;
    rect->y = alias_ttr->rect.y;

    if (alias_ptr->rect.w > alias_ttr->rect.w) rect->w = alias_ttr->rect.w;
    else rect->w = alias_ptr->rect.w;

    rect->h = alias_ptr->rect.h;

    return 0;
}

int GUI_Alias_Textbox_Backspace(char *buf) {

    int buf_len = strlen(buf);
    if (buf_len == 0) return 0;

    buf[buf_len-1] = '\0';

    return 1;
}

int GUI_Alias_Textbox_InitAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;

    // allocate space for buffer
    char *link_ptr = malloc(sizeof(char) * 255);
    memset(link_ptr, 0x00, 255);
    alias_ptr->link = link_ptr;

    // create texture
    GUI_Alias_Textbox_Empty(alias_ttr, pathname, color, point, buffer);

    return 0;
}

int GUI_Alias_Textbox_UpdateAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char **buffer, int key, int *code) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    char *link_ptr = alias_ptr->link;
    int link_len = strlen(link_ptr) - 1;
    int result = 0;

    if (link_len == -1 && key == 0) {
        GUI_Alias_Textbox_Empty(alias_ttr, pathname, PP4M_GREY_NORMAL, point, "Input text here");
        return 0;
    } else if (link_len >= 255) return 0;

    // create a better key func
    if (key == 0 && link_len > -1) return 0;
    else if (key == -2 && link_len > -1) result += GUI_Alias_Textbox_Backspace(link_ptr);
    else if (key == -3 && link_len > -1) {

        int len = strlen(link_ptr) + glo_user.len;
        char *buf_ptr = malloc(sizeof(char) * len + 1);

        memset(buf_ptr, 0x00, len + 1);
        snprintf(buf_ptr, 255, "%s %s", glo_user.username, link_ptr);
        memset(link_ptr, 0x00, len);

        *code = CL_LOBBY_POST_MESG;
        *buffer = buf_ptr;

        return 0;
    }

    result += isprint(key);

    if (result != 0) {
        link_ptr[++link_len] = key;
        link_ptr[++link_len] = '\0';

        SDL_DestroyTexture(alias_ptr->texture);
        GUI_Alias_Textbox_UpdateTexture(alias_ttr, pathname, color, point);
    }

    return 0;
}

int GUI_Alias_Textbox_DestrAlias(GUI_TextureAlias *alias_ptr) {

    if (alias_ptr->link != NULL) {
        free(alias_ptr->link);
        alias_ptr->link = NULL;
    }

    if (alias_ptr->texture != NULL) {
        SDL_DestroyTexture(alias_ptr->texture);
        alias_ptr->texture = NULL;
    }

    return 0;
}

int GUI_Alias_InnerWindow_Init(GUI_TextureAlias *window) {

    GUI_TextureAlias *window_inner_oob = malloc(sizeof(GUI_TextureAlias));
    window->link = window_inner_oob;

    window_inner_oob->rect.x = window->rect.x - 5;
    window_inner_oob->rect.y = window->rect.y - 5;
    window_inner_oob->rect.w = window->rect.w - 5;
    window_inner_oob->rect.h = window->rect.h - 5;

    PP4M_HOOK *init_list = pp4m_HOOK_Init();
    window_inner_oob->link = init_list;

    return 0;
}

// return -1 if src is NULL, -2 if dest is NULL, otherwise 0 on success
int GUI_Alias_RectCopy(SDL_Rect *dest, SdL_Rect *src) {
    if (src == NULL) return -1;
    else if (dest == NULL) return -2;

    dest->x = src->x;
    dest->y = src->y;
    dest->w = src->w;
    dest->h = src->h;

    return 0;
}

// return -1 if outside of oob, otherwise 0 on success
int GUI_Alias_RectUpdate_OOB(SDL_Rect *rect_1, SDL_Rect *rect_2, SDL_Rect *rect_oob) {

    // rect_2
    rect_2->x = 0;
    rect_2->y = 0;
    rect_2->w = rect_1->w;
    rect_2->h = rect_1->h;

    // rect_1
    int rect_yh = rect_1->y + rect_1->h;
    int rect_xw = rect_1->x + rect_1->w;

    // oob
    int oob_yh = rect_oob->y + rect_oob->h;
    int oob_xw = rect_oob->x + rect_oob->w;

    // object is out of bounds from rect_oob->y
    if (rect_1->y < rect_oob->y) {

        // check also if is outside
        if (rect_yh >= rect_oob->y) return -1;

        int delta_y = rect_oob->y - rect_1->y;

        rect_1->y += delta_y;
        rect_2->y = delta_y;
    }

    // object is out of bounds from oob_yh
    if (rect_yh > oob_yh) {

        // check also if is outside
        if (rect_1->y >= oob_yh) return -1;

        int delta_yh = oob_yh - rect_yh;

        rect_1->h -= delta_yh;
        rect_2->h = delta_yh;
    }


    // object is out of bounds from rect_oob->x
    if (rect_1->x < rect_oob->x) {

        // check also if is outside
        if (rect_xw <= rect_oob->x) return -1;

        int delta_x = rect_oob->x - rect_1->x;

        rect_1->x += delta_x;
        rect_2->x = delta_x;
    }

    // object is out of bounds from oob_xw
    if (rect_xw > oob_xw) {

        // check also if is outside
        if (rect_1->x >= oob_xw) return -1;

        int delta_xw = oob_xw - rect_xw;

        rect_1->w -= delta_xw;
        rect_2->w = delta_xw;
    }

    return 0;
}

int GUI_Alias_InnerWindow_Render(GUI_TextureAlias *window_inner_oob) {
    int result = -1;

    PP4M_HOOK *head = window_inner_oob->link;
    PP4M_HOOK *curr = head;

    int val = pp4m_HOOK_Size(head);
    GUI_TextureAlias *alias_ptr = NULL;

    SDL_Rect alias_rect_bak;
    SDL_Rect src_rect;

    // alias_ptr->rect is dest rect for render
    // src_rect is src rect for texture

    for (int i = 0; i < val; i++) {
        alias_ptr = curr->ptr;
        curr = curr->next;

        if (alias_ptr->obj == OBJ_NULL) continue;
        else if (alias_ptr->obj == OBJ_SCROLL_HORIZONTAL) continue;
        else if (alias_ptr->obj == OBJ_SCROLL_VERTICAL) continue;

        // copy SDL_Rect
        GUI_Alias_RectCopy(&alias_rect_bak, &alias_ptr->rect);

        if (GUI_Alias_RectUpdate_OOB(&alias_ptr->rect, &src_rect, &window_inner_oob->rect) == 0) {
            SDL_RenderCopy(glo_render, alias_ptr->texture, &src_rect, &alias_ptr->rect);

            // restore SDL_Rect
            GUI_Alias_RectCopy(&alias_ptr->rect, &alias_rect_bak);
        }
    }

    return 0;
}

PP4M_HOOK *GUI_Alias_Tail(GUI_TextureAlias *alias) {

    PP4M_HOOK *curr = alias->link;

    while(curr->next != NULL)
        curr = curr->next;

    return curr;
}

int GUI_Alias_InnerWindow_Move(GUI_TextureAlias *inner_window) {

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

int GUI_Alias_InnerWindow_Add(GUI_TextureAlias *inner_window, char *pathname, SDL_Color color, int point, char **buffer, int code) {
    if (*buffer == NULL) return -1;

    PP4M_HOOK *head = inner_window->link;
    PP4M_HOOK *tail = GUI_Alias_Tail(inner_window);

    GUI_TextureAlias *alias_ptr = NULL;
    GUI_TextureAlias *alias_inner_w_scroll = head->ptr;
    GUI_TextureAlias *new_alias = malloc(sizeof(GUI_TextureAlias));

    SDL_Rect rect = {
        inner_window->rect.x,
        inner_window->rect.y,
        0, 0
    };

    new_alias->obj = OBJ_NONE;

    // temporary fix of user
    char buf_user[17];
    int len_buf = 0;

    sscanf(*buffer, "%s %*s", buf_user);
    len_buf = strlen(buf_user) + 1; // adding the space

    new_alias->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &new_alias->rect, 0, 0, &(*buffer)[len_buf]);

    // grab last message height
    if (tail->ptr != head->ptr) {
        alias_ptr = tail->ptr;

        if (alias_ptr != alias_inner_w_scroll) {

            // chat is out of bounds
            if ((alias_ptr->rect.y + alias_ptr->rect.h + new_alias->rect.h) > (inner_window->rect.y + inner_window->rect.h)) {

                GUI_Alias_InnerWindow_Move(inner_window);

                if (alias_inner_w_scroll->obj == OBJ_NULL) {
                    alias_inner_w_scroll->obj = OBJ_SCROLL_VERTICAL;
                    alias_inner_w_scroll->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &alias_inner_w_scroll->rect, alias_inner_w_scroll->rect.x, alias_inner_w_scroll->rect.y, alias_inner_w_scroll->rect.w, alias_inner_w_scroll->rect.h);
                }
            }

            // grab (y + height) value of last message
            rect.y = alias_ptr->rect.y + alias_ptr->rect.h + 5;
        }
    }

    // incoming from opponent
    if (strcmp(glo_user.username, buf_user) != 0) new_alias->rect.x = rect.x;
    else new_alias->rect.x = inner_window->rect.x + inner_window->rect.w - (new_alias->rect.w + 10);

    // apply height to message
    new_alias->rect.y = rect.y;

    pp4m_HOOK_Next(head, new_alias);
    return 0;
}

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
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

bool GUI_Alias_FramerateSet(int val, int *old) {
    if (clock() < (*old + val))
        return false;

    *old += val;
    return true;
}

int GUI_Alias_IsColor(SDL_Color *color) {
    if (color->r == 0 && color->g == 0 && color->b == 0 && color->a == 0) return 1;
    else return -1;
}

int GUI_Alias_ResetColor(SDL_Color *color) {
    color->r = 0; color->g = 0; color->b = 0; color->a = 0;
    return 0;
}

GUI_TextureAlias *GUI_Alias_InitAlias(void) {
    GUI_TextureAlias *alias = malloc(sizeof(GUI_TextureAlias));

    alias->obj = OBJ_NULL;
    alias->texture = NULL;
    alias->link = NULL;
    alias->add = NULL;
    alias->timer = 0;

    return alias;
}

// return NULL on end of list, otherwise return PP4M_HOOK pointer where obj corrisponds on the list
PP4M_HOOK *GUI_Alias_FindObj(PP4M_HOOK *hook_list, GUI_ALIAS_OBJ obj) {
    PP4M_HOOK *ptr = NULL;
    PP4M_HOOK *curr = hook_list;
    GUI_TextureAlias *alias = NULL;

    while(curr != NULL && ptr == NULL) {
        alias = curr->ptr;
        if (alias->obj == obj) ptr = curr;
        curr = curr->next;
    }

    return ptr;
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

    alias_ptr->dst_rect.x = 0;
    alias_ptr->dst_rect.y = 0;
    alias_ptr->dst_rect.w = rect.w;
    alias_ptr->dst_rect.h = rect.h;

    if (rect.w > alias_ttr->dst_rect.w)
        alias_ptr->dst_rect.x = rect.w - alias_ttr->dst_rect.w;

    return 0;
}

int GUI_Alias_Textbox_UpdateTexture(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point) {
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

int GUI_Alias_Textbox_UpdateRect(GUI_TextureAlias *alias_ttr) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    //GUI_TextureAlias *alias_blink = alias_ttr->add;

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

int GUI_Alias_Textbox_Backspace(char *buf) {

    int buf_len = strlen(buf);

    if (buf_len == 0) return 0;
    buf[buf_len-1] = '\0';

    return 1;
}

int GUI_Alias_Textbox_InitBlink(GUI_TextureAlias *alias_ttr) {
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

int GUI_Alias_BlinkUpdate(GUI_TextureAlias *alias_ttr) {
    GUI_TextureAlias *alias_blink = alias_ttr->add;

    if (GUI_Alias_FramerateSet(alias_blink->interval, &alias_blink->timer) == false)
        return -1;

    char val = 0;
    SDL_GetTextureAlphaMod(alias_blink->texture, &val);

    if (val == 0) SDL_SetTextureAlphaMod(alias_blink->texture, 255);
    else SDL_SetTextureAlphaMod(alias_blink->texture, 0);

    return 0;
}

int GUI_Alias_Textbox_InitAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;

    // allocate space for buffer
    char *link_ptr = malloc(sizeof(char) * 255);
    memset(link_ptr, 0x00, 255);
    alias_ptr->link = link_ptr;

    // create texture
    GUI_Alias_Textbox_Empty(alias_ttr, pathname, color, point, buffer);
    GUI_Alias_Textbox_InitBlink(alias_ttr);

    return 0;
}

int GUI_Alias_Textbox_UpdateAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char **buffer, int key, int *code) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    GUI_TextureAlias *alias_blink = alias_ttr->add;

    char *link_ptr = alias_ptr->link;
    int link_len = strlen(link_ptr) - 1;
    int result = 0;

    GUI_Alias_BlinkUpdate(alias_ttr);

    if (link_len == -1 && key == 0) {
        alias_blink->dst_rect.x = alias_ttr->dst_rect.x;

        GUI_Alias_Textbox_Empty(alias_ttr, pathname, PP4M_GREY_NORMAL, point, glo_lang[_LANG_PROMPT_INPUT_TEXT]);
        return 0;
    }

    // create a better key func
    if (key == 0 && link_len > -1) return 0;
    else if (key == -2 && link_len > -1) result += GUI_Alias_Textbox_Backspace(link_ptr);
    else if (link_len > 255) return 0;
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

    // follow
    alias_blink->dst_rect.x = alias_ttr->dst_rect.x + alias_ptr->dst_rect.w;
    alias_blink->timer = 0;

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

// return -1 if src is NULL, -2 if dest is NULL, otherwise 0 on success
int GUI_Alias_RectCopy(SDL_Rect *dest, SDL_Rect *src) {
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

    // get to last obj of list from innerWindow_OOB containing OBJ_WINDOW_OOB_RENDER
    PP4M_HOOK *render_ptr = GUI_Alias_FindObj(window_inner_oob->link, OBJ_WINDOW_OOB_RENDER);
    if (render_ptr == NULL) return -1;
    GUI_TextureAlias *render_obj = render_ptr->ptr;

    // hook to linked list from obj
    if (render_obj->texture == NULL) return -3;

    PP4M_HOOK *curr_alias_list = render_obj->link;

    int val = pp4m_HOOK_Size(curr_alias_list);
    GUI_TextureAlias *alias_ptr = NULL;

    SDL_SetRenderTarget(glo_render, render_obj->texture);
    SDL_RenderClear(glo_render);

    for (int i = 0; i < val; i++) {
        alias_ptr = curr_alias_list->ptr;
        curr_alias_list = curr_alias_list->next;

        // in theory, this is safe, but it crashes if theres a obj, and the render_obj hasnt been initialized
        if (alias_ptr->obj == OBJ_NULL) continue;

        SDL_RenderCopy(glo_render, alias_ptr->texture, NULL, &alias_ptr->dst_rect);
    }

    SDL_SetRenderTarget(glo_render, NULL);

    // temporary fix (untill i add src_dest and src_src to obj's)
    SDL_Rect dst_rect;
    SDL_Rect src_rect;

    dst_rect.x = window_inner_oob->dst_rect.x;
    dst_rect.y = window_inner_oob->dst_rect.y;
    dst_rect.w = render_obj->dst_rect.w;
    dst_rect.h = render_obj->dst_rect.h;

    // for now, we just grub the latest height, in future we will use obj_scroll_..
    src_rect.x = render_obj->dst_rect.x;
    src_rect.y = render_obj->dst_rect.y;
    src_rect.w = render_obj->dst_rect.w;
    src_rect.h = render_obj->dst_rect.h;

    if (render_obj->dst_rect.w > window_inner_oob->dst_rect.w) {
        dst_rect.w = window_inner_oob->dst_rect.w;
        src_rect.x = render_obj->dst_rect.w - window_inner_oob->dst_rect.w;
    }

    if (render_obj->dst_rect.h > window_inner_oob->dst_rect.h) {
        dst_rect.h = window_inner_oob->dst_rect.h;
        src_rect.y = render_obj->dst_rect.h - window_inner_oob->dst_rect.h;
    }

    SDL_RenderCopy(glo_render, render_obj->texture, &src_rect, &dst_rect);

    return 0;
}

PP4M_HOOK *GUI_Alias_Tail(GUI_TextureAlias *alias) {

    PP4M_HOOK *curr = alias->link;

    while(curr->next != NULL)
        curr = curr->next;

    return curr;
}

int GUI_AliasDestroy_WindowChat(GUI_TextureAlias *window) {

    //bug:
    // this deletes the first iteraction, but it will free the last item on list

    PP4M_HOOK *curr = window->link;
    int val = pp4m_HOOK_Size(window->link);

    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        alias_ptr = curr->ptr;

        if (alias_ptr->obj == OBJ_WINDOW_INNER_OOB_CHAT)
            GUI_AliasDestroy_WindowChat_InnerWindow(alias_ptr);
        else if (alias_ptr->obj == OBJ_TEXTBOX_ALIAS)
            GUI_AliasDestroy_Textbox(alias_ptr);

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(window->link);

    return 0;
}

int GUI_AliasDestroy_WindowChat_InnerWindow(GUI_TextureAlias *inner_window) {

    PP4M_HOOK *curr = inner_window->link;
    int val = pp4m_HOOK_Size(inner_window->link);

    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        alias_ptr = curr->ptr;

        if (alias_ptr->obj == OBJ_WINDOW_OOB_RENDER)
            GUI_AliasDestroy_WindowChat_Chat(alias_ptr->link);

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(inner_window->link);

    return 0;
}

int GUI_AliasDestroy_WindowChat_Chat(PP4M_HOOK *list) {

    PP4M_HOOK *curr = list;

    int val = pp4m_HOOK_Size(list);
    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        alias_ptr = curr->ptr;

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(list);

    return 0;
}

int GUI_AliasDestroy_Textbox(GUI_TextureAlias *alias_ttr) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    GUI_TextureAlias *alias_blink = alias_ttr->add;

    if (alias_ptr != NULL) {
        if (alias_ptr->link != NULL)
            free(alias_ptr->link); // char*

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);
    }

    if (alias_blink != NULL) {
        if (alias_blink->texture != NULL)
            SDL_DestroyTexture(alias_blink->texture);
    }

    return 0;
}

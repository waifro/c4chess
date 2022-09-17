#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "../c4network/client.h"
#include "../c4network/server.h"

#include "../global.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "gui.h"
#include "gui_alias.h"
#include "destroy.h"

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

int GUI_Alias_WriteFontOnTop(GUI_TextureAlias *txr_alias, char *path, SDL_Color color, int point, char *title) {

	GUI_TextureAlias *font = GUI_Alias_InitAlias();
	font->texture = pp4m_TTF_TextureFont(glo_render, path, color, point, &font->dst_rect, 0, 0, title);
	
	SDL_SetRenderTarget(glo_render, txr_alias->texture);
	
	font->dst_rect.x = (txr_alias->dst_rect.w / 2) - (font->dst_rect.w / 2);
	font->dst_rect.y = (txr_alias->dst_rect.h / 2) - (font->dst_rect.h / 2);
	
	SDL_RenderCopy(glo_render, font->texture, NULL, &font->dst_rect);
	GUI_Destroy_Alias(font);
	
    SDL_SetRenderTarget(glo_render, NULL);

	return 0;
}

int GUI_Alias_AlignObject_Middle(GUI_TextureAlias *alias_major, GUI_TextureAlias *alias_minor) {
	
	alias_minor->dst_rect.x = (alias_major->dst_rect.x + alias_major->dst_rect.w / 2) - (alias_minor->dst_rect.w / 2);
	alias_minor->dst_rect.y = (alias_major->dst_rect.y + alias_major->dst_rect.h / 2) - (alias_minor->dst_rect.h / 2);
	
	return 0;
}

int GUI_Alias_AlignObject_Proportion(GUI_TextureAlias *alias_major, GUI_TextureAlias *alias_minor, int prop_x, int prop_y) {
	
	int prop_a = alias_major->dst_rect.w / 100;
	int prop_b = alias_major->dst_rect.h / 100;
	
	alias_minor->dst_rect.x = alias_major->dst_rect.x + (prop_a * prop_x) - (alias_minor->dst_rect.w / 2);
	alias_minor->dst_rect.y = alias_major->dst_rect.y + (prop_b * prop_y) - (alias_minor->dst_rect.h / 2);
	
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

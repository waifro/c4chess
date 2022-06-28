#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../global.h"
#include "../pp4m/pp4m.h"
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

int GUI_Alias_Textbox_UpdateAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, int key) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    char *link_ptr = alias_ptr->link;
    int link_len = strlen(link_ptr) - 1;
    int result = 0;

    if (link_len == -1 && key == 0) {
        GUI_Alias_Textbox_Empty(alias_ttr, pathname, PP4M_GREY_NORMAL, point, "Input text here");
        return 0;
    } else if (link_len >= 255) return -1;

    // create a better key func
    if (key == 0 && link_len > -1) return -2;
    if (key == -2 && link_len > -1) result += GUI_Alias_Textbox_Backspace(link_ptr);

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

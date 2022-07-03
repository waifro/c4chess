#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../network/client.h"
#include "../network/server.h"

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

        int len = strlen(link_ptr);
        char *buf_ptr = malloc(sizeof(char) * len + 1);

        memset(buf_ptr, 0x00, len + 1);
        memcpy(buf_ptr, link_ptr, len);
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

int GUI_Alias_InnerWindow_Init(GUI_TextureAlias *alias) {

    PP4M_HOOK *init = pp4m_HOOK_Init();
    alias->link = init;

    return 0;
}

PP4M_HOOK *GUI_Alias_Tail(GUI_TextureAlias *alias) {

    PP4M_HOOK *curr = alias->link;

    while(curr->next != NULL)
        curr = curr->next;

    return curr;
}

int GUI_Alias_InnerWindow_Add(GUI_TextureAlias *alias, char *pathname, SDL_Color color, int point, char **buffer, int code) {
    if (*buffer == NULL) return -1;

    PP4M_HOOK *head = alias->link;
    PP4M_HOOK *tail = GUI_Alias_Tail(alias);

    GUI_TextureAlias *alias_ptr = NULL;
    GUI_TextureAlias *new_alias = malloc(sizeof(GUI_TextureAlias));

    SDL_Rect rect = {
        alias->rect.x,
        alias->rect.y,
        0, 0
    };

    // todo:
    // create a new obj for player 0 and 1
    new_alias->obj = OBJ_NONE;

    // todo:
    // to modify this for both players
    // check if buffer is from same player
    // if yes, dont create new info

    //new_alias->rect.x = rect.x;
    //new_alias->rect.y = rect.y + rect.h + 5;

    // what happends if goes over the width or height of alias->rect?
    //printf("added texture to linked convo: %s\nx %d -> %d\ny %d -> %d\n", *buffer, rect.x, new_alias->rect.x, rect.y, new_alias->rect.y);

    new_alias->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &new_alias->rect, 0, 0, *buffer);

    // grab last message height
    if (tail->ptr != NULL) {
        alias_ptr = tail->ptr;

        // grab (y + height) value of last message
        rect.y = alias_ptr->rect.y + alias_ptr->rect.h + 5;
    }

    printf("code: %d\n", code);

    // incoming from opponent
    if (code == SV_LOBBY_POST_MESG) new_alias->rect.x = rect.x;
    else new_alias->rect.x = alias->rect.x + alias->rect.w - new_alias->rect.w;

    // apply height to message
    new_alias->rect.y = rect.y;

    pp4m_HOOK_Next(head, new_alias);
    return 0;
}

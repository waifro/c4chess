#include <stdio.h>
#include <stdlib.h>

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

int GUI_Alias_Textbox_DestrAlias(GUI_TextureAlias *alias_ptr) {

    if (alias_ptr->link != NULL)
        free(alias_ptr->link);

    if (alias_ptr->texture != NULL)
        SDL_DestroyTexture(alias_ptr->texture);

    return 0;
}

int GUI_Alias_Textbox_Init(GUI_TextureAlias *alias_ttr, char *buffer) {

    int buf_size = strlen(buffer) + 1;
    SDL_Rect rect;

    GUI_TextureAlias *alias_ptr = alias_ttr->link;

    char *link_ptr = malloc(sizeof(char) * buf_size);
    alias_ptr->link = link_ptr;

    for (int i = 0; i < buf_size; i++)
        link_ptr[i] = buffer[i];

    GUI_Alias_UpdateTextbox_Alias(alias_ttr, OPENSANS_REGULAR, PP4M_BLACK, 18, buffer);

    //alias_ptr->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_GREY_NORMAL, 18, &rect, 0, 0, keyb_buffer);

    return 0;
}

int GUI_Alias_UpdateTextbox_Alias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *keyb_buffer) {

    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    char *link_ptr = alias_ptr->link;
    SDL_Rect rect;

    if (strcmp(keyb_buffer, "") == 0) return -1;

    printf("test\n");

    if (alias_ptr->texture != NULL) {
        SDL_DestroyTexture(alias_ptr->texture);
        alias_ptr->texture = NULL;
    }

    alias_ptr->texture = pp4m_TTF_TextureFont(glo_render, pathname, color, point, &rect, 0, 0, keyb_buffer);

    alias_ptr->rect.x = 0;
    alias_ptr->rect.y = 0;
    alias_ptr->rect.w = rect.w;
    alias_ptr->rect.h = rect.h;

    if (rect.w > alias_ttr->rect.w) {
        alias_ptr->rect.x = rect.w - alias_ttr->rect.w;
        alias_ptr->rect.w = alias_ttr->rect.w;
    }

    return 0;
}

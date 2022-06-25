#ifndef GUI_ALIAS_H
#define GUI_ALIAS_H

#include <SDL2/SDL.h>
#include "../pp4m/pp4m_input.h"

/* structures & variables */

typedef enum {
    OBJ_NONE,

    OBJ_BUTTON_TXTBOX, // textbox
    OBJ_BUTTON_LINK_OFF, // indicates usage of *link
    OBJ_BUTTON_LINK_ON,

    OBJ_BUTTON_RETURN,
    OBJ_BUTTON_EXIT,

    OBJ_SCROLL_VERTICAL,
    OBJ_SCROLL_HORIZONTAL,

    OBJ_TEXTBOX_INPUT,

    OBJ_LINK_PTR // it automatically runs a new run of *link

} GUI_ALIAS_OBJ;

typedef struct {
    GUI_ALIAS_OBJ obj;
    void *link;
    SDL_Rect rect;
    SDL_Texture *texture;
} GUI_TextureAlias;

/* prototypes */
int GUI_Alias_InputOnObj(PP4M_INPUT_POS input, SDL_Rect rect);

int GUI_Alias_IsColor(SDL_Color *color);
int GUI_Alias_ResetColor(SDL_Color *color);

SDL_Texture *GUI_Alias_CreateSnapshot(SDL_Renderer *renderer, int width, int height);

#endif

#ifndef GUI_ALIAS_H
#define GUI_ALIAS_H

#include <SDL2/SDL.h>

/* structures & variables */

typedef struct {
    int x, y, w, h, z, j; // z & j are only for arithmetic calc.
}   GUI_RectAlias;

typedef struct {
    GUI_RectAlias rect_als;
    SDL_Rect rect;
    SDL_Texture *texture;
} GUI_TextureAlias;

typedef enum {
    EMPTY,
    FULL
} GUI_TextureFlagAlias;

/* prototypes */
void GUI_TextureAlias_InitRect(GUI_TextureAlias *source, int x, int y, int w, int h, GUI_TextureFlagAlias flag);

#endif

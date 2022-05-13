#ifndef GUI_ALIAS_H
#define GUI_ALIAS_H

#include <SDL2/SDL.h>

/* structures & variables */

typedef struct {
    int z, j;  // z & j are only for arithmetic calc.
    SDL_Rect rect;
    SDL_Texture *texture;
} GUI_TextureAlias;


/* prototypes */
//GUI_TextureAlias *GUI_TextureAlias_InitRect(int x, int y, int w, int h, GUI_TextureFlagAlias flag);

#endif

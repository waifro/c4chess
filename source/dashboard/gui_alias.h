#ifndef GUI_ALIAS_H
#define GUI_ALIAS_H

#include <SDL2/SDL.h>

/* structures & variables */

typedef struct {
    int obj;
    SDL_Rect rect;
    SDL_Texture *texture;
} GUI_TextureAlias;


/* prototypes */
SDL_Texture *GUI_Alias_CreateSnapshot(SDL_Renderer *renderer, int width, int height);

#endif

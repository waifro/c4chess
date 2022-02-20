/* Private Project Four Me */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// local headers
#include "pp4m.h"
#include "pp4m_io.h"
#include "pp4m_image.h"

void pp4m_IMG_Init(void) {

    IMG_Init(IMG_INIT_PNG);

    return;
}

void pp4m_IMG_Quit(void) {

    IMG_Quit();

    return;

}

SDL_Texture *pp4m_IMG_ImageToTexture(SDL_Renderer *renderer, SDL_Texture *texture, char *path, SDL_Rect *rect, float x, float y, int w, int h) {

    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }

    SDL_Surface *surface = IMG_Load(path);
    
    if (surface == NULL)
    {
        pp4m_IO_Feedback("feedback.txt", SDL_GetError());
        return NULL;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL)
    {
        pp4m_IO_Feedback("feedback.txt", SDL_GetError());
        return NULL;
    }

    SDL_FreeSurface(surface);

    if (x != 0 || y != 0 || w != 0 || h != 0) {

        rect->x = (int)x;
        rect->y = (int)y;

        if (w == 0 || h == 0) SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
        else {
            rect->w = (int)w;
            rect->h = (int)h;
        }

    }

    return texture;
}

/* 2022 @waifro */
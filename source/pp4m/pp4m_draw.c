/* Private Project Four Me */

#include <SDL2/SDL.h>
#include "pp4m_draw.h"

SDL_Texture *pp4m_DRAW_CreateTexture(SDL_Renderer *renderer, int width, int height) {

    SDL_Texture *texture;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

void pp4m_DRAW_TextureDrawLine(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, PP4M_PointToPoint *ptp, int x, int y, int x2, int y2) {

    SDL_SetRenderTarget(renderer, texture);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (ptp != NULL) SDL_RenderDrawLine(renderer, ptp->x, ptp->y, ptp->x2, ptp->y2);
    else SDL_RenderDrawLine(renderer, x, y, x2, y2);

    SDL_SetRenderTarget(renderer, NULL);

    return;
}

void pp4m_DRAW_TextureDrawPoint(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, int x, int y, int radius) {

    PP4M_PointToPoint ptp;

    ptp.x = x - radius;
    ptp.y = y;
    ptp.x2 = x - radius;
    ptp.y2 = y;

    SDL_SetRenderTarget(renderer, texture);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    for (int n = 0; n < radius; n++) {
        SDL_RenderDrawLine(renderer, ptp.x, ptp.y, ptp.x2, ptp.y2);
        ptp.x+=1; ptp.x2+=1;
        ptp.y-=1; ptp.y2+=1;
    }

    for (int n = 0; n < radius; n++) {
        SDL_RenderDrawLine(renderer, ptp.x, ptp.y, ptp.x2, ptp.y2);
        ptp.x+=1; ptp.x2+=1;
        ptp.y+=1; ptp.y2-=1;
    }

    SDL_SetRenderTarget(renderer, NULL);

    return;
}

SDL_Texture *pp4m_DRAW_TextureInitColor(SDL_Renderer *renderer, SDL_Color color, SDL_Rect *rect, float x, float y, float w, float h) {

    SDL_Surface surface;
    SDL_Texture *texture = NULL;

    memcpy(&surface, (void*)SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0), sizeof(surface));

    rect->x = 0;
    rect->y = 0;
    rect->w = (int)w;
    rect->h = (int)h;

    SDL_FillRect(&surface, rect, SDL_MapRGB(surface.format, color.r, color.g, color.b));

    rect->x = (int)x;
    rect->y = (int)y;

    texture = SDL_CreateTextureFromSurface(renderer, &surface);

    SDL_FreeSurface(&surface);
    return texture;

}

/* 2021 @richardwaifro */

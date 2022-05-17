/* Private Project Four Me */

#include <SDL2/SDL.h>
#include "pp4m_draw.h"

SDL_Rect pp4m_DRAW_InitRect(int x, int y, int w, int h) {

    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    return (rect);
}

SDL_Texture *pp4m_DRAW_CreateTexture(SDL_Renderer *renderer, int width, int height) {

    SDL_Texture *texture;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

void pp4m_DRAW_SetRenderColor(SDL_Renderer *renderer, SDL_Color *color) {
    if (color != NULL) SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
    else SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    return;
}

int pp4m_DRAW_TextureDrawLine(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, PP4M_PointToPoint *ptp, int x, int y, int x2, int y2) {
    int result = 0;
    SDL_SetRenderTarget(renderer, texture);

    pp4m_DRAW_SetRenderColor(renderer, &color);

    if (ptp != NULL) result = SDL_RenderDrawLine(renderer, ptp->x, ptp->y, ptp->x2, ptp->y2);
    else result = SDL_RenderDrawLine(renderer, x, y, x2, y2);

    pp4m_DRAW_SetRenderColor(renderer, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    return (result);
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

void pp4m_DRAW_TextureDrawCircle(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, int circle_center_x, int circle_center_y, int radius) {

    int x = (radius - 1);
    int y = 0;

    int tx = 1; int ty = 1;

    int diameter = (radius * 2);
    int error = (tx - diameter);

    SDL_SetRenderTarget(renderer, texture);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    while (x >= y) {

        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, circle_center_x + x, circle_center_y - y);
        SDL_RenderDrawPoint(renderer, circle_center_x + x, circle_center_y + y);
        SDL_RenderDrawPoint(renderer, circle_center_x - x, circle_center_y - y);
        SDL_RenderDrawPoint(renderer, circle_center_x - x, circle_center_y + y);
        SDL_RenderDrawPoint(renderer, circle_center_x + y, circle_center_y - x);
        SDL_RenderDrawPoint(renderer, circle_center_x + y, circle_center_y + x);
        SDL_RenderDrawPoint(renderer, circle_center_x - y, circle_center_y - x);
        SDL_RenderDrawPoint(renderer, circle_center_x - y, circle_center_y + x);

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);

    return;
}

int pp4m_DRAW_TextureDrawCircle_Filled(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    SDL_SetRenderTarget(renderer, texture);
    pp4m_DRAW_SetRenderColor(renderer, &color);

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    pp4m_DRAW_SetRenderColor(renderer, NULL);

    return status;
}

SDL_Texture *pp4m_DRAW_TextureInitColor_Target(SDL_Renderer *renderer, SDL_Color color, int alpha, SDL_Rect *rect, float x, float y, float w, float h) {

    SDL_Texture *texture = NULL;

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_SetRenderTarget(renderer, texture);

    if (rect != NULL) {
        rect->x = 0;
        rect->y = 0;
        rect->w = (int)w;
        rect->h = (int)h;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (rect != NULL)
        SDL_RenderFillRect(renderer, rect);
    else
        SDL_RenderFillRect(renderer, NULL);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

    if (rect != NULL) {
        rect->x = (int)x;
        rect->y = (int)y;
    }

    SDL_SetRenderTarget(renderer, NULL);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, alpha);

    return texture;

}

SDL_Texture *pp4m_DRAW_TextureInitColor(SDL_Renderer *renderer, SDL_Color color, SDL_Rect *rect, float x, float y, float w, float h) {

    SDL_Surface *surface = NULL;
    SDL_Texture *texture = NULL;

    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);

    rect->x = 0;
    rect->y = 0;
    rect->w = (int)w;
    rect->h = (int)h;

    SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, color.r, color.g, color.b));

    rect->x = (int)x;
    rect->y = (int)y;

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);
    return texture;

}

/* 2022 @waifro */

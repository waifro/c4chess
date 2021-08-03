#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"

#include "../global.h"
#include "gui.h"

/*
SDL_Texture *GUI_PopupWindow_Title(char title[256], SDL_Rect window_size) {
    if (sizeof(title) > 255) return;

    SDL_Texture *texture_title = NULL;
    // missing ttf font

    return texture_title;
}
*/

GUI_TextureAlias GUI_CreateTexture_BackgroundPolarize(void) {

    // initializing variables
    GUI_TextureAlias background;
    GUI_TextureAlias_InitRect(&background, 0, 0, 1280, 720, EMPTY);
    background.texture = pp4m_DRAW_TextureRect(glo_renderw, PP4M_GREY, &background.rect, 0, 0, glo_screen_w, glo_screen_h);

    // blending the texture for trasparent filter
    SDL_SetTextureBlendMode(background.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(background.texture, 100);

    return (background);
}

GUI_TextureAlias GUI_CreateTexture_ButtonExit(int x, int y) {

    // initializing variables
    GUI_TextureAlias button_exit;
    GUI_TextureAlias_InitRect(&button_exit, 0, 0, 50, 50, FULL);

    // accessing the texture modulation
    button_exit.texture = SDL_CreateTexture(glo_renderw, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, button_exit.rect.w, button_exit.rect.h);

    // targeting texture for drawing
    SDL_SetRenderTarget(glo_renderw, button_exit.texture);

    // drawing lines
    SDL_RenderDrawLine(glo_renderw, (button_exit.rect_als.x-5), (button_exit.rect_als.y-5), (button_exit.rect_als.z-5), (button_exit.rect_als.j-5));
    SDL_RenderDrawLine(glo_renderw, (button_exit.rect_als.z-5), (button_exit.rect_als.y-5), (button_exit.rect_als.x-5), (button_exit.rect_als.j-5));

    // reset targeting to render
    SDL_SetRenderTarget(glo_renderw, NULL);

    memcpy(&button_exit.rect_als.x, &x, sizeof(x));
    memcpy(&button_exit.rect_als.y, &y, sizeof(y));

    return (button_exit);
}

int GUI_PopupWindow_Core(int x, int y, int w, int h, char *text) {

    // background cloudy/blurred/polarized
    GUI_TextureAlias BackgroundPolar;
    BackgroundPolar = GUI_CreateTexture_BackgroundPolarize();

    // popup window
    GUI_TextureAlias PopupWindow;
    GUI_TextureAlias_InitRect(&PopupWindow, x, y, w, h, FULL);
    PopupWindow.texture = pp4m_DRAW_TextureRect(glo_renderw, PP4M_WHITE, &PopupWindow.rect, x, y, w, h);

    // button exit
    GUI_TextureAlias ButtonExit;
    ButtonExit = GUI_CreateTexture_ButtonExit((PopupWindow.rect_als.z-10), (PopupWindow.rect_als.j-10));

    SDL_Event event;

    while(event.type != SDL_QUIT) {

        SDL_PollEvent(&event);

        SDL_RenderClear(glo_renderw);
        SDL_RenderCopy(glo_renderw, BackgroundPolar.texture, NULL, &BackgroundPolar.rect);
        SDL_RenderCopy(glo_renderw, PopupWindow.texture, NULL, &PopupWindow.rect);
        SDL_RenderCopy(glo_renderw, ButtonExit.texture, NULL, &ButtonExit.rect);
        SDL_RenderPresent(glo_renderw);

    }

    // missing exiting animation

    SDL_DestroyTexture(BackgroundPolar.texture);
    SDL_DestroyTexture(PopupWindow.texture);
    SDL_DestroyTexture(ButtonExit.texture);

    return 0;
}

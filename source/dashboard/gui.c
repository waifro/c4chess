#include <string.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"

#include "../global.h"
#include "gui.h"

SDL_Texture *GUI_PopupWindow_Title(char title[256], SDL_Rect *rect, SDL_Rect window_pos) {
    if (strlen(title) > 255) return (NULL);

    SDL_Texture *texture_title = NULL;

    texture_title = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_BLACK, 24, rect, window_pos.x, window_pos.y, title);

    return (texture_title);
}

GUI_TextureAlias GUI_CreateTexture_BackgroundPolarize(void) {

    // initializing variables
    GUI_TextureAlias background;
    GUI_TextureAlias_InitRect(&background, 0, 0, 1280, 720, FULL);
    background.texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY, &background.rect, 0, 0, glo_screen_w, glo_screen_h);

    // blending the texture for trasparent filter
    //SDL_SetTextureBlendMode(background.texture, SDL_BLENDMODE_BLEND);
    //SDL_SetTextureAlphaMod(background.texture, 100);

    return (background);
}

GUI_TextureAlias GUI_CreateTexture_ButtonExit(int x, int y) {

    // initializing variables
    GUI_TextureAlias button_exit;
    PP4M_PointToPoint ptp1 = {5, 5, 45, 45};
    PP4M_PointToPoint ptp2 = {45, 5, 5, 45};

    GUI_TextureAlias_InitRect(&button_exit, x, y, 50, 50, FULL);

    button_exit.texture = pp4m_DRAW_CreateTexture(glo_render, button_exit.rect.w, button_exit.rect.h);

    SDL_SetRenderTarget(glo_render, button_exit.texture);

    pp4m_DRAW_SetRenderColor(glo_render, PP4M_RED);
    SDL_RenderFillRect(glo_render, NULL);

    SDL_SetRenderTarget(glo_render, NULL);

    pp4m_DRAW_TextureDrawLine(glo_render, button_exit.texture, PP4M_WHITE, &ptp1, 0, 0, 0, 0);
    pp4m_DRAW_TextureDrawLine(glo_render, button_exit.texture, PP4M_WHITE, &ptp2, 0, 0, 0, 0);

    return (button_exit);
}

int GUI_PopupWindow_Core(int x, int y, int w, int h, char *title) {
    SDL_Event event;

    // background cloudy/blurred/polarized
    GUI_TextureAlias BackgroundPolar;
    BackgroundPolar = GUI_CreateTexture_BackgroundPolarize();

    // popup window
    GUI_TextureAlias PopupWindow;
    GUI_TextureAlias_InitRect(&PopupWindow, x, y, w, h, FULL);
    PopupWindow.texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &PopupWindow.rect, x, y, w, h);

    GUI_TextureAlias TextureTitle;
    TextureTitle.texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_RED, 24, &TextureTitle.rect, PopupWindow.rect.x, PopupWindow.rect.y, title);
    //TextureTitle.texture = GUI_PopupWindow_Title(title, &TextureTitle.rect, PopupWindow.rect);

    // button exit
    GUI_TextureAlias ButtonExit;
    int foo = PopupWindow.rect_als.z - 60;
    int bar = PopupWindow.rect_als.j - 60;

    ButtonExit = GUI_CreateTexture_ButtonExit(foo, bar);

    while(event.type != SDL_QUIT) {

        SDL_PollEvent(&event);

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, BackgroundPolar.texture, NULL, &BackgroundPolar.rect);
        SDL_RenderCopy(glo_render, PopupWindow.texture, NULL, &PopupWindow.rect);
        SDL_RenderCopy(glo_render, TextureTitle.texture, NULL, &TextureTitle.rect);
        SDL_RenderCopy(glo_render, ButtonExit.texture, NULL, &ButtonExit.rect);
        SDL_RenderPresent(glo_render);

    }

    // missing exiting animation

    SDL_DestroyTexture(BackgroundPolar.texture);
    SDL_DestroyTexture(PopupWindow.texture);
    SDL_DestroyTexture(TextureTitle.texture);
    SDL_DestroyTexture(ButtonExit.texture);

    return 0;
}

void GUI_Testing(void) {

    SDL_Texture *texture = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100, 100);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, 25);

    SDL_Rect rect = { (glo_screen_w/2-50), (glo_screen_h/2-50), 100, 100};

    //pp4m_DRAW_TextureDrawCircle_Filled(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Color color, int circle_center_x, int circle_center_y, int radius)
    pp4m_DRAW_TextureDrawCircle_Filled(glo_render, texture, PP4M_RED, (rect.w / 2), (rect.h / 2), 50);

    SDL_Event event;
    while(1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, texture, NULL, &rect);
        SDL_RenderPresent(glo_render);

    }

    return;
}

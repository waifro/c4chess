#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "../chess/core.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"

#include "../global.h"
#include "gui.h"

SDL_Texture *GUI_PopupWindow_Title(char *title, SDL_Rect *rect, SDL_Color color, SDL_Rect window_pos) {
    if (strlen(title) > 255) return (NULL);

    SDL_Texture *texture_title = NULL;
    texture_title = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, color, 24, rect, 0, 0, title);

    int w = 0, h = 0;
    SDL_QueryTexture(texture_title, NULL, NULL, &w, &h);

    rect->x = (window_pos.w / 2) - (w / 2) + window_pos.x;
    rect->y = window_pos.y;

    return (texture_title);
}

GUI_TextureAlias GUI_CreateTexture_BackgroundInit(SDL_Color color) {
    // initializing variables
    GUI_TextureAlias background;
    GUI_TextureAlias_InitRect(&background, 0, 0, 1280, 720, FULL);
    background.texture = pp4m_DRAW_TextureInitColor(glo_render, color, &background.rect, 0, 0, glo_screen_w, glo_screen_h);

    return (background);
}

GUI_TextureAlias GUI_CreateTexture_BackgroundPolarize(SDL_Color color, int alpha) {

    // initializing variables
    GUI_TextureAlias background;
    GUI_TextureAlias_InitRect(&background, 0, 0, 1280, 720, FULL);
    background.texture = pp4m_DRAW_TextureInitColor(glo_render, color, &background.rect, 0, 0, glo_screen_w, glo_screen_h);

    // blending the texture for trasparent filter
    SDL_SetTextureBlendMode(background.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(background.texture, alpha);

    return (background);
}

GUI_TextureAlias GUI_CreateTexture_Button(char *title, SDL_Color color, int x, int y, int w, int h) {

    // initializing variables
    GUI_TextureAlias button;

    button.texture = pp4m_DRAW_TextureInitColor_Target(glo_render, color, &button.rect, x, y, w, h);
    //button.texture = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 150, 150);

    SDL_Texture *texture = NULL; SDL_Rect rect;


    SDL_SetRenderTarget(glo_render, button.texture);

    texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, color, 24, &rect, x, y, title);
    SDL_RenderCopy(glo_render, texture, NULL, &rect);

    SDL_SetRenderTarget(glo_render, NULL);

    SDL_DestroyTexture(texture);

    //button.rect.x = x;
    //button.rect.y = y;

    return (button);
}

int GUI_PopupWindow_Core(PP4M_HOOK *list_hook, int x, int y, int w, int h, char *title) {
    SDL_Event event;

    // background cloudy/blurred/polarized
    GUI_TextureAlias BackgroundPolar;
    BackgroundPolar = GUI_CreateTexture_BackgroundPolarize(PP4M_BLACK, 150);

    // popup window
    GUI_TextureAlias PopupWindow;
    GUI_TextureAlias_InitRect(&PopupWindow, x, y, w, h, FULL);
    PopupWindow.texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &PopupWindow.rect, x, y, w, h);

    GUI_TextureAlias TextureTitle;
    TextureTitle.texture = GUI_PopupWindow_Title(title, &TextureTitle.rect, PP4M_WHITE, PopupWindow.rect);

    // button continue
    GUI_TextureAlias ButtonContinue;
    ButtonContinue = GUI_CreateTexture_Button("Continue", PP4M_GREY_HEAVY, PopupWindow.rect_als.x + 10, PopupWindow.rect_als.y + 10, 420, 75);

    // button exit
    GUI_TextureAlias ButtonExit;
    ButtonExit = GUI_CreateTexture_Button("Exit", PP4M_GREY_HEAVY, PopupWindow.rect_als.x + 10, PopupWindow.rect_als.y + 95, 420, 75);

    while(1) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;

        SDL_RenderClear(glo_render);

        int val = pp4m_HOOK_Size(list_hook);
        PP4M_HOOK *current = list_hook;
        CHESS_CORE_TILE *hook_tile = NULL;

        for (int n = 0; n <= val; n++) {

            if (n == 0) {
                SDL_RenderCopy(glo_render, list_hook->ptr, NULL, NULL);
                hook_tile= current->ptr;
                current = current->next;
                continue;
            }

            hook_tile= current->ptr;
            current = current->next;

            SDL_RenderCopy(glo_render, hook_tile->texture, NULL, &hook_tile->rect);
            if (hook_tile->piece != NULL) SDL_RenderCopy(glo_render, hook_tile->piece->texture, NULL, &hook_tile->rect);
        }

        SDL_RenderCopy(glo_render, BackgroundPolar.texture, NULL, &BackgroundPolar.rect);
        SDL_RenderCopy(glo_render, PopupWindow.texture, NULL, &PopupWindow.rect);
        SDL_RenderCopy(glo_render, TextureTitle.texture, NULL, &TextureTitle.rect);
        SDL_RenderCopy(glo_render, ButtonContinue.texture, NULL, &ButtonContinue.rect);
        SDL_RenderCopy(glo_render, ButtonExit.texture, NULL, &ButtonExit.rect);
        SDL_RenderPresent(glo_render);

    }

    // missing exiting animation

    SDL_DestroyTexture(BackgroundPolar.texture);
    SDL_DestroyTexture(PopupWindow.texture);
    SDL_DestroyTexture(TextureTitle.texture);
    SDL_DestroyTexture(ButtonExit.texture);
    SDL_DestroyTexture(ButtonContinue.texture);

    return 0;
}

void GUI_Testing(void) {

    SDL_Texture *texture = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100, 100);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, 100);

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

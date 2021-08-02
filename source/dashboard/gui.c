#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"

#include "../global.h"
#include "gui.h"

int GUI_PopupWindow(int x, int y, int w, int h, char *text) {

    printf("Hello");

    // background blended
    // create an external function
    SDL_Texture *texture_background = NULL;
    SDL_Rect rect_background;
    texture_background = pp4m_DRAW_TextureRect(glo_renderw, PP4M_GREY, &rect_background, 0, 0, glo_screen_w, glo_screen_h);
    SDL_SetTextureBlendMode(texture_background, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture_background, 100);

    // popup window
    // create an external function
    SDL_Texture *texture_window = NULL;
    SDL_Rect rect_window;
    texture_window = pp4m_DRAW_TextureRect(glo_renderw, PP4M_WHITE, &rect_window, x, y, w, h);

    // exit button
    // create an external function
    SDL_Texture *texture_button_exit = NULL;
    SDL_Rect rect_button_exit;
    texture_button_exit = pp4m_DRAW_TextureRect(glo_renderw, PP4M_RED, &rect_button_exit, (rect_window.x+rect_window.w-75), (rect_window.y+rect_window.h-75), 50, 50);

    while(1) {
        if (1==0) break;
        SDL_RenderClear(glo_renderw);
        SDL_RenderCopy(glo_renderw, texture_background, NULL, &rect_background);
        SDL_RenderCopy(glo_renderw, texture_window, NULL, &rect_window);
        SDL_RenderCopy(glo_renderw, texture_button_exit, NULL, &rect_button_exit);
        SDL_RenderPresent(glo_renderw);
    }

    // missing exiting animation

    SDL_DestroyTexture(texture_background);
    SDL_DestroyTexture(texture_window);
    SDL_DestroyTexture(texture_button_exit);

    return 0;
}

SDL_Texture *GUI_PopupWindow_Title(char title[256], SDL_Rect window_size) {
    if (sizeof(title) > 255) return;

    SDL_Texture *texture_title = NULL;
    // missing ttf font

    return texture_title;
}

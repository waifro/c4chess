#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "../chess/core.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_input.h"

#include "../global.h"
#include "gui.h"

int GUI_PopupWindow_Title(PP4M_HOOK *head, char *path, char *title, SDL_Color color, int point) {

    GUI_TextureAlias *rect_pw = head->next->ptr;
    int x = rect_pw->rect.x + rect_pw->rect.w / 2;
    int y = rect_pw->rect.y + 20;

    GUI_TextureAlias *txr_alias = calloc(1, sizeof(GUI_TextureAlias));
    txr_alias->obj = 0;
    txr_alias->texture = pp4m_TTF_TextureFont(glo_render, path, color, point, &txr_alias->rect, x, y, title);

    txr_alias->rect.x = x - txr_alias->rect.w / 2;

    pp4m_HOOK_Next(head, txr_alias);
    return (0);
}

int GUI_PopupWindow_Button(PP4M_HOOK *head, char *path, int FLAG_OBJ, char *title, SDL_Color color_text, int point, SDL_Color color_button, int x_pp, int y_pp, int w, int h) {
    if (head == NULL) return (-1);

    GUI_TextureAlias *rect_pw = head->next->ptr;

    int x = rect_pw->rect.x + x_pp;
    int y = rect_pw->rect.y + y_pp;

    /* initializing variables */
    // _btx is button

    GUI_TextureAlias *txr_alias = malloc(sizeof(GUI_TextureAlias));
    txr_alias->obj = FLAG_OBJ;

    txr_alias->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, color_button, 255, &txr_alias->rect, x, y, w, h);

    SDL_SetRenderTarget(glo_render, txr_alias->texture);

    // _fnt is font
    SDL_Rect rect_fnt;
    SDL_Texture *texture_fnt = NULL;
    texture_fnt = pp4m_TTF_TextureFont(glo_render, path, color_text, point, &rect_fnt, 0, 0, title);

    int w_text = 0;
    int h_text = 0;
    SDL_QueryTexture(texture_fnt, NULL, NULL, &w_text, &h_text);

    rect_fnt.x = (w / 2) - (w_text / 2);
    rect_fnt.y = (h / 2) - (h_text / 2);

    SDL_RenderCopy(glo_render, texture_fnt, NULL, &rect_fnt);

    SDL_SetRenderTarget(glo_render, NULL);

    SDL_DestroyTexture(texture_fnt);
    texture_fnt = NULL;

    pp4m_HOOK_Next(head, txr_alias);
    return (0);
}

PP4M_HOOK *GUI_PopupWindow_Init(int w, int h) {

    int x = (glo_screen_w / 2) - (w / 2);
    int y = (glo_screen_h / 2) - (h / 2);

    // background cloudy/blurred/polarized
    GUI_TextureAlias *background = calloc(1, sizeof(GUI_TextureAlias));
    background->obj = 0;
    background->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_BLACK, 150, &background->rect, 0, 0, glo_screen_w, glo_screen_h);

    // popup window
    GUI_TextureAlias *popupWindow = calloc(1, sizeof(GUI_TextureAlias));
    popupWindow->obj = 0;
    popupWindow->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_GREY_HEAVY, 255, &popupWindow->rect, x, y, w, h);

    PP4M_HOOK *head = NULL;
    head = pp4m_HOOK_Init();

    pp4m_HOOK_Next(head, background);
    pp4m_HOOK_Next(head, popupWindow);

    return (head);
}

int GUI_PopupWindow_Core(PP4M_HOOK *head, SDL_Texture *background) {

    PP4M_HOOK *current = head;
    int val = pp4m_HOOK_Size(head);

    GUI_TextureAlias *txr_alias = NULL;
    SDL_Color color_btn_bak;

    SDL_Event event;
    int result = 0;

    PP4M_INPUT_POS input;

    while(result == 0) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) result = -3;

        pp4m_INPUT_GetMouseState(&event, &input);

        current = head;

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, background, NULL, NULL);

        for (int n = 0; n <= val; n++) {

            txr_alias = current->ptr;
            current = current->next;
            SDL_GetTextureColorMod(txr_alias->texture, &color_btn_bak.r, &color_btn_bak.g, &color_btn_bak.b);

            if (input.iner == 1) {
                if (txr_alias->obj != 0 && input.x >= txr_alias->rect.x && input.x <= (txr_alias->rect.x + txr_alias->rect.w) &&
                    input.y >= txr_alias->rect.y && input.y <= (txr_alias->rect.y + txr_alias->rect.h)) {
                    if (txr_alias->obj == -1) result = -1;
                    else if (txr_alias->obj == -2) result = -2;
                }
            }

            if (txr_alias->obj != 0 && input.x >= txr_alias->rect.x && input.x <= (txr_alias->rect.x + txr_alias->rect.w) &&
                input.y >= txr_alias->rect.y && input.y <= (txr_alias->rect.y + txr_alias->rect.h)) {
                SDL_SetTextureColorMod(txr_alias->texture, 220, 220, 220);
            }

            SDL_RenderCopy(glo_render, txr_alias->texture, NULL, &txr_alias->rect);
            SDL_SetTextureColorMod(txr_alias->texture, color_btn_bak.r, color_btn_bak.g, color_btn_bak.b);
        }

        SDL_RenderPresent(glo_render);
    }

    // missing exiting animation
    for (int n = 0; n <= val; n++)
        pp4m_HOOK_Remove(head);

    return (result);
}

void GUI_Testing(void) {

    SDL_Texture *roundpw_txr = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, glo_screen_w, glo_screen_h);

    SDL_Rect draw_rect = { 0, 0, 100, 100};

    SDL_Texture *draw_txr = SDL_CreateTexture(glo_render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100, 100);

    pp4m_DRAW_TextureDrawCircle_Filled(glo_render, draw_txr, PP4M_WHITE, draw_rect.w / 2, draw_rect.h / 2, 50);

    SDL_SetRenderTarget(glo_render, roundpw_txr);

    SDL_RenderCopy(glo_render, draw_txr, NULL, &draw_rect);
    draw_rect.x = 0;
    draw_rect.y = 400;
    SDL_RenderCopy(glo_render, draw_txr, NULL, &draw_rect);
    draw_rect.x = 550;
    draw_rect.y = 0;
    SDL_RenderCopy(glo_render, draw_txr, NULL, &draw_rect);
    draw_rect.x = 550;
    draw_rect.y = 400;
    SDL_RenderCopy(glo_render, draw_txr, NULL, &draw_rect);

    //SDL_SetRenderTarget(glo_render, draw_txr);
    //SDL_RenderClear(glo_render);

    pp4m_DRAW_SetRenderColor(glo_render, &PP4M_WHITE);

    draw_rect.x = 0;
    draw_rect.y = 50;
    draw_rect.w = 651;
    draw_rect.h = 400;

    SDL_RenderFillRect(glo_render, &draw_rect);

    draw_rect.x = 50;
    draw_rect.y = 0;
    draw_rect.w = 550;
    draw_rect.h = 501; // might be the cause of radius -1?

    SDL_RenderFillRect(glo_render, &draw_rect);

    SDL_SetRenderTarget(glo_render, NULL);
    pp4m_DRAW_SetRenderColor(glo_render, NULL);

    // moving the texture a bit away from the screen
    draw_rect.x = 50;
    draw_rect.y = 50;
    draw_rect.w = 1280;
    draw_rect.h = 720;

    SDL_Event event;
    while(1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, roundpw_txr, NULL, &draw_rect);
        SDL_RenderPresent(glo_render);

    }

    return;
}

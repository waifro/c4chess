#include <stdio.h>
#include <SDL2/SDL.h>

#include "../chess/middle.h"
#include "popup.h"
#include "gui.h"
#include "gui_alias.h"

#include "../global.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"

PP4M_HOOK *GUI_PopupWindow_Init(int w, int h) {

    int x = (glo_screen_w / 2) - (w / 2);
    int y = (glo_screen_h / 2) - (h / 2);

    // background cloudy/blurred/polarized
    GUI_TextureAlias *background = calloc(1, sizeof(GUI_TextureAlias));
    background->obj = 0;
    background->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_BLACK, 150, &background->dst_rect, 0, 0, glo_screen_w, glo_screen_h);

    // popup window
    GUI_TextureAlias *popupWindow = calloc(1, sizeof(GUI_TextureAlias));
    popupWindow->obj = 0;
    popupWindow->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_GREY_HEAVY, 255, &popupWindow->dst_rect, x, y, w, h);

    PP4M_HOOK *head = NULL;
    head = pp4m_HOOK_Init();

    pp4m_HOOK_Next(head, background);
    pp4m_HOOK_Next(head, popupWindow);

    return (head);
}

int GUI_PopupWindow_Title(PP4M_HOOK *head, char *path, char *title, SDL_Color color, int point) {

    GUI_TextureAlias *rect_pw = head->next->ptr;
    int x = rect_pw->dst_rect.x + rect_pw->dst_rect.w / 2;
    int y = rect_pw->dst_rect.y + 20;

    GUI_TextureAlias *txr_alias = calloc(1, sizeof(GUI_TextureAlias));
    txr_alias->obj = 0;
    txr_alias->texture = pp4m_TTF_TextureFont(glo_render, path, color, point, &txr_alias->dst_rect, x, y, title);

    txr_alias->dst_rect.x = x - txr_alias->dst_rect.w / 2;

    pp4m_HOOK_Next(head, txr_alias);
    return (0);
}

int GUI_PopupWindow_Button(PP4M_HOOK *head, char *path, int OBJ_FLAG, char *title, SDL_Color color_text, int point, SDL_Color color_button, int x_pp, int y_pp, int w, int h) {
    if (head == NULL) return (-1);

    GUI_TextureAlias *rect_pw = head->next->ptr;

    int x = rect_pw->dst_rect.x + x_pp;
    int y = rect_pw->dst_rect.y + y_pp;

    /* initializing variables */
    // _btx is button

    GUI_TextureAlias *txr_alias = malloc(sizeof(GUI_TextureAlias));
    txr_alias->obj = OBJ_FLAG;

    txr_alias->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, color_button, 255, &txr_alias->dst_rect, x, y, w, h);
    
    GUI_Alias_WriteFontOnTop(txr_alias, path, color_text, point, title);

    pp4m_HOOK_Next(head, txr_alias);
    return (0);
}

int GUI_PopupWindow_Core(PP4M_HOOK *head, SDL_Texture *background) {

    PP4M_HOOK *current = head;
    int val = pp4m_HOOK_Size(head);

    GUI_TextureAlias *txr_alias = NULL;
    SDL_Color color_btn_bak;

    SDL_Event event;
    int result = 0;

    PP4M_INPUT_POS input = pp4m_INPUT_InitInputPos();

    while(result == 0) {

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) result = -3;

        pp4m_INPUT_GetMouseState(&event, &input);

        current = head;

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, background, NULL, NULL);

        for (int n = 0; n < val; n++) {

            txr_alias = current->ptr;
            current = current->next;
            SDL_GetTextureColorMod(txr_alias->texture, &color_btn_bak.r, &color_btn_bak.g, &color_btn_bak.b);

            if (txr_alias->obj != OBJ_NONE) {

                if (GUI_Alias_InputOnObj(input, txr_alias->dst_rect) == 1) {
                    SDL_SetTextureColorMod(txr_alias->texture, 220, 220, 220);

                    if (input.iner == 1) {

                        if (txr_alias->obj == OBJ_BUTTON_LINK_OFF)
                            txr_alias->obj = OBJ_BUTTON_LINK_ON;

                        else if (txr_alias->obj == OBJ_BUTTON_RETURN) result = -1;
                        else if (txr_alias->obj == OBJ_BUTTON_EXIT) result = -2;
                    }

                        /*
                        else {
                            // reset button presses
                            if (txr_alias->obj == OBJ_BUTTON_LINK_ON) txr_alias->obj = OBJ_BUTTON_LINK_OFF;
                        }
                        */
                }
            }

            SDL_RenderCopy(glo_render, txr_alias->texture, NULL, &txr_alias->dst_rect);
            SDL_SetTextureColorMod(txr_alias->texture, color_btn_bak.r, color_btn_bak.g, color_btn_bak.b);
        }

        SDL_RenderPresent(glo_render);
    }

    // missing exiting animation
    for (int n = 0; n < val; n++)
        pp4m_HOOK_Remove(head);

    return (result);
}

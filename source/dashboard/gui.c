#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "../security/debug.h"
#include "../chess/core.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "../global.h"
#include "gui.h"

#define TEXTURE_LOBBYCHAT "resources/logo_mesg.png"
#define TEXTURE_LOBBYCHAT_SEND "resources/logo_direct.png"

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

PP4M_HOOK *GUI_RenderWindow_Chat_Init(PP4M_HOOK *hook_list) {

    PP4M_HOOK *chat_ttr_list = pp4m_HOOK_Init();
    PP4M_HOOK *chat_init_list = pp4m_HOOK_Init();

    GUI_TextureAlias *alias_button_chat = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_button_chat->obj = OBJ_BUTTON_LINK_OFF;
    alias_button_chat->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT, &alias_button_chat->rect, 850, 600, 30, 30);

    GUI_TextureAlias *alias_window = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_window->obj = OBJ_NONE;
    alias_window->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &alias_window->rect, alias_button_chat->rect.x + 20, alias_button_chat->rect.y - 440, 300, 450);

    GUI_TextureAlias *alias_inner_w = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_inner_w->obj = OBJ_SCROLL_VERTICAL;
    alias_inner_w->texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &alias_inner_w->rect, alias_window->rect.x + 10, alias_window->rect.y + 10, alias_window->rect.w - 20, alias_window->rect.h - 50);

    GUI_Alias_InnerWindow_Init(alias_inner_w);

    GUI_TextureAlias *alias_textbox = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_textbox->obj = OBJ_TEXTBOX_ALIAS;
    alias_textbox->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_WHITE, 255, &alias_textbox->rect, alias_inner_w->rect.x, alias_inner_w->rect.y + alias_inner_w->rect.h + 5, alias_inner_w->rect.w - 30, 30);

    GUI_TextureAlias *alias_text = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));

    // save pointer of hooked Alias
    alias_textbox->link = alias_text;
    alias_text->obj = OBJ_TEXTBOX_INPUT_OFF;
    GUI_Alias_Textbox_InitAlias(alias_textbox, OPENSANS_REGULAR, PP4M_GREY_NORMAL, 18, "Input text here");

    GUI_TextureAlias *alias_button_send = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_button_send->obj = OBJ_BUTTON_TXTBOX;
    alias_button_send->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEXTURE_LOBBYCHAT_SEND, &alias_button_send->rect, alias_textbox->rect.x + alias_textbox->rect.w + 5, alias_textbox->rect.y, 30, 30);

    GUI_TextureAlias *alias_chat = (GUI_TextureAlias*)malloc(sizeof(GUI_TextureAlias));
    alias_chat->obj = OBJ_NULL;

    pp4m_HOOK_Next(chat_init_list, alias_button_chat);

    pp4m_HOOK_Next(chat_ttr_list, alias_window);
    pp4m_HOOK_Next(chat_ttr_list, alias_inner_w);
    pp4m_HOOK_Next(chat_ttr_list, alias_textbox);
    pp4m_HOOK_Next(chat_ttr_list, alias_button_send);
    pp4m_HOOK_Next(chat_ttr_list, alias_chat);

    // save pointer of hooked list (unrendered)
    alias_button_chat->link = chat_ttr_list;

    pp4m_HOOK_Next(hook_list, chat_init_list);

    return chat_ttr_list;
}

int GUI_HookLink_Render(PP4M_HOOK *link) {

    PP4M_HOOK *current = link;
    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    int val = pp4m_HOOK_Size(link);

    for (int i = 0; i < val; i++) {
        alias_ttr = current->ptr;
        current = current->next;

        if (alias_ttr->obj == OBJ_NULL) continue;
        SDL_RenderCopy(glo_render, alias_ttr->texture, NULL, &alias_ttr->rect);

        if (alias_ttr->obj == OBJ_LINK_PTR)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_SCROLL_VERTICAL)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_BUTTON_LINK_ON)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
            alias_ptr = alias_ttr->link; SDL_Rect rect;
            GUI_Alias_Textbox_UpdateRect(alias_ttr, &rect);
            SDL_RenderCopy(glo_render, alias_ptr->texture, &alias_ptr->rect, &rect);
        }
    }

    return 0;
}

int GUI_HookList_Render(PP4M_HOOK *hook_list) {
    int val = pp4m_HOOK_Size(hook_list);

    PP4M_HOOK *current = hook_list;
    PP4M_HOOK *curr_ptr = NULL;

    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        curr_ptr = current->ptr;
        current = current->next;
        int res = pp4m_HOOK_Size(curr_ptr);

        for (int n = 0; n < res; n++) {
            alias_ttr = curr_ptr->ptr;
            curr_ptr = curr_ptr->next;

            if (alias_ttr->obj == OBJ_NULL) continue;
            SDL_RenderCopy(glo_render, alias_ttr->texture, NULL, &alias_ttr->rect);

            if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
                alias_ptr = alias_ttr->link; SDL_Rect rect;
                GUI_Alias_Textbox_UpdateRect(alias_ttr, &rect);
                SDL_RenderCopy(glo_render, alias_ptr->texture, &alias_ptr->rect, &rect);
            }

            if (alias_ttr->obj == OBJ_LINK_PTR)
                GUI_HookLink_Render(alias_ttr->link);

            else if (alias_ttr->obj == OBJ_SCROLL_VERTICAL)
                GUI_HookLink_Render(alias_ttr->link);

            else if (alias_ttr->obj == OBJ_BUTTON_LINK_ON)
                GUI_HookLink_Render(alias_ttr->link);
        }
    }

    return 0;
}

void GUI_HookList_Quit(PP4M_HOOK *hook_list) {

    PP4M_HOOK *current = hook_list;
    PP4M_HOOK *curr_nxt = NULL;
    PP4M_HOOK *ptr_list = NULL;

    // take total main loop
    int val = pp4m_HOOK_Size(hook_list);

    for (int i = 0; i < val; i++) {
        curr_nxt = current;

        // grab last ptr of tail
        int ras = pp4m_HOOK_Size(curr_nxt);

        for (int n = 0; n < ras; n++) {
            ptr_list = curr_nxt->ptr;
            curr_nxt = curr_nxt->next;
        }

        int res = pp4m_HOOK_Size(ptr_list);

        // remove all last ptr -> hook
        for (int n = 0; n < res; n++)
            pp4m_HOOK_Remove(ptr_list);

        // remove tail
        pp4m_HOOK_Remove(current);
    }

    return;
}

int GUI_HookLink_Update(PP4M_HOOK *link, PP4M_INPUT_POS input, char **buffer, int key) {

    PP4M_HOOK *current = link;

    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    int val = pp4m_HOOK_Size(link);

    for (int i = 0; i < val; i++) {
        alias_ttr = current->ptr;
        current = current->next;

        if (alias_ttr->obj == OBJ_NULL) continue;

        // we should create a func for this
        if (input.iner == 1) {
            if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
                alias_ptr = alias_ttr->link;

                if (GUI_Alias_InputOnObj(input, alias_ttr->rect) == 1) alias_ptr->obj = OBJ_TEXTBOX_INPUT_ON;
                else alias_ptr->obj = OBJ_TEXTBOX_INPUT_OFF;
            }
        }

        if (alias_ttr->obj == OBJ_SCROLL_VERTICAL) {
            GUI_Alias_InnerWindow_Add(alias_ttr,  OPENSANS_REGULAR, PP4M_BLACK, 14, buffer);
        }

        if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
            alias_ptr = alias_ttr->link;

            if (alias_ptr->obj == OBJ_TEXTBOX_INPUT_ON)
                GUI_Alias_Textbox_UpdateAlias(alias_ttr, OPENSANS_REGULAR, PP4M_BLACK, 18, buffer, key);
        }
    }

    return 0;
}

int GUI_HookList_Update(PP4M_HOOK *hook_list, PP4M_INPUT_POS input, char **buffer, int key) {
    int result = 0;

    int val = pp4m_HOOK_Size(hook_list);

    SDL_Color color_btn_bak;
    GUI_Alias_ResetColor(&color_btn_bak);

    PP4M_HOOK *current = hook_list;
    PP4M_HOOK *curr_ptr = NULL;
    PP4M_HOOK *link_ptr = NULL;

    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        curr_ptr = current->ptr;
        current = current->next;
        int res = pp4m_HOOK_Size(curr_ptr);

        for (int n = 0; n < res; n++) {
            if (curr_ptr == NULL) continue;

            alias_ttr = curr_ptr->ptr;
            curr_ptr = curr_ptr->next;

            // hooked list update
            if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF || alias_ttr->obj == OBJ_BUTTON_LINK_ON)
                GUI_HookLink_Update(alias_ttr->link, input, buffer, key);

            if (alias_ttr->obj == OBJ_SCROLL_VERTICAL) {
                GUI_HookLink_Update(alias_ttr->link, input, buffer, key);
            }

            if (input.iner == 1) {
                if (GUI_Alias_InputOnObj(input, alias_ttr->rect) == 1) {

                    if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF) {
                        alias_ttr->obj = OBJ_BUTTON_LINK_ON;
                    }

                    else if (alias_ttr->obj == OBJ_BUTTON_RETURN) result = -1;
                    else if (alias_ttr->obj == OBJ_BUTTON_EXIT) result = -2;
                } else {
                    if (alias_ttr->obj == OBJ_BUTTON_LINK_ON) {
                        link_ptr = alias_ttr->link;
                        alias_ptr = link_ptr->ptr;

                        if (GUI_Alias_InputOnObj(input, alias_ptr->rect) != 1)
                            alias_ttr->obj = OBJ_BUTTON_LINK_OFF;
                    }
                }
            }

            /* attempt on highlighting objects failed :(

            else if (GUI_Alias_InputOnObj(input, alias_ttr->rect) == 1) {

                // mouse on top of object highlights it
                if (alias_ttr->obj != OBJ_NONE) {
                    if (GUI_Alias_IsColor(&color_btn_bak) == -1) SDL_GetTextureColorMod(alias_ttr->texture, &color_btn_bak.r, &color_btn_bak.g, &color_btn_bak.b);
                    SDL_SetTextureColorMod(alias_ttr->texture, 220, 220, 220);
                }

            } else if (GUI_Alias_InputOnObj(input, alias_ttr->rect) == -1) {

                // restores the object to original color
                if (GUI_Alias_IsColor(&color_btn_bak) == 1) {
                    SDL_SetTextureColorMod(alias_ttr->texture, color_btn_bak.r, color_btn_bak.g, color_btn_bak.b);
                    GUI_Alias_ResetColor(&color_btn_bak);
                }
            }
            */
        }
    }

    return result;
}

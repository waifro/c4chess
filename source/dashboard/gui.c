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
#include "ingame.h"
#include "gui.h"

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

int GUI_HookLink_RenderObj(GUI_TextureAlias *alias_ptr) {
    if (alias_ptr == NULL) return -1;

    if (alias_ptr->texture != NULL)
        SDL_RenderCopy(glo_render, alias_ptr->texture, NULL, &alias_ptr->dst_rect);

    switch(alias_ptr->obj) {
        default:
            break;
        case OBJ_LINK_PTR:
            GUI_HookLink_Render(alias_ptr->link);
            break;
        case OBJ_WINDOW:
            GUI_HookLink_Render(alias_ptr->link);
            break;
        case OBJ_WINDOW_CHAT:
            GUI_HookLink_Render(alias_ptr->link);
            break;
    }

    return 0;
}

int GUI_HookLink_Render(PP4M_HOOK *link) {

    PP4M_HOOK *current = link;
    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    int val = pp4m_HOOK_Size(link);

    for (int i = 0; i < val; i++) {
        alias_ttr = current->ptr;
        current = current->next;

        if (alias_ttr == NULL) continue;
        if (alias_ttr->obj == OBJ_NULL || alias_ttr->obj == OBJ_WINDOW_OOB_RENDER) continue;

        if (alias_ttr->texture != NULL)
            SDL_RenderCopy(glo_render, alias_ttr->texture, NULL, &alias_ttr->dst_rect);

        if (alias_ttr->obj == OBJ_LINK_PTR)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_WINDOW)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_WINDOW_CHAT)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_BUTTON_LINK_ON)
            GUI_HookLink_RenderObj(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_WINDOW_INNER_OOB_CHAT) {
            GUI_Alias_InnerWindow_Render(alias_ttr);
            GUI_HookLink_Render(alias_ttr->link);
        }

        // temporary fix
        else if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
            GUI_Alias_Textbox_UpdateRect(alias_ttr);

            alias_ptr = alias_ttr->link;
            SDL_RenderCopy(glo_render, alias_ptr->texture, &alias_ptr->src_rect, &alias_ptr->dst_rect);

            if (alias_ttr->add != NULL) {

                alias_ptr = alias_ttr->add;
                SDL_RenderCopy(glo_render, alias_ptr->texture, &alias_ptr->src_rect, &alias_ptr->dst_rect);
            }
        }
    }

    return 0;
}

void GUI_HookList_Quit(PP4M_HOOK *hook_list) {

    PP4M_HOOK *curr = hook_list;
    GUI_TextureAlias *alias = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    int val = pp4m_HOOK_Size(hook_list);

    for (int i = 0; i < val; i++) {

        alias = curr->ptr;

        if (alias->obj == OBJ_BUTTON_LINK_OFF || alias->obj == OBJ_BUTTON_LINK_ON) {
            alias_ptr = alias->link;

            if (alias_ptr->obj == OBJ_WINDOW_CHAT)
                GUI_AliasDestroy_WindowChat(alias_ptr);

            if (alias_ptr->texture != NULL)
                SDL_DestroyTexture(alias_ptr->texture);
        }

        if (alias->texture != NULL)
            SDL_DestroyTexture(alias->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(hook_list);

    return;
}

int GUI_HookLink_Update(PP4M_HOOK *link, PP4M_INPUT_POS input, char **buffer, int key, int *code) {
    int result = 0;

    PP4M_HOOK *current = link;

    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    int val = pp4m_HOOK_Size(link);

    for (int i = 0; i < val; i++) {
        alias_ttr = current->ptr;

        if (alias_ttr->obj == OBJ_NULL) continue;

        // hooked list update
        if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF || alias_ttr->obj == OBJ_BUTTON_LINK_ON) {
            alias_ptr = alias_ttr->link;

            GUI_HookLink_Update(alias_ptr->link, input, buffer, key, code);
        }

        // we should create a func for this
        if (input.iner == 1) {
            if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
                alias_ptr = alias_ttr->link;

                if (GUI_Alias_InputOnObj(input, alias_ttr->dst_rect) == 1) alias_ptr->obj = OBJ_TEXTBOX_INPUT_ON;
                else alias_ptr->obj = OBJ_TEXTBOX_INPUT_OFF;

                /*
                // resetting the blinking texture
                GUI_TextureAlias *alias_blink = alias_ttr->add;
                alias_blink->timer = 255;
                GUI_Alias_BlinkUpdate(alias_ttr);
                */
            }
        }

        if (alias_ttr->obj == OBJ_WINDOW_INNER_OOB_CHAT)
            GUI_Ingame_ChatUpdate(alias_ttr, OPENSANS_REGULAR, PP4M_BLACK, 14, buffer);

        /*
        // note: is a bit of a missleading the name of the func.
        // instead of updating everything OBJ_WINDOW_CHAT releated,
        // it only updates any pressed keys or adding a message to the list
        else if (alias_ttr->obj == OBJ_WINDOW_CHAT) {


            // anyways ...
            GUI_HookLink_Update(alias_ttr->link, input, buffer, key, code);
        }
        */

        else if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
            alias_ptr = alias_ttr->link;

            if (alias_ptr->obj == OBJ_TEXTBOX_INPUT_ON)
                GUI_Alias_Textbox_UpdateAlias(alias_ttr, OPENSANS_REGULAR, PP4M_BLACK, 18, buffer, key, code);
        }

        if (input.iner == 1) {
            if (GUI_Alias_InputOnObj(input, alias_ttr->dst_rect) == 1) {

                if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF) {
                    alias_ttr->obj = OBJ_BUTTON_LINK_ON;
                }

                else if (alias_ttr->obj == OBJ_BUTTON_RETURN) result = -1;
                else if (alias_ttr->obj == OBJ_BUTTON_EXIT) result = -2;
            } else {
                if (alias_ttr->obj == OBJ_BUTTON_LINK_ON) {
                    alias_ptr = alias_ttr->link;

                    if (GUI_Alias_InputOnObj(input, alias_ptr->dst_rect) != 1)
                        alias_ttr->obj = OBJ_BUTTON_LINK_OFF;
                }
            }
        }

        current = current->next;
    }

    return result;
}

/*
int GUI_HookList_Update(PP4M_HOOK *hook_list, PP4M_INPUT_POS input, char **buffer, int key, int *code) {

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

            // hooked list update
            if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF || alias_ttr->obj == OBJ_BUTTON_LINK_ON) {
                alias_ptr = alias_ttr->link;
                printf("alias->obj = %d\n", alias_ttr->obj);

                GUI_HookLink_Update(alias_ptr->link, input, buffer, key, code);
            }


            else if (alias_ttr->obj == OBJ_WINDOW)
                GUI_HookLink_Update(alias_ttr->link, input, buffer, key, code);

            // note: is a bit of a missleading the name of the func.
            // instead of updating everything OBJ_WINDOW_CHAT releated,
            // it only updates any pressed keys or adding a message to the list
            else if (alias_ttr->obj == OBJ_WINDOW_CHAT) {

                printf("hello\n");
                GUI_Ingame_ChatUpdate(current, OPENSANS_REGULAR, PP4M_BLACK, 14, buffer);

                // anyways ...
                GUI_HookLink_Update(alias_ttr->link, input, buffer, key, code);
            }

            if (input.iner == 1) {
                if (GUI_Alias_InputOnObj(input, alias_ttr->dst_rect) == 1) {

                    if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF) {
                        alias_ttr->obj = OBJ_BUTTON_LINK_ON;
                    }

                    else if (alias_ttr->obj == OBJ_BUTTON_RETURN) result = -1;
                    else if (alias_ttr->obj == OBJ_BUTTON_EXIT) result = -2;
                } else {
                    if (alias_ttr->obj == OBJ_BUTTON_LINK_ON) {
                            /* old code
                        link_ptr = alias_ttr->link;
                        alias_ptr = link_ptr->ptr;

                        if (GUI_Alias_InputOnObj(input, alias_ptr->dst_rect) != 1)
                            alias_ttr->obj = OBJ_BUTTON_LINK_OFF;

                    }
                }
            }

            /* attempt on highlighting objects failed :(

            else if (GUI_Alias_InputOnObj(input, alias_ttr->dst_rect) == 1) {

                // mouse on top of object highlights it
                if (alias_ttr->obj != OBJ_NONE) {
                    if (GUI_Alias_IsColor(&color_btn_bak) == -1) SDL_GetTextureColorMod(alias_ttr->texture, &color_btn_bak.r, &color_btn_bak.g, &color_btn_bak.b);
                    SDL_SetTextureColorMod(alias_ttr->texture, 220, 220, 220);
                }

            } else if (GUI_Alias_InputOnObj(input, alias_ttr->dst_rect) == -1) {

                // restores the object to original color
                if (GUI_Alias_IsColor(&color_btn_bak) == 1) {
                    SDL_SetTextureColorMod(alias_ttr->texture, color_btn_bak.r, color_btn_bak.g, color_btn_bak.b);
                    GUI_Alias_ResetColor(&color_btn_bak);
                }
            }

            curr_ptr = curr_ptr->next;
        }
    }

    return result;
}
*/

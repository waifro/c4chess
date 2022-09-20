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
#include "textbox.h"
#include "destroy.h"
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
	if (link == NULL) return -1;
	
    PP4M_HOOK *current = link;
    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    int val = pp4m_HOOK_Size(link);

    for (int i = 0; i < val; i++) {
        alias_ttr = current->ptr;
        current = current->next;
		
		// ignore certain objects
        if (alias_ttr == NULL) continue;
        
        if (alias_ttr->obj == OBJ_NULL ||
        	alias_ttr->obj == OBJ_WINDOW_OOB_RENDER ||
        	alias_ttr->obj == OBJ_FUNC_CONTAINER) continue;
		
		// check if a texture exists
        if (alias_ttr->texture != NULL)
            SDL_RenderCopy(glo_render, alias_ttr->texture, NULL, &alias_ttr->dst_rect);
		
		/*
        if (alias_ttr->obj == OBJ_LINK_PTR)
            GUI_HookLink_Render(alias_ttr->link);

		else if (alias_ttr->obj == OBJ_DISPLAY_TIMER)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_WINDOW)
            GUI_HookLink_Render(alias_ttr->link);

        else if (alias_ttr->obj == OBJ_WINDOW_CHAT)
            GUI_HookLink_Render(alias_ttr->link);
		*/
		
        if (alias_ttr->obj == OBJ_BUTTON_LINK_ON)
            GUI_HookLink_RenderObj(alias_ttr->link);
		
        else if (alias_ttr->obj == OBJ_WINDOW_INNER_OOB_CHAT) {
            GUI_Alias_InnerWindow_Render(alias_ttr);
            GUI_HookLink_Render(alias_ttr->link);
        }
		
        // temporary fix
        else if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
            GUI_Textbox_UpdateRect(alias_ttr);

            alias_ptr = alias_ttr->link;
            SDL_RenderCopy(glo_render, alias_ptr->texture, &alias_ptr->src_rect, &alias_ptr->dst_rect);

            if (alias_ptr->obj == OBJ_TEXTBOX_INPUT_ON)
                if (alias_ttr->add != NULL) {

                    alias_ptr = alias_ttr->add;
                    SDL_RenderCopy(glo_render, alias_ptr->texture, &alias_ptr->src_rect, &alias_ptr->dst_rect);
                }
        }
        
        // hopefully this wont break stuff
		else if (GUI_HookLink_ValidRender(alias_ttr) == 0)
			GUI_HookLink_Render(alias_ttr->link);
    }

    return 0;
}

int GUI_HookLink_ValidRender(GUI_TextureAlias *alias_ttr) {
	
	if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF || alias_ttr->obj == OBJ_BUTTON_LINK_ON)
		return -2;
	
	if (alias_ttr->link == NULL)
		return -1;

	return 0;
}

int GUI_HookList_Quit(PP4M_HOOK *hook_list) {
	if (hook_list == NULL) return -1;
	
    PP4M_HOOK *curr = hook_list;
    GUI_TextureAlias *alias = NULL;
    GUI_TextureAlias *alias_ptr = NULL;

    int val = pp4m_HOOK_Size(hook_list);

    for (int i = 0; i < val; i++) {

        alias = curr->ptr;

		if (alias->obj == OBJ_DISPLAY_TIMER) {
			GUI_Destroy_Timer(alias->link);
			
		} else if (alias->obj == OBJ_BUTTON_LINK_OFF || alias->obj == OBJ_BUTTON_LINK_ON) {
            alias_ptr = alias->link;

            if (alias_ptr->obj == OBJ_WINDOW_CHAT)
                GUI_Destroy_WindowChat(alias_ptr);

            if (alias_ptr->texture != NULL)
                SDL_DestroyTexture(alias_ptr->texture);
        } else {
        	
        	if (alias->link != NULL) {
        		GUI_HookList_Quit(alias->link);
        		alias->link = NULL;
        	}
        		
        }

        if (alias->texture != NULL)
            SDL_DestroyTexture(alias->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(hook_list);

    return 0;
}

int GUI_HookLink_Update(PP4M_HOOK *link, PP4M_INPUT_POS input, char **buf_arr, int key, int *code, int *socket) {
    if (link == NULL) return -1;
    
    int result = 0;

    PP4M_HOOK *current = link;

    GUI_TextureAlias *alias_ttr = NULL;
    GUI_TextureAlias *alias_ptr = NULL;
	
	// applying onthefly modifications to mouse surfacing
	static SDL_Color color_bak;
	static GUI_TextureAlias *pointer_obj = NULL;
	
    int val = pp4m_HOOK_Size(link);

    for (int i = 0; i < val; i++) {
    	if (result != 0) break;
    	
        alias_ttr = current->ptr;
		
		if (alias_ttr->obj == OBJ_NULL) continue;
		
		else if (alias_ttr->obj == OBJ_LINK_PTR) {
			result = GUI_HookLink_Update(alias_ttr->link, input, buf_arr, key, code, socket);
			continue;
		}
		
		else if (alias_ttr->obj == OBJ_FUNC_CONTAINER) {
		
			int (*func)(int *) = alias_ttr->add;
			
			result = func(socket);
			
			if (result == 1) return -1;
			
			continue;
			
		}
		
        // hooked list update
        if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF || alias_ttr->obj == OBJ_BUTTON_LINK_ON) {
            alias_ptr = alias_ttr->link;

            GUI_HookLink_Update(alias_ptr->link, input, buf_arr, key, code, socket);
        }

        // we should create a func for this
        if (input.iner == 1) {
            if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
                alias_ptr = alias_ttr->link;

                if (GUI_Alias_InputOnObj(input, alias_ttr->dst_rect) == 1) alias_ptr->obj = OBJ_TEXTBOX_INPUT_ON;
                else alias_ptr->obj = OBJ_TEXTBOX_INPUT_OFF;
            }
        }

        if (alias_ttr->obj == OBJ_DISPLAY_TIMER) {
            GUI_Ingame_TimerUpdate_Clock(alias_ttr);
        }

        if (alias_ttr->obj == OBJ_WINDOW_INNER_OOB_CHAT)
            GUI_Ingame_ChatUpdate(alias_ttr, OPENSANS_REGULAR, PP4M_BLACK, 14, buf_arr);

        else if (alias_ttr->obj == OBJ_TEXTBOX_ALIAS) {
            alias_ptr = alias_ttr->link;

            if (alias_ptr->obj == OBJ_TEXTBOX_INPUT_ON)
                GUI_Textbox_UpdateAlias(alias_ttr, OPENSANS_REGULAR, PP4M_BLACK, 18, buf_arr, key, code);
        }

        if (input.iner == 1) {
            if (GUI_Alias_InputOnObj(input, alias_ttr->dst_rect) == 1) {

                if (alias_ttr->obj == OBJ_BUTTON_LINK_OFF)
                	alias_ttr->obj = OBJ_BUTTON_LINK_ON;
				
				else if (alias_ttr->obj == OBJ_BUTTON_PLAY)
					result = OBJ_BUTTON_PLAY;
				
				else if (alias_ttr->obj == OBJ_BUTTON_PLAY_ONLINE)
					result = OBJ_BUTTON_PLAY_ONLINE;
				
                else if (alias_ttr->obj == OBJ_BUTTON_RETURN)
					result = -1;
					
                else if (alias_ttr->obj == OBJ_BUTTON_EXIT)
					result = -2;
				
            } else {
                if (alias_ttr->obj == OBJ_BUTTON_LINK_ON) {
                    alias_ptr = alias_ttr->link;

                    if (GUI_Alias_InputOnObj(input, alias_ptr->dst_rect) != 1)
                        alias_ttr->obj = OBJ_BUTTON_LINK_OFF;
                }
            }
        }
		
		GUI_HoverMouse_Highlight(&input, &alias_ttr, &pointer_obj, &color_bak);
		
        current = current->next;
    }

    return result;
}

int GUI_HoverMouse_Highlight(PP4M_INPUT_POS *input, GUI_TextureAlias **alias_ttr, GUI_TextureAlias **alias_ptr, SDL_Color *color_bak) {
	
	GUI_TextureAlias *ttr = *alias_ttr;
	GUI_TextureAlias *ptr = *alias_ptr;
	
	if (ttr->obj == OBJ_NONE ||
		ttr->obj == OBJ_BUTTON_LINK_OFF ||
		ttr->obj == OBJ_BUTTON_LINK_ON)
		return -1;
	
	// mouse hovering on top of object
	if (ttr != ptr) {
			
		if (GUI_Alias_InputOnObj(*input, ttr->dst_rect) == 1) {
			
			// reset old pointer back to original colors
			if (ptr != NULL)
				SDL_SetTextureColorMod(ptr->texture, color_bak->r, color_bak->g, color_bak->b);
				
			// save new pointer and color
			*alias_ptr = ttr;
			SDL_GetTextureColorMod(ttr->texture, &color_bak->r, &color_bak->g, &color_bak->b);
				
			// highlight object
			SDL_SetTextureColorMod(ttr->texture, 220, 220, 220);

		} else if (ptr != NULL && GUI_Alias_InputOnObj(*input, ptr->dst_rect) == -1) {
				
			// reset old pointer back to original colors
			SDL_SetTextureColorMod(ptr->texture, color_bak->r, color_bak->g, color_bak->b);
					
			// reset also ther pointer
			ptr = NULL;
		}
	}

	return 0;
}



#include "destroy.h"
#include "gui.h"

#include "../pp4m/pp4m.h"
#include <SDL2/SDL.h>

int GUI_Destroy_WindowChat(GUI_TextureAlias *window) {

    PP4M_HOOK *curr = window->link;
    int val = pp4m_HOOK_Size(window->link);

    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        alias_ptr = curr->ptr;

        if (alias_ptr->obj == OBJ_WINDOW_INNER_OOB_CHAT)
            GUI_Destroy_WindowChat_InnerWindow(alias_ptr);
        else if (alias_ptr->obj == OBJ_TEXTBOX_ALIAS)
            GUI_Destroy_Textbox(alias_ptr);

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(window->link);

    return 0;
}

int GUI_Destroy_WindowChat_InnerWindow(GUI_TextureAlias *inner_window) {

    PP4M_HOOK *curr = inner_window->link;
    int val = pp4m_HOOK_Size(inner_window->link);

    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        alias_ptr = curr->ptr;

        if (alias_ptr->obj == OBJ_WINDOW_OOB_RENDER)
            GUI_Destroy_WindowChat_Chat(alias_ptr->link);

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(inner_window->link);

    return 0;
}

int GUI_Destroy_WindowChat_Chat(PP4M_HOOK *list) {

    PP4M_HOOK *curr = list;

    int val = pp4m_HOOK_Size(list);
    GUI_TextureAlias *alias_ptr = NULL;

    for (int i = 0; i < val; i++) {
        alias_ptr = curr->ptr;

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);

        curr = curr->next;
    }

    for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(list);

    return 0;
}

int GUI_Destroy_Textbox(GUI_TextureAlias *alias_ttr) {
    GUI_TextureAlias *alias_ptr = alias_ttr->link;
    GUI_TextureAlias *alias_blink = alias_ttr->add;

    if (alias_ptr != NULL) {
        if (alias_ptr->link != NULL)
            free(alias_ptr->link); // char*

        if (alias_ptr->texture != NULL)
            SDL_DestroyTexture(alias_ptr->texture);
    }

    if (alias_blink != NULL) {
        if (alias_blink->texture != NULL)
            SDL_DestroyTexture(alias_blink->texture);
    }

    return 0;
}


int GUI_Destroy_Timer(PP4M_HOOK *list) {
	
	PP4M_HOOK *curr = list;
	
	int val = pp4m_HOOK_Size(list);
	GUI_TextureAlias *alias_ptr = NULL;
	
	for (int i = 0; i < val; i++) {
		alias_ptr = curr->ptr;
		
		if (alias_ptr->texture != NULL)
			SDL_DestroyTexture(alias_ptr->texture);
		
		if (alias_ptr->add != NULL)
			free(alias_ptr->add);
		
		curr = curr->next;
	}
	
	for (int i = 0; i < val; i++)
        pp4m_HOOK_Remove(list);
	
	return 0;
}

int GUI_Destroy_Alias(GUI_TextureAlias *ttr_alias) {
	
	if (ttr_alias != NULL) {
	
		if (ttr_alias->texture != NULL)
			SDL_DestroyTexture(ttr_alias->texture);
			
		if (ttr_alias->link != NULL)
			GUI_Destroy_Alias(ttr_alias->link);
			
		if (ttr_alias->add != NULL)
			free(ttr_alias->add);

	}

	return 0;
}



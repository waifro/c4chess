#ifndef GUI_ALIAS_H
#define GUI_ALIAS_H

#include <SDL2/SDL.h>
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_input.h"

/* structures & variables */

typedef enum {
    OBJ_NONE,                   // ignore object
    OBJ_NULL,                   // drop interaction

    OBJ_BUTTON_TXTBOX,          // object to toggle OBJ_TEXTBOX_INPUT buffer

    OBJ_BUTTON_LINK_OFF,        // indicates *link as linked list toggled off
    OBJ_BUTTON_LINK_ON,         // indicates *link as linked list toggled on

    OBJ_BUTTON_RETURN,          // go back
    OBJ_BUTTON_EXIT,            // terminate any loop

    OBJ_WINDOW,                 // window containing more stuff for general purposes
    OBJ_WINDOW_CHAT,            // window containing structure of windowed chat
    OBJ_WINDOW_INNER_OOB,       // used for cropping textures out-of-bounds inside this rectangle
    OBJ_WINDOW_INNER_OOB_CHAT,  // window containing chat structure

    OBJ_SCROLL_VERTICAL,        // with input, scroll up and down
    OBJ_SCROLL_HORIZONTAL,      // with input, scroll left or right

    OBJ_TEXTBOX_LINK,           // texture behind OBJ_TEXTBOX_INPUT + *link as linked list
    OBJ_TEXTBOX_ALIAS,          // texture behind OBJ_TEXTBOX_INPUT + *link as GUI_TextureAlias

    OBJ_TEXTBOX_INPUT_OFF,      // init, reset, update and modify texture's input
    OBJ_TEXTBOX_INPUT_ON,

    OBJ_CHAT_MESG,              // normal chat message
    OBJ_CHAT_RECT,              // OBJ_CHAT_MESG -> link = SDL_Rect (used in conjunction with OBJ_INNER_WINDOW_OOB)

    OBJ_LINK_PTR                // treat object as linked list

} GUI_ALIAS_OBJ;

/*
    structure of different objs inside a linked list

    |- && LL
    |- OBJ_NONE                                   // initial window
    |- OBJ_NONE                                   // minor thing to render
    |- OBJ_NULL                                   // minor thing to avoid
    |- OBJ_BUTTON_LINK_OFF \                      // contains linked list for example chat
                            - OBJ_WINDOW_CHAT **structure of ingame chat windowed**
    |- OBJ_LINK_PTR \
                    |- && LL
                    |- OBJ_NONE
                    (NULL)
    |- OBJ_NONE
    |- OBJ_NONE
    |- OBJ_NONE
    (NULL)
*/

/*  OBJ_WINDOW_CHAT
    hipothetical structure of ingame chat windowed

    |- OBJ_WINDOW_CHAT \
                        |- && LL                                          // contains linked list of entire conversation
                        |- OBJ_NONE                                             // white blank object behind chat
                        |- OBJ_WINDOW_INNER_OOB_CHAT \                          // useful for cropping images and adding scrolls objects
                                                     |- && LL
                                                     |- OBJ_SCROLL_VERTICAL                                 // obj used for scrolling up/down
                                                     |- OBJ_LINK_PTR \
                                                                     |- && LL
                                                                     |- OBJ_CHAT_MESG          // conversation from player 0
                                                                     |- OBJ_CHAT_MESG          // conversation from player 1
                                                                     |- OBJ_CHAT_MESG          // conversation from player 0
                                                                     |- OBJ_CHAT_MESG          // conversation from player 0, new line
                                                                     |- OBJ_CHAT_MESG          // conversation from player 1
                                                                     |- etc ...
                                                                     (NULL)
                                                     (NULL)
                        |- OBJ_TEXTBOX_ALIAS \
                                             |- OBJ_TEXTBOX_INPUT_OFF \                       // obj to indicate accept input or not
                                                                       |- char*
                        |- OBJ_BUTTON_TXTBOX                                                     // send message
                        (NULL)

*/

/* definitions */

typedef struct {
    GUI_ALIAS_OBJ obj;
    void *link;
    SDL_Rect rect;
    SDL_Texture *texture;
} GUI_TextureAlias;

/* prototypes */
int GUI_Alias_InputOnObj(PP4M_INPUT_POS input, SDL_Rect rect);

int GUI_Alias_IsColor(SDL_Color *color);
int GUI_Alias_ResetColor(SDL_Color *color);

GUI_TextureAlias *GUI_Alias_InitAlias(void);

// return NULL on end of list, otherwise return PP4M_HOOK pointer where obj corrisponds on the list
PP4M_HOOK *GUI_Alias_FindObj(PP4M_HOOK *hook_list, GUI_ALIAS_OBJ obj);

SDL_Texture *GUI_Alias_CreateSnapshot(SDL_Renderer *renderer, int width, int height);

int GUI_Alias_Textbox_Empty(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer);
int GUI_Alias_Textbox_UpdateTexture(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point);
int GUI_Alias_Textbox_UpdateRect(GUI_TextureAlias *alias_ttr, SDL_Rect *rect);

int GUI_Alias_Textbox_Backspace(char *buf);

int GUI_Alias_Textbox_InitAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer);
int GUI_Alias_Textbox_UpdateAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char **buffer, int key, int *code);
int GUI_Alias_Textbox_DestrAlias(GUI_TextureAlias *alias_ptr);

// return -1 if src is NULL, -2 if dest is NULL, otherwise 0 on success
int GUI_Alias_RectCopy(SDL_Rect *dest, SDL_Rect *src);

// return -1 if outside of oob, otherwise 0 on success
int GUI_Alias_RectUpdate_OOB(SDL_Rect *rect_1, SDL_Rect *rect_2, SDL_Rect *rect_oob);

int GUI_Alias_InnerWindow_Render(GUI_TextureAlias *window_inner_oob);

PP4M_HOOK *GUI_Alias_Tail(GUI_TextureAlias *alias);

#endif

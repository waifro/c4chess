#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SDL2/SDL.h>

#include "gui.h"

/* on success, initializes everything textbox releated */
int GUI_Textbox_InitAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer);

/* on success, updates everything textbox releated */
int GUI_Textbox_UpdateAlias(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char **buffer, int key, int *code);

/* on success, updates the texture from the allocated string */
int GUI_Textbox_UpdateTexture(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point);

/* on success, updates the texture's rect to be inside the frame */
int GUI_Textbox_UpdateRect(GUI_TextureAlias *alias_ttr);

/* on success, retraces to the previous character */
int GUI_Textbox_Backspace(char *buf);

/* on success, allocates a temporary texture */
int GUI_Textbox_Empty(GUI_TextureAlias *alias_ttr, char *pathname, SDL_Color color, int point, char *buffer);

/* on success, initializes the blink object */
int GUI_Textbox_BlinkInit(GUI_TextureAlias *alias_ttr);

/* on success, updates blink object frame by frame */
int GUI_Textbox_BlinkUpdate(GUI_TextureAlias *alias_ttr);

#endif

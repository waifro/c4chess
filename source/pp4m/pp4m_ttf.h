/* Private Project Four Me */

#ifndef _PP4M_TTF_H
#define _PP4M_TTF_H

void pp4m_TTF_Init(void);
void pp4m_TTF_Quit(void);

void pp4m_TTF_MEM_TextureFont(SDL_Renderer *renderer, PP4M_SDL *st, const char *text, const char *path, int point, float x, float y, SDL_Color color);
SDL_Texture *pp4m_TTF_TextureFont(SDL_Renderer *renderer, char *filename, SDL_Color color, int point, SDL_Rect *dst, float x, float y, char *text);

#endif // _PP4M_TTF_H


/* 2022 @waifro */
/* Private Project Four Me */

#ifndef _PP4M_IMAGE_H
#define _PP4M_IMAGE_H

void pp4m_IMG_Init(void);
void pp4m_IMG_Quit(void);

SDL_Texture *pp4m_IMG_ImageToTexture(SDL_Renderer *renderer, SDL_Texture *texture, char *path, SDL_Rect *rect, float x, float y, int w, int h);

#endif // _PP4M_IMAGE_H

/* 2022 @waifro */

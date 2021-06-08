/* Private Project Four Me */

#include <SDL2/SDL.h>
#include "pp4m_draw.h"

SDL_Texture *pp4m_DRAW_TextureRect(SDL_Renderer *renderer, SDL_Color color, SDL_Rect *rect, float x, float y, float w, float h) {

    SDL_Surface surface;
    SDL_Texture *texture = NULL;

    memcpy(&surface, (void*)SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0), sizeof(surface));

    rect->x = 0;
    rect->y = 0;
    rect->w = (int)w;
    rect->h = (int)h;

    SDL_FillRect(&surface, rect, SDL_MapRGB((void*)surface.format, color.r, color.g, color.b));

    rect->x = (int)x;
    rect->y = (int)y;

    texture = SDL_CreateTextureFromSurface(renderer, &surface);

    SDL_FreeSurface(&surface);
    return texture;

}

/*                                         Copyright (c) 2021 @richardwaifro //
#                                                                                                                                                    #
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), #
# to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, #
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:         #
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.                     #
#                                                                                                                                                    #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,                                                                                    #
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                                                                #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.                                                                                              #
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,                                                                        #
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                                                     #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                            #
#                                                                                                                                                    #
/#                                         Copyright (c) 2021 @richardwaifro */

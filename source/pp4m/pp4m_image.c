/* Private Project Four Me */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pp4m_io.h"
#include "pp4m_image.h"

void pp4m_IMG_Init(void) {

    IMG_Init(IMG_INIT_PNG);

    return;
}

void pp4m_IMG_Quit(void) {

    IMG_Quit();

    return;

}

SDL_Texture *pp4m_IMG_ImageToRenderer(SDL_Renderer *renderer, SDL_Texture *texture, char *path, SDL_Rect *rect, float x, float y, int w, int h) {

    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }

    SDL_Surface *surface = IMG_Load(path);

    if (surface == NULL) pp4m_IO_Feedback("feedback.txt", SDL_GetError());

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) pp4m_IO_Feedback("feedback.txt", SDL_GetError());

    SDL_FreeSurface(surface);

    if (x > 0 || y > 0 || w > 0 || h > 0) {

        rect->x = (int)x;
        rect->y = (int)y;

        if (w == 0 || h == 0) SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
        else {
            rect->w = w;
            rect->h = h;
        }

    }

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

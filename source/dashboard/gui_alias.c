#include <stdio.h>
#include <stdlib.h>

#include "gui_alias.h"

void GUI_TextureAlias_InitRect(GUI_TextureAlias *source, int x, int y, int w, int h, GUI_TextureFlagAlias flag) {

    memcpy(&source->rect_als.x, &x, sizeof(x));
    memcpy(&source->rect_als.y, &y, sizeof(y));
    memcpy(&source->rect_als.w, &w, sizeof(w));
    memcpy(&source->rect_als.h, &h, sizeof(h));

    if (flag != EMPTY) {
        int z = x + w;
        int j = y + j;

        memcpy(&source->rect_als.z, &z, sizeof(z));
        memcpy(&source->rect_als.j, &j, sizeof(j));
    }

    source->rect.x = source->rect_als.x;
    source->rect.y = source->rect_als.y;
    source->rect.w = source->rect_als.w;
    source->rect.h = source->rect_als.h;

    return;
}

/*
SDL_Rect *GUI_TxrAlias_Src(GUI_RectAlias *destin) {



    return;
}

SDL_Rect *GUI_txrAlias_Dst(GUI_RectAlias *source) {

    // converts GUI_RectAlias to SDL_Rect
    SDL_Rect *result;
    result = NULL;

    &result->x == &source->x;
    result->y = &source->y;
    result->w = &source->w;
    result->h = &source->h;


    return result;
}
*/

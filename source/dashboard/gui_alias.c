#include <stdio.h>
#include <stdlib.h>

#include "gui_alias.h"

void GUI_TextureAlias_InitRect(GUI_TextureAlias *source, int x, int y, int w, int h, GUI_TextureFlagAlias flag) {

    source->rect.x = x;
    source->rect.y = y;
    source->rect.h = h;
    source->rect.w = w;

    if (flag != EMPTY) {
        int z = x + w;
        int j = y + h;

        source->rect_als.z = z;
        source->rect_als.j = j;
    }

    source->rect_als.x = source->rect.x;
    source->rect_als.y = source->rect.y;
    source->rect_als.w = source->rect.w;
    source->rect_als.h = source->rect.h;

    return;
}

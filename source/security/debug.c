#include <stdio.h>
#include <stdarg.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_draw.h"
#include "../dashboard/gui_alias.h"
#include "../global.h"
#include "debug.h"

PP4M_HOOK *debug_list;

int DEBUG_PrintBox(const char *format, ...) {
    if (!debug_list) DEBUG_InitBox();
    char buffer[256];

    va_list arg;
    va_start(arg, format);
    vsprintf(buffer, format, arg);
    va_end(arg);

    int val = pp4m_HOOK_Size(debug_list);

    PP4M_HOOK *current = debug_list;
    for (; val > 0; val--)
        current = current->next;

    GUI_TextureAlias *foo = current->ptr;

    GUI_TextureAlias *buf_txt = calloc(1, sizeof(GUI_TextureAlias));
    buf_txt->obj = 0;
    buf_txt->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_WHITE, 12, &buf_txt->rect, foo->rect.x + 10, foo->rect.y, buffer);

    pp4m_HOOK_Next(debug_list, buf_txt);

    return 0;
}

int DEBUG_InitBox(void) {

    debug_list = pp4m_HOOK_Init();

    GUI_TextureAlias *box = calloc(1, sizeof(GUI_TextureAlias));
    box->obj = 0;
    box->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_BLACK, 150, &box->rect, 50, 50, 300, 300);

    pp4m_HOOK_Next(debug_list, box);

    return 0;
}

int DEBUG_QuitBox(void) {

    int val = pp4m_HOOK_Size(debug_list);
    for (; val > 0; val--)
        pp4m_HOOK_Remove(debug_list);

    return 0;
}

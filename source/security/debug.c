#include <stdio.h>
#include <stdarg.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"
#include "../dashboard/gui.h"
#include "../dashboard/gui_alias.h"
#include "../global.h"
#include "debug.h"

PP4M_HOOK *glo_debug_list = NULL;

int DEBUG_PrintBox(int level, const char *format, ...) {

    #if DEBUG_LEVEL > 0
    if (level <= DEBUG_LEVEL) {
        if (glo_debug_list == NULL) DEBUG_InitBox();
        char buffer[256];

        va_list arg;
        va_start(arg, format);
        vsnprintf(buffer, 256, format, arg);
        va_end(arg);

        int line = pp4m_HOOK_Size(glo_debug_list);
        if (line > 20) DEBUG_UpdateBox_ClearLine(&line);

        DEBUG_UpdateBox_WriteLine(line, buffer);
    }
    #endif // DEBUG_LEVEL

    return 0;
}

int DEBUG_UpdateBox_Render(void) {

    PP4M_HOOK *current = glo_debug_list;
    GUI_TextureAlias *alias_ttr = NULL;

    for (int i = pp4m_HOOK_Size(glo_debug_list); i > 0; i--) {
        alias_ttr = current->ptr;
        SDL_RenderCopy(glo_render, alias_ttr->texture, NULL, &alias_ttr->dst_rect);
        current = current->next;
    }

    return 0;
}

int DEBUG_UpdateBox_WriteLine(int val, char *string) {

    GUI_TextureAlias *buf = NULL;
    PP4M_HOOK *current = glo_debug_list;
    for (int i = 0; i < val; i++) {
        buf = current->ptr;
        current = current->next;
    }

    GUI_TextureAlias *buf_txt = calloc(1, sizeof(GUI_TextureAlias));
    buf_txt->obj = 0;
    buf_txt->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_WHITE, 12, &buf_txt->dst_rect, buf->dst_rect.x, buf->dst_rect.y + 15, string);
    if (val == 0) buf_txt->dst_rect.x += 15;

    pp4m_HOOK_Next(glo_debug_list, buf_txt);

    return 0;
}

int DEBUG_UpdateBox_ClearLine(int *val) {

    PP4M_HOOK *tail = NULL;
    GUI_TextureAlias *alias_ttr = NULL;

    PP4M_HOOK *bak = NULL;
    bak = pp4m_HOOK_Init();

    tail = glo_debug_list;
    for (int i = 0; i < *val; i++) {
        if (i != 1) pp4m_HOOK_Next(bak, tail->ptr);
        tail = tail->next;
    }

    *val = pp4m_HOOK_Size(bak);
    tail = glo_debug_list;

    for (int i = 0; i < *val; i++) {
        tail->ptr = bak->ptr;

        if (i != 0)
        {
            alias_ttr = tail->ptr;
            alias_ttr->dst_rect.y -= 15;
        }

        bak = bak->next;
        tail = tail->next;
    }

    pp4m_HOOK_Remove(glo_debug_list);

    return 0;
}

int DEBUG_InitBox(void) {

    glo_debug_list = pp4m_HOOK_Init();

    GUI_TextureAlias *box = calloc(1, sizeof(GUI_TextureAlias));
    box->obj = 0;
    box->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_BLACK, 100, &box->dst_rect, 15, 15, 500, 330);

    pp4m_HOOK_Next(glo_debug_list, box);

    return 0;
}

int DEBUG_QuitBox(void) {

    int val = pp4m_HOOK_Size(glo_debug_list);
    for (; val > 0; val--)
        pp4m_HOOK_Remove(glo_debug_list);

    return 0;
}

#include <stdio.h>
#include <stdarg.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"
#include "../dashboard/gui.h"
#include "../dashboard/gui_alias.h"
#include "../global.h"
#include "debug.h"

PP4M_HOOK *glo_debug_list;

int DEBUG_PrintBox(int level, const char *format, ...) {
    #if DEBUG_LEVEL > 0

    if (level == DEBUG_LEVEL) {
        if (!glo_debug_list) DEBUG_InitBox();
        char buffer[256];

        va_list arg;
        va_start(arg, format);
        vsprintf(buffer, format, arg);
        va_end(arg);

        int val = pp4m_HOOK_Size(glo_debug_list);

        PP4M_HOOK *current = glo_debug_list;

        if (val > 10) {

            free(current->ptr);
            PP4M_HOOK *bak = pp4m_HOOK_Init();

            current = current->next;
            for (int i = 0; i < val; i++) {
                pp4m_HOOK_Next(bak, current);
                if (current->next == NULL) break;
                current = current->next;
            }

            val = pp4m_HOOK_Size(bak);
            for (int i = 0; i < val; i++) {
                pp4m_HOOK_Next(glo_debug_list, bak);
                if (bak->next == NULL) break;
                bak = bak->next;
            }

            for (int i = 0; i < val; i++)
                pp4m_HOOK_Remove(bak);

            current = glo_debug_list;
        }

        for (; val > 0; val--)
            current = current->next;

        GUI_TextureAlias *foo = current->ptr;

        GUI_TextureAlias *buf_txt = calloc(1, sizeof(GUI_TextureAlias));
        buf_txt->obj = 0;
        buf_txt->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_WHITE, 12, &buf_txt->rect, foo->rect.x + 10, foo->rect.y, buffer);

        pp4m_HOOK_Next(glo_debug_list, buf_txt);
    }
    #endif // DEBUG_LEVEL

    return 0;
}

int DEBUG_InitBox(void) {

    glo_debug_list = pp4m_HOOK_Init();

    GUI_TextureAlias *box = calloc(1, sizeof(GUI_TextureAlias));
    box->obj = 0;
    box->texture = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_BLACK, 150, &box->rect, 50, 50, 300, 300);

    pp4m_HOOK_Next(glo_debug_list, box);

    return 0;
}

int DEBUG_QuitBox(void) {

    int val = pp4m_HOOK_Size(glo_debug_list);
    for (; val > 0; val--)
        pp4m_HOOK_Remove(glo_debug_list);

    return 0;
}

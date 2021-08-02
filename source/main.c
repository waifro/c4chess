// standard libraries
#include <stdio.h>

// local libraries
#include "global.h"
#include "pp4m/pp4m.h"
#include "pp4m/pp4m_ttf.h"

#include "dashboard/gui.h"

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    glo_renderw = pp4m_Init(glo_window, "c4chess alpha-rewritten-testing", glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    while(1) {
        if (1 == 0) break;
        GUI_PopupWindow(300, 300, 275, 275, "hello");
    }

    pp4m_TTF_Quit();
    pp4m_Quit();

    return 0;
}

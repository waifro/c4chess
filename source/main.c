// standard libraries
#include <stdio.h>
#include <unistd.h>

#define SDL_MAIN_HANDLED

// local libraries
#include "global.h"
#include "pp4m/pp4m.h"
#include "pp4m/pp4m_ttf.h"

#include "chess/core.h"
#include "dashboard/gui.h"

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    getcwd(glo_current_dir, 255);
    // printf("\ncwd: %s\n", glo_current_dir);

    glo_render = pp4m_Init(glo_window, "c4chess alpha-rewritten-testing", glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    CORE_Testing(WHITE_PLAYER);
    //GUI_PopupWindow_Core(300, 300, 275, 275, "hello");

    //GUI_Testing();

    pp4m_TTF_Quit();
    pp4m_Quit();

    return 0;
}

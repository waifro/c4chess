// standard libraries
#include <stdio.h>
#include <unistd.h>

//#define SDL_MAIN_HANDLED

// local libraries
#include "global.h"
#include "pp4m/pp4m.h"
#include "pp4m/pp4m_ttf.h"

#include "version.h"
#include "chess/core.h"
#include "dashboard/gui.h"

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    char title_ver[256];
    sprintf(title_ver, "c4chess alpha-rewritten-testing %s", FULLVERSION_STRING);

    glo_render = pp4m_Init(glo_window, title_ver, glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    // main player (meaning choosen player)
    CORE_Testing(WHITE_PLAYER);

    // testing
    //GUI_PopupWindow_Core(300, 300, 275, 275, "hello");
    //GUI_Testing();

    // Exiting from game
    pp4m_TTF_Quit();
    pp4m_Quit(glo_window, glo_render);

    return 0;
}

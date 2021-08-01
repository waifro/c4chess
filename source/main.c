// standard libraries
#include <stdio.h>

// local libraries
#include "global.h"
#include "pp4m/pp4m.h"

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    glo_renderw = pp4m_Init(glo_window, "c4chess alpha-rewritten-testing", glo_screen_w, glo_screen_h);

    while(1) {
        if (1 == 0) break;
    }

    return 0;
}

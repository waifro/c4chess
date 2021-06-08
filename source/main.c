#include <stdio.h>
#include <stdlib.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_image.h"
#include "pp4m/pp4m.h"

SDL_Window *global_window = NULL;
SDL_Renderer *global_renderer = NULL;

int main (int argc, char *argv[]) {

    global_renderer = pp4m_Init(global_window, "scacchi", 1270, 720);

    GAME_Game();

    pp4m_Quit();

    return 0;
}

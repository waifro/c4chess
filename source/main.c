#include <stdio.h>
#include <stdlib.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_image.h"
#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "game.h"

SDL_Window *global_window;
SDL_Renderer *global_renderer;

int main (int argc, char *argv[]) {

    global_renderer = pp4m_Init(global_window, "scacchi", 1270, 720);

    SDL_Texture *background = pp4m_DRAW_TextureRect(global_renderer, PP4M_WHITE, NULL, 1280, 720, 1, 1);

    SDL_RenderClear(global_renderer);
    SDL_RenderCopy(global_renderer, background, NULL, NULL);
    SDL_RenderPresent(global_renderer);

    SDL_Delay(5000);

    /*
    GAME_CreateChessboard_RightTiles();

    SDL_Delay(5000);
    */
    
    SDL_DestroyRenderer(global_renderer);
    SDL_DestroyWindow(global_window);
    pp4m_Quit();

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_image.h"
#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "game.h"

SDL_Window *global_window = NULL;
SDL_Renderer *global_renderer;

int main (int argc, char *argv[]) {

    global_renderer = pp4m_Init(global_window, "scacchi", 1280, 720);
    
    PP4M_SDL background;
    background.texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_GREY_DARK, &background.rect, 1280, 720, 1, 1);

    SDL_RenderClear(global_renderer);
    SDL_RenderCopy(global_renderer, background.texture, NULL, NULL);
    SDL_RenderPresent(global_renderer);

    GAME_CreateChessboard_RightTiles();
    GAME_CreateChessboard_LeftTiles();

    PP4M_SDL BlackKing;
    BlackKing.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, "/home/waifro/Scrivania/projects/Scacchi-main/resources/bianco_re.png", &BlackKing.rect, 490, 160, 50, 50);

    SDL_RenderCopy(global_renderer, BlackKing.texture, NULL, &BlackKing.rect);
    SDL_RenderPresent(global_renderer);

    SDL_Delay(5000);

    SDL_DestroyTexture(background.texture);
    SDL_DestroyRenderer(global_renderer);
    SDL_DestroyWindow(global_window);
    pp4m_Quit();

    return 0;
}

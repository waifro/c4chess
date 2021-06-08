#include "pp4m/pp4m.h"
#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "game.h"

void GAME_CreateChessboard_RightTiles(void) {
    // destra

    int ChessTile_X = (1280 / 2) - 250;
    int ChessTile_Y = (720 / 2) - 350;

    PP4M_SDL RightTile[32];

    for(int n=0; n < 32; n++) {
      // starts from A tile

      if (n == 4) {
          // B tile
          ChessTile_X = (1280 / 2) - 350;
          ChessTile_Y += 100;

        } else if (n == 8) {
          // C tile
          ChessTile_X = (1280 / 2) - 250;
          ChessTile_Y += 100;

        } else if (n == 12) {
          // D tile
          ChessTile_X = (1280 / 2) - 350;
          ChessTile_Y += 100;

        } else if (n == 16) {
          // E tile
          ChessTile_X = (1280 / 2) - 250;
          ChessTile_Y += 100;

        } else if (n == 20) {
          // F tile
          ChessTile_X = (1280 / 2) - 350;
          ChessTile_Y += 100;

        } else if (n == 24) {
          // G tile
          ChessTile_X = (1280 / 2) - 250;
          ChessTile_Y += 100;

        } else if (n == 28) {
          // H tile
          ChessTile_X = (1280 / 2) - 350;
          ChessTile_Y += 100;

        }

      RightTile[n].texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_BLACK, &RightTile->rect, ChessTile_X, ChessTile_Y, 100, 100);
      ChessTile_X += 200;

    }

    return;
}

void GAME_CreateChessboard_LeftTiles(void) {
    // sinistra

    return;
}

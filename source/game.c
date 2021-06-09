#include <stdio.h>
#include <string.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "game.h"
#include "core.h"

void GAME_CreateChessboard_Tiles(void) {

  int ChessTile_X = (1280 / 2) - 200;
  int ChessTile_Y = (720 / 2) - 200;

  CORE_TILE tile[64];

  char text_row[] = "ABCDEFGH";
  int row_position = 0;

  int colomn = 1;
  char row = text_row[row_position];

  int move_row = 0;

  int change_color = 0;

  for (int n = 0; n < 64; n++) {

    if (row_position >= 8) {
      ChessTile_X = (SCREEN_WIDTH / 2) - 200;
      ChessTile_Y += 50;
      row_position = 0;
    }

    if (change_color == 0) {

      tile[n].pp4m->background = pp4m_DRAW_TextureRect(global_renderer, PP4M_WHITE, &tile[n].pp4m->rect, ChessTile_X, ChessTile_Y, 50, 50);

      sprintf(tile[n].position, "%d%c", colomn, row);
      row = text_row[row_position];
      ChessTile_X += 50;
      row_position += 1;
      change_color = 1;

      SDL_RenderCopy(global_renderer, tile[n].pp4m.background, NULL, &tile[n].pp4m->rect);

    }

    if (change_color == 1) {

      tile[n].pp4m.background = pp4m_DRAW_TextureRect(global_renderer, PP4M_BLACK, &tile.pp4m->rect, ChessTile_X, ChessTile_Y, 50, 50);

      sprintf(tile[n].position, "%d%c", colomn, row);
      row = text_row[row_position];
      ChessTile_X += 50;
      row_position += 1;
      change_color = 1;

      SDL_RenderCopy(global_renderer, tile[n].pp4m.background, NULL, &tile[n].pp4m->rect);

    }

  }

  SDL_RenderPresent(global_renderer);

  return;
}

void GAME_CreateChessboard_RightTiles(void) {
    // destra

    int ChessTile_X = (1280 / 2) - 150;
    int ChessTile_Y = (720 / 2) - 200;

    PP4M_SDL RightTile[32];

    for(int n=0; n < 32; n++) {
      // starts from A tile

      if (n == 4) {
          // B tile
          ChessTile_X = (1280 / 2) - 200;
          ChessTile_Y += 50;

        } else if (n == 8) {
          // C tile
          ChessTile_X = (1280 / 2) - 150;
          ChessTile_Y += 50;

        } else if (n == 12) {
          // D tile
          ChessTile_X = (1280 / 2) - 200;
          ChessTile_Y += 50;

        } else if (n == 16) {
          // E tile
          ChessTile_X = (1280 / 2) - 150;
          ChessTile_Y += 50;

        } else if (n == 20) {
          // F tile
          ChessTile_X = (1280 / 2) - 200;
          ChessTile_Y += 50;

        } else if (n == 24) {
          // G tile
          ChessTile_X = (1280 / 2) - 150;
          ChessTile_Y += 50;

        } else if (n == 28) {
          // H tile
          ChessTile_X = (1280 / 2) - 200;
          ChessTile_Y += 50;

        }

      RightTile[n].texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_GREY, &RightTile->rect, ChessTile_X, ChessTile_Y, 50, 50);
      ChessTile_X += 100;
      SDL_RenderCopy(global_renderer, RightTile[n].texture, NULL, &RightTile->rect);
    }

    return;
}

void GAME_CreateChessboard_LeftTiles(void) {
    // sinistra

    int ChessTile_X = (1280 / 2) - 200;
    int ChessTile_Y = (720 / 2) - 200;

    PP4M_SDL LeftTile[32];

    for(int n=0; n < 32; n++) {
      // starts from A tile

      if (n == 4) {
          // B tile
          ChessTile_X = (1280 / 2) - 150;
          ChessTile_Y += 50;

        } else if (n == 8) {
          // C tile
          ChessTile_X = (1280 / 2) - 200;
          ChessTile_Y += 50;

        } else if (n == 12) {
          // D tile
          ChessTile_X = (1280 / 2) - 150;
          ChessTile_Y += 50;

        } else if (n == 16) {
          // E tile
          ChessTile_X = (1280 / 2) - 200;
          ChessTile_Y += 50;

        } else if (n == 20) {
          // F tile
          ChessTile_X = (1280 / 2) - 150;
          ChessTile_Y += 50;

        } else if (n == 24) {
          // G tile
          ChessTile_X = (1280 / 2) - 200;
          ChessTile_Y += 50;

        } else if (n == 28) {
          // H tile
          ChessTile_X = (1280 / 2) - 150;
          ChessTile_Y += 50;

        }

      LeftTile[n].texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_WHITE, &LeftTile->rect, ChessTile_X, ChessTile_Y, 50, 50);
      ChessTile_X += 100;
      SDL_RenderCopy(global_renderer, LeftTile[n].texture, NULL, &LeftTile->rect);
    }

    return;
}

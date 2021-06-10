#include <stdio.h>
#include <string.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "game.h"
#include "core.h"

CORE_TILE[64];

void GAME_InitializeChessboard(void) {

  GAME_CreateChessboard_Tiles();

  GAME_InitializePieces();

  SDL_RenderPresent(global_renderer);

  SDL_Delay(5000);

  return;
}

void GAME_InitializePieces(void) {

  GAME_PIECE BlackPawn_A = { 0, 0, 7, 'A', BLACK_PAWN};
  GAME_PIECE BlackPawn_B = { 0, 0, 7, 'B', BLACK_PAWN};
  GAME_PIECE BlackPawn_C = { 0, 0, 7, 'C', BLACK_PAWN};
  GAME_PIECE BlackPawn_D = { 0, 0, 7, 'D', BLACK_PAWN};
  GAME_PIECE BlackPawn_E = { 0, 0, 7, 'E', BLACK_PAWN};
  GAME_PIECE BlackPawn_F = { 0, 0, 7, 'F', BLACK_PAWN};
  GAME_PIECE BlackPawn_G = { 0, 0, 7, 'G', BLACK_PAWN};
  GAME_PIECE BlackPawn_H = { 0, 0, 7, 'H', BLACK_PAWN};

  // IT DOESNT EXTRACT X, Y BECAUSE IT GOES TROW ONLY 8 TILES
  // from the tagged tiles, it extracts the x, y values
  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_A.colomn) > 0 && sizeof(BlackPawn_A.row) > 0) break;

    if (BlackPawn_A.colomn == tile[n].colomn) BlackPawn_A.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_A.row == tile[n].row) BlackPawn_A.x = tile[n].pp4m.rect.x;

  }

  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_B.colomn) > 0 && sizeof(BlackPawn_B.row) > 0) break;

    if (BlackPawn_B.colomn == tile[n].colomn) BlackPawn_B.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_B.row == tile[n].row) BlackPawn_B.x = tile[n].pp4m.rect.x;

  }

  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_C.colomn) > 0 && sizeof(BlackPawn_C.row) > 0) break;

    if (BlackPawn_C.colomn == tile[n].colomn) BlackPawn_C.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_C.row == tile[n].row) BlackPawn_C.x = tile[n].pp4m.rect.x;

  }

  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_D.colomn) > 0 && sizeof(BlackPawn_D.row) > 0) break;

    if (BlackPawn_D.colomn == tile[n].colomn) BlackPawn_D.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_D.row == tile[n].row) BlackPawn_D.x = tile[n].pp4m.rect.x;

  }

  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_E.colomn) > 0 && sizeof(BlackPawn_E.row) > 0) break;

    if (BlackPawn_E.colomn == tile[n].colomn) BlackPawn_E.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_E.row == tile[n].row) BlackPawn_E.x = tile[n].pp4m.rect.x;

  }

  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_F.colomn) > 0 && sizeof(BlackPawn_F.row) > 0) break;

    if (BlackPawn_F.colomn == tile[n].colomn) BlackPawn_F.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_F.row == tile[n].row) BlackPawn_F.x = tile[n].pp4m.rect.x;

  }

  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_G.colomn) > 0 && sizeof(BlackPawn_G.row) > 0) break;

    if (BlackPawn_G.colomn == tile[n].colomn) BlackPawn_G.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_G.row == tile[n].row) BlackPawn_G.x = tile[n].pp4m.rect.x;

  }

  for (int n = 0; n < 8; n++) {

    if (sizeof(BlackPawn_H.colomn) > 0 && sizeof(BlackPawn_H.row) > 0) break;

    if (BlackPawn_H.colomn == tile[n].colomn) BlackPawn_H.y = tile[n].pp4m.rect.y;
    for (int i = 0; i < 8; i++) if (BlackPawn_H.row == tile[n].row) BlackPawn_H.x = tile[n].pp4m.rect.x;

  }

  //WhitePawn_A = { 0, 0, 2, 'A', WHITE_PAWN};
  //WhitePawn_B = { 0, 0, 2, 'B', WHITE_PAWN};
  //WhitePawn_C = { 0, 0, 2, 'C', WHITE_PAWN};
  //WhitePawn_D = { 0, 0, 2, 'D', WHITE_PAWN};
  //WhitePawn_E = { 0, 0, 2, 'E', WHITE_PAWN};
  //WhitePawn_F = { 0, 0, 2, 'F', WHITE_PAWN};
  //WhitePawn_G = { 0, 0, 2, 'G', WHITE_PAWN};
  //WhitePawn_H = { 0, 0, 2, 'H', WHITE_PAWN};

  return;
}

void GAME_CreateChessboard_Tiles(void) {

  int ChessTile_X = (1280 / 2) - 200;
  int ChessTile_Y = (720 / 2) - 200;

  // assigning alphanumeric tags onto every tiles
  char text_row[] = "ABCDEFGH";
  int row_position = 0;

  int colomn = 8;

  // on every row the tile changes color, and on every colomn changes pattern
  int change_color = 0;

  for (int n = 0; n < 64; n++) {

    // on every 8 rows, the colomn goes down
    if (row_position > 7) {
      ChessTile_X = (SCREEN_WIDTH / 2) - 200;
      ChessTile_Y += 50;

      change_color ^= 1;

      colomn -= 1;
      row_position = 0;
    }

    if (change_color == 0) {

      tile[n].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_WHITE, &tile[n].pp4m.rect, ChessTile_X, ChessTile_Y, 50, 50);

      // tagging alphanumeric for each tile
      tile[n].colomn = colomn;
      tile[n].row = text_row[row_position];

      ChessTile_X += 50;
      row_position += 1;
      change_color ^= 1;

      SDL_RenderCopy(global_renderer, tile[n].pp4m.texture, NULL, &tile[n].pp4m.rect);

    }

    else if (change_color == 1) {

      tile[n].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_GREY, &tile[n].pp4m.rect, ChessTile_X, ChessTile_Y, 50, 50);

      // tagging alphanumeric for each tile
      tile[n].colomn = colomn;
      tile[n].row = text_row[row_position];

      ChessTile_X += 50;
      row_position += 1;
      change_color ^= 1;

      SDL_RenderCopy(global_renderer, tile[n].pp4m.texture, NULL, &tile[n].pp4m.rect);

    }

  }

  SDL_RenderPresent(global_renderer);

  return;
}

// deprecated.
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

// deprecated.
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

#include <stdio.h>
#include <string.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "game.h"
#include "core.h"

char *BLACK_PAWN = "resources/dpawn.png";
char *BLACK_KNIGHT = "resources/dknight.png";
char *BLACK_BISHOP = "resources/dbishop.png";
char *BLACK_ROOK = "resources/drook.png";
char *BLACK_QUEEN = "resources/dqueen.png";
char *BLACK_KING = "resources/dking.png";

char *WHITE_PAWN = "resources/wpawn.png";
char *WHITE_KNIGHT = "resources/wknight.png";
char *WHITE_BISHOP = "resources/wbishop.png";
char *WHITE_ROOK = "resources/wrook.png";
char *WHITE_QUEEN = "resources/wqueen.png";
char *WHITE_KING = "resources/wking.png";

CORE_TILE tile[64];

GAME_PIECE BlackPawn_A = {NULL, 0, 0, 0, 7, 'A'};
GAME_PIECE BlackPawn_B = { 0, 0, 0, 7, 'B'};
GAME_PIECE BlackPawn_C = { 0, 0, 0, 7, 'C'};
GAME_PIECE BlackPawn_D = { 0, 0, 0, 7, 'D'};
GAME_PIECE BlackPawn_E = { 0, 0, 0, 7, 'E'};
GAME_PIECE BlackPawn_F = { 0, 0, 0, 7, 'F'};
GAME_PIECE BlackPawn_G = { 0, 0, 0, 7, 'G'};
GAME_PIECE BlackPawn_H = { 0, 0, 0, 7, 'H'};

void GAME_InitializeChessboard(void) {

  GAME_CreateChessboard_Tiles();

  GAME_InitializePieces();

  SDL_RenderPresent(global_renderer);

  SDL_Delay(5000);

  return;
}


void GAME_UpdatePositionPiece(GAME_PIECE *piece, int colomn, char row) {

  int count = 0;
  for (int n = 0; n < 64; n++) {

    if (tile[n].colomn == piece->colomn) { piece->y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == piece->row) { piece->x = tile[n].pp4m.rect.x; count += 1; }

  if (count >= 2) break;
  }

  return;
}
void GAME_InitializePieces(void) {

  BlackPawn_A.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_A.pp4m.rect, 0, 0, 50, 50);
  BlackPawn_B.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_B.pp4m.rect, 0, 0, 50, 50);
  BlackPawn_C.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_C.pp4m.rect, 0, 0, 50, 50);
  BlackPawn_D.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_D.pp4m.rect, 0, 0, 50, 50);
  BlackPawn_E.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_E.pp4m.rect, 0, 0, 50, 50);
  BlackPawn_F.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_F.pp4m.rect, 0, 0, 50, 50);
  BlackPawn_G.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_G.pp4m.rect, 0, 0, 50, 50);
  BlackPawn_H.pp4m.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_H.pp4m.rect, 0, 0, 50, 50);

  int count = 0;
  for (int n = 0; n < 64; n++) {

    if (count >= 16) break;

    if (tile[n].colomn == BlackPawn_A.colomn) { BlackPawn_A.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_A.row) { BlackPawn_A.x = tile[n].pp4m.rect.x; count += 1; }

    if (tile[n].colomn == BlackPawn_B.colomn) { BlackPawn_B.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_B.row) { BlackPawn_B.x = tile[n].pp4m.rect.x; count += 1; }

    if (tile[n].colomn == BlackPawn_C.colomn) { BlackPawn_C.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_C.row) { BlackPawn_C.x = tile[n].pp4m.rect.x; count += 1; }

    if (tile[n].colomn == BlackPawn_D.colomn) { BlackPawn_D.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_D.row) { BlackPawn_D.x = tile[n].pp4m.rect.x; count += 1; }

    if (tile[n].colomn == BlackPawn_E.colomn) { BlackPawn_E.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_E.row) { BlackPawn_E.x = tile[n].pp4m.rect.x; count += 1; }

    if (tile[n].colomn == BlackPawn_F.colomn) { BlackPawn_F.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_F.row) { BlackPawn_F.x = tile[n].pp4m.rect.x; count += 1; }

    if (tile[n].colomn == BlackPawn_G.colomn) { BlackPawn_G.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_G.row) { BlackPawn_G.x = tile[n].pp4m.rect.x; count += 1; }

    if (tile[n].colomn == BlackPawn_H.colomn) { BlackPawn_H.y = tile[n].pp4m.rect.y; count += 1; }
    if (tile[n].row == BlackPawn_H.row) { BlackPawn_H.x = tile[n].pp4m.rect.x; count += 1; }

  }

  SDL_RenderCopy(global_renderer, BlackPawn_A.pp4m.texture, NULL, &BlackPawn_A.pp4m.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_B.pp4m.texture, NULL, &BlackPawn_B.pp4m.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_C.pp4m.texture, NULL, &BlackPawn_C.pp4m.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_D.pp4m.texture, NULL, &BlackPawn_D.pp4m.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_E.pp4m.texture, NULL, &BlackPawn_E.pp4m.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_F.pp4m.texture, NULL, &BlackPawn_F.pp4m.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_G.pp4m.texture, NULL, &BlackPawn_G.pp4m.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_H.pp4m.texture, NULL, &BlackPawn_H.pp4m.rect);


  // IT DOESNT EXTRACT X, Y BECAUSE IT GOES TROW ONLY 8 TILES
  // from the tagged tiles, it extracts the x, y values


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

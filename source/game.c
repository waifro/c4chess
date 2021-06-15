#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_io.h"
#include "pp4m/pp4m_draw.h"
#include "pp4m/pp4m_image.h"

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

GAME_PIECE BlackPawn_A;
GAME_PIECE BlackPawn_B;
GAME_PIECE BlackPawn_C;
GAME_PIECE BlackPawn_D;
GAME_PIECE BlackPawn_E;
GAME_PIECE BlackPawn_F;
GAME_PIECE BlackPawn_G;
GAME_PIECE BlackPawn_H;

GAME_PIECE BlackKnight_1;
GAME_PIECE BlackKnight_2;
GAME_PIECE BlackBishop_1;
GAME_PIECE BlackBishop_2;
GAME_PIECE BlackRook_1;
GAME_PIECE BlackRook_2;
GAME_PIECE BlackKing;
GAME_PIECE BlackQueen;

GAME_PIECE WhitePawn_A;
GAME_PIECE WhitePawn_B;
GAME_PIECE WhitePawn_C;
GAME_PIECE WhitePawn_D;
GAME_PIECE WhitePawn_E;
GAME_PIECE WhitePawn_F;
GAME_PIECE WhitePawn_G;
GAME_PIECE WhitePawn_H;

GAME_PIECE WhiteKnight_1;
GAME_PIECE WhiteKnight_2;
GAME_PIECE WhiteBishop_1;
GAME_PIECE WhiteBishop_2;
GAME_PIECE WhiteRook_1;
GAME_PIECE WhiteRook_2;
GAME_PIECE WhiteKing;
GAME_PIECE WhiteQueen;

void GAME_InitializeChessboard(void) {

  GAME_CreateChessboard_Tiles();

  GAME_InitializePieces();

  SDL_RenderCopy(global_renderer, BlackPawn_A.texture, NULL, &BlackPawn_A.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_B.texture, NULL, &BlackPawn_B.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_C.texture, NULL, &BlackPawn_C.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_D.texture, NULL, &BlackPawn_D.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_E.texture, NULL, &BlackPawn_E.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_F.texture, NULL, &BlackPawn_F.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_G.texture, NULL, &BlackPawn_G.rect);
  SDL_RenderCopy(global_renderer, BlackPawn_H.texture, NULL, &BlackPawn_H.rect);

  SDL_RenderCopy(global_renderer, WhitePawn_A.texture, NULL, &WhitePawn_A.rect);
  SDL_RenderCopy(global_renderer, WhitePawn_B.texture, NULL, &WhitePawn_B.rect);
  SDL_RenderCopy(global_renderer, WhitePawn_C.texture, NULL, &WhitePawn_C.rect);
  SDL_RenderCopy(global_renderer, WhitePawn_D.texture, NULL, &WhitePawn_D.rect);
  SDL_RenderCopy(global_renderer, WhitePawn_E.texture, NULL, &WhitePawn_E.rect);
  SDL_RenderCopy(global_renderer, WhitePawn_F.texture, NULL, &WhitePawn_F.rect);
  SDL_RenderCopy(global_renderer, WhitePawn_G.texture, NULL, &WhitePawn_G.rect);
  SDL_RenderCopy(global_renderer, WhitePawn_H.texture, NULL, &WhitePawn_H.rect);

  SDL_RenderCopy(global_renderer, BlackKnight_1.texture, NULL, &BlackKnight_1.rect);
  SDL_RenderCopy(global_renderer, BlackKnight_2.texture, NULL, &BlackKnight_2.rect);
  SDL_RenderCopy(global_renderer, BlackBishop_1.texture, NULL, &BlackBishop_1.rect);
  SDL_RenderCopy(global_renderer, BlackBishop_2.texture, NULL, &BlackBishop_2.rect);
  SDL_RenderCopy(global_renderer, BlackRook_1.texture, NULL, &BlackRook_1.rect);
  SDL_RenderCopy(global_renderer, BlackRook_2.texture, NULL, &BlackRook_2.rect);
  SDL_RenderCopy(global_renderer, BlackKing.texture, NULL, &BlackKing.rect);
  SDL_RenderCopy(global_renderer, BlackQueen.texture, NULL, &BlackQueen.rect);

  SDL_RenderCopy(global_renderer, WhiteKnight_1.texture, NULL, &WhiteKnight_1.rect);
  SDL_RenderCopy(global_renderer, WhiteKnight_2.texture, NULL, &WhiteKnight_2.rect);
  SDL_RenderCopy(global_renderer, WhiteBishop_1.texture, NULL, &WhiteBishop_1.rect);
  SDL_RenderCopy(global_renderer, WhiteBishop_2.texture, NULL, &WhiteBishop_2.rect);
  SDL_RenderCopy(global_renderer, WhiteRook_1.texture, NULL, &WhiteRook_1.rect);
  SDL_RenderCopy(global_renderer, WhiteRook_2.texture, NULL, &WhiteRook_2.rect);
  SDL_RenderCopy(global_renderer, WhiteKing.texture, NULL, &WhiteKing.rect);
  SDL_RenderCopy(global_renderer, WhiteQueen.texture, NULL, &WhiteQueen.rect);

  SDL_RenderPresent(global_renderer);

  SDL_Delay(5000);

  return;
}


void GAME_UpdatePositionPiece(GAME_PIECE *piece, int colomn, char row) {

  for (int n = 0; n < 64; n++) {

    if (row == tile[n].row) {
      piece->row = tile[n].row;
      piece->rect.x = tile[n].pp4m.rect.x;
    }

  }

  for (int n = 0; n < 64; n++) {

    if (colomn == tile[n].colomn) {
      piece->colomn = tile[n].colomn;
      piece->rect.y = tile[n].pp4m.rect.y;
    }

  }

  return;
}

void GAME_InitializePieces(void) {

  BlackPawn_A.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_A.rect, 0, 0, 50, 50);
  BlackPawn_B.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_B.rect, 0, 0, 50, 50);
  BlackPawn_C.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_C.rect, 0, 0, 50, 50);
  BlackPawn_D.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_D.rect, 0, 0, 50, 50);
  BlackPawn_E.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_E.rect, 0, 0, 50, 50);
  BlackPawn_F.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_F.rect, 0, 0, 50, 50);
  BlackPawn_G.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_G.rect, 0, 0, 50, 50);
  BlackPawn_H.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_PAWN, &BlackPawn_H.rect, 0, 0, 50, 50);

  BlackRook_1.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_ROOK, &BlackRook_1.rect, 0, 0, 50, 50);
  BlackRook_2.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_ROOK, &BlackRook_2.rect, 0, 0, 50, 50);
  BlackKnight_1.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_KNIGHT, &BlackKnight_1.rect, 0, 0, 50, 50);
  BlackKnight_2.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_KNIGHT, &BlackKnight_2.rect, 0, 0, 50, 50);
  BlackBishop_1.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_BISHOP, &BlackBishop_1.rect, 0, 0, 50, 50);
  BlackBishop_2.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_BISHOP, &BlackBishop_2.rect, 0, 0, 50, 50);
  BlackKing.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_KING, &BlackKing.rect, 0, 0, 50, 50);
  BlackQueen.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, BLACK_QUEEN, &BlackQueen.rect, 0, 0, 50, 50);

  // white pieces
  WhitePawn_A.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_A.rect, 0, 0, 50, 50);
  WhitePawn_B.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_B.rect, 0, 0, 50, 50);
  WhitePawn_C.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_C.rect, 0, 0, 50, 50);
  WhitePawn_D.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_D.rect, 0, 0, 50, 50);
  WhitePawn_E.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_E.rect, 0, 0, 50, 50);
  WhitePawn_F.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_F.rect, 0, 0, 50, 50);
  WhitePawn_G.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_G.rect, 0, 0, 50, 50);
  WhitePawn_H.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_PAWN, &WhitePawn_H.rect, 0, 0, 50, 50);

  WhiteRook_1.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_ROOK, &WhiteRook_1.rect, 0, 0, 50, 50);
  WhiteRook_2.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_ROOK, &WhiteRook_2.rect, 0, 0, 50, 50);
  WhiteKnight_1.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_KNIGHT, &WhiteKnight_1.rect, 0, 0, 50, 50);
  WhiteKnight_2.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_KNIGHT, &WhiteKnight_2.rect, 0, 0, 50, 50);
  WhiteBishop_1.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_BISHOP, &WhiteBishop_1.rect, 0, 0, 50, 50);
  WhiteBishop_2.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_BISHOP, &WhiteBishop_2.rect, 0, 0, 50, 50);
  WhiteKing.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_KING, &WhiteKing.rect, 0, 0, 50, 50);
  WhiteQueen.texture = pp4m_IMG_ImageToRenderer(global_renderer, NULL, WHITE_QUEEN, &WhiteQueen.rect, 0, 0, 50, 50);

  // from the tagged tiles, it extracts the x, y values
  GAME_UpdatePositionPiece(&BlackPawn_A, 7, 'A');
  GAME_UpdatePositionPiece(&BlackPawn_B, 7, 'B');
  GAME_UpdatePositionPiece(&BlackPawn_C, 7, 'C');
  GAME_UpdatePositionPiece(&BlackPawn_D, 7, 'D');
  GAME_UpdatePositionPiece(&BlackPawn_E, 7, 'E');
  GAME_UpdatePositionPiece(&BlackPawn_F, 7, 'F');
  GAME_UpdatePositionPiece(&BlackPawn_G, 7, 'G');
  GAME_UpdatePositionPiece(&BlackPawn_H, 7, 'H');

  GAME_UpdatePositionPiece(&BlackRook_1, 8, 'A');
  GAME_UpdatePositionPiece(&BlackKnight_1, 8, 'B');
  GAME_UpdatePositionPiece(&BlackBishop_1, 8, 'C');
  GAME_UpdatePositionPiece(&BlackKing, 8, 'D');
  GAME_UpdatePositionPiece(&BlackQueen, 8, 'E');
  GAME_UpdatePositionPiece(&BlackBishop_2, 8, 'F');
  GAME_UpdatePositionPiece(&BlackKnight_2, 8, 'G');
  GAME_UpdatePositionPiece(&BlackRook_2, 8, 'H');

  // white pieces
  GAME_UpdatePositionPiece(&WhitePawn_A, 2, 'A');
  GAME_UpdatePositionPiece(&WhitePawn_B, 2, 'B');
  GAME_UpdatePositionPiece(&WhitePawn_C, 2, 'C');
  GAME_UpdatePositionPiece(&WhitePawn_D, 2, 'D');
  GAME_UpdatePositionPiece(&WhitePawn_E, 2, 'E');
  GAME_UpdatePositionPiece(&WhitePawn_F, 2, 'F');
  GAME_UpdatePositionPiece(&WhitePawn_G, 2, 'G');
  GAME_UpdatePositionPiece(&WhitePawn_H, 2, 'H');

  GAME_UpdatePositionPiece(&WhiteRook_1, 1, 'A');
  GAME_UpdatePositionPiece(&WhiteKnight_1, 1, 'B');
  GAME_UpdatePositionPiece(&WhiteBishop_1, 1, 'C');
  GAME_UpdatePositionPiece(&WhiteKing, 1, 'D');
  GAME_UpdatePositionPiece(&WhiteQueen, 1, 'E');
  GAME_UpdatePositionPiece(&WhiteBishop_2, 1, 'F');
  GAME_UpdatePositionPiece(&WhiteKnight_2, 1, 'G');
  GAME_UpdatePositionPiece(&WhiteRook_2, 1, 'H');


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

      if (colomn <= 1) colomn = 1;
      else colomn -= 1;

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

      SDL_Delay(100);
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_io.h"
#include "pp4m/pp4m_ttf.h"
#include "pp4m/pp4m_draw.h"
#include "pp4m/pp4m_image.h"

#include "main.h"
#include "game.h"
#include "core.h"
#include "input.h"
#include "debug.h"

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

GAME_PLAYER global_player;

void GAME_InitializeChessboard(void) {

  PP4M_SDL background;
  background.texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_GREY_DARK, &background.rect, 1280, 720, 1, 1);

  GAME_CreateChessboard_Tiles();
  CORE_InitializationPoint();

  GAME_InitializePieces();

  int foo = -1; int keep = -1;
  int reset = 0;

  // starts white
  global_player = WHITE;

  SDL_Event event;

  while (SDL_PollEvent(&event) >= 0) {

    foo = INPUT_MouseInteractPiece(&event); // if mouse touches a tile, returns != -1

    if (foo != -1 && tile[foo].piece != NULL) {

            // if new position and "keep" is unused, change tile color, and print possible pattern
            if (foo != keep && keep == -1 && tile[foo].piece->player == global_player) {

                keep = foo;

                // Selecting the piece from tile.toggle
                tile[foo].toggle = true;

                SDL_DestroyTexture(tile[foo].pp4m.texture);
                tile[foo].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_BLUE, &tile[foo].pp4m.rect, tile[foo].pp4m.rect.x, tile[foo].pp4m.rect.y, 50, 50);

                CORE_CheckPieceMovement(foo);
            }

            else if (foo != keep && keep != -1) {

                // if deselected, or valid move is choosen, lets reset the toggles and color
                // look if is touching a valid position for the selected piece to move
                SDL_DestroyTexture(tile[keep].pp4m.texture);
                tile[keep].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, tile[keep].color, &tile[keep].pp4m.rect, tile[keep].pp4m.rect.x, tile[keep].pp4m.rect.y, 50, 50);

                // If touch is in a valid position, move piece ( also, tile[foo].toggle = false)
                if (point[foo].toggle == true) {
                        global_player = CORE_SwitchPlayerTurn(global_player);
                        CORE_UpdateMovementPieceFromPoint(foo);
                }

                reset = true;
                keep = -1;
            }

    // checks only if a possible valid move, otherwise it resets everything on empty tile
    } else if (foo != -1 && tile[foo].piece == NULL) {

        reset = true;
        if (point[foo].toggle == true) {
                global_player = CORE_SwitchPlayerTurn(global_player);
                CORE_UpdateMovementPieceFromPoint(foo);
        }

        SDL_DestroyTexture(tile[keep].pp4m.texture);
        tile[keep].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, tile[keep].color, &tile[keep].pp4m.rect, tile[keep].pp4m.rect.x, tile[keep].pp4m.rect.y, 50, 50);
    }

    // ternary operator
    sprintf(DebugInfo[0].text, "global_player: %s", (global_player==1?"black":"white"));
    DEBUG_WriteTextureFont(DebugInfo[0].text, 0);

    sprintf(DebugInfo[1].text, "foo: %c%d, tile[%d]", tile[foo].row, tile[foo].colomn, foo);
    DEBUG_WriteTextureFont(DebugInfo[1].text, 1);

    sprintf(DebugInfo[2].text, "keep: %c%d, tile[%d]", tile[keep].row, tile[keep].colomn, keep);
    DEBUG_WriteTextureFont(DebugInfo[2].text, 2);

    SDL_RenderClear(global_renderer);
    SDL_RenderCopy(global_renderer, background.texture, NULL, NULL);

    for (int n = 0; n < 64; n++) {

        // this renders the tiles.
        SDL_RenderCopy(global_renderer, tile[n].pp4m.texture, NULL, &tile[n].pp4m.rect);

        // this renders the pieces on board.
        if (tile[n].piece != NULL) SDL_RenderCopy(global_renderer, tile[n].piece->texture, NULL, &tile[n].piece->rect);

        // this resets everything gameplay-releated
        if (reset == true) {

          tile[n].toggle = false;
          point[n].toggle = false;
          if (n >= 63) reset = false;

        } else if (reset == false) {
          // this renders the points if toggled true
          if (point[n].toggle == true) SDL_RenderCopy(global_renderer, point[n].pp4m.texture, NULL, &point[n].pp4m.rect);

        }

    }

    DEBUG_Print(0);
    DEBUG_Print(1);
    DEBUG_Print(2);
    DEBUG_Print(3);
    DEBUG_Print(4);

    SDL_RenderPresent(global_renderer);

    if (INPUT_Exit(&event) == true) break;
  }

  return;
}

void GAME_UpdatePositionPiece(int pos, GAME_PIECE *piece, GAME_PLAYER player, GAME_IDENTIFIER identifier, int colomn, char row) {

    // new information trusted gets the array position of tile[]
    int foo;
    foo = CORE_ReturnTilePosition(colomn, row);

    // resets the pointer of previous tile
    if (pos != -1) tile[pos].piece = NULL;

    tile[foo].piece = piece;
    tile[foo].piece->rect.x = tile[foo].pp4m.rect.x;
    tile[foo].piece->rect.y = tile[foo].pp4m.rect.y;
    if (tile[foo].piece->identifier == 0) piece->identifier = identifier;
    if (tile[foo].piece->player == 0) piece->player = player;

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
  GAME_UpdatePositionPiece(-1, &BlackPawn_A, BLACK, DPAWN, 7, 'A');
  GAME_UpdatePositionPiece(-1, &BlackPawn_B, BLACK, DPAWN, 7, 'B');
  GAME_UpdatePositionPiece(-1, &BlackPawn_C, BLACK, DPAWN, 7, 'C');
  GAME_UpdatePositionPiece(-1, &BlackPawn_D, BLACK, DPAWN, 7, 'D');
  GAME_UpdatePositionPiece(-1, &BlackPawn_E, BLACK, DPAWN, 7, 'E');
  GAME_UpdatePositionPiece(-1, &BlackPawn_F, BLACK, DPAWN, 7, 'F');
  GAME_UpdatePositionPiece(-1, &BlackPawn_G, BLACK, DPAWN, 7, 'G');
  GAME_UpdatePositionPiece(-1, &BlackPawn_H, BLACK, DPAWN, 7, 'H');

  GAME_UpdatePositionPiece(-1, &BlackRook_1, BLACK, ROOK, 8, 'A');
  GAME_UpdatePositionPiece(-1, &BlackKnight_1, BLACK, KNIGHT, 8, 'B');
  GAME_UpdatePositionPiece(-1, &BlackBishop_1, BLACK, BISHOP, 8, 'C');
  GAME_UpdatePositionPiece(-1, &BlackQueen, BLACK, QUEEN, 8, 'D');
  GAME_UpdatePositionPiece(-1, &BlackKing, BLACK, KING, 8, 'E');
  GAME_UpdatePositionPiece(-1, &BlackBishop_2, BLACK, BISHOP, 8, 'F');
  GAME_UpdatePositionPiece(-1, &BlackKnight_2, BLACK, KNIGHT, 8, 'G');
  GAME_UpdatePositionPiece(-1, &BlackRook_2, BLACK, ROOK, 8, 'H');

  // white pieces
  GAME_UpdatePositionPiece(-1, &WhitePawn_A, WHITE, PAWN, 2, 'A');
  GAME_UpdatePositionPiece(-1, &WhitePawn_B, WHITE, PAWN, 2, 'B');
  GAME_UpdatePositionPiece(-1, &WhitePawn_C, WHITE, PAWN, 2, 'C');
  GAME_UpdatePositionPiece(-1, &WhitePawn_D, WHITE, PAWN, 2, 'D');
  GAME_UpdatePositionPiece(-1, &WhitePawn_E, WHITE, PAWN, 2, 'E');
  GAME_UpdatePositionPiece(-1, &WhitePawn_F, WHITE, PAWN, 2, 'F');
  GAME_UpdatePositionPiece(-1, &WhitePawn_G, WHITE, PAWN, 2, 'G');
  GAME_UpdatePositionPiece(-1, &WhitePawn_H, WHITE, PAWN, 2, 'H');

  GAME_UpdatePositionPiece(-1, &WhiteRook_1, WHITE, ROOK, 1, 'A');
  GAME_UpdatePositionPiece(-1, &WhiteKnight_1, WHITE, KNIGHT, 1, 'B');
  GAME_UpdatePositionPiece(-1, &WhiteBishop_1, WHITE, BISHOP, 1, 'C');
  GAME_UpdatePositionPiece(-1, &WhiteQueen, WHITE, QUEEN, 1, 'D');
  GAME_UpdatePositionPiece(-1, &WhiteKing, WHITE, KING, 1, 'E');
  GAME_UpdatePositionPiece(-1, &WhiteBishop_2, WHITE, BISHOP, 1, 'F');
  GAME_UpdatePositionPiece(-1, &WhiteKnight_2, WHITE, KNIGHT, 1, 'G');
  GAME_UpdatePositionPiece(-1, &WhiteRook_2, WHITE, ROOK, 1, 'H');


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
      tile[n].color = PP4M_WHITE;

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
      tile[n].color = PP4M_GREY;

      ChessTile_X += 50;
      row_position += 1;
      change_color ^= 1;

      SDL_RenderCopy(global_renderer, tile[n].pp4m.texture, NULL, &tile[n].pp4m.rect);

    }

  }

  SDL_RenderPresent(global_renderer);

  return;
}

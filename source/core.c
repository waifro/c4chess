#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "core.h"
#include "game.h"

// initialization of global (external) variable
CORE_TILE tile[64];
CORE_TILE point[64];

int CORE_ReturnTilePosition(int colomn, char row) {

    int z = 0;
    for (int n = 0; n < 8; n++) {

        if (colomn == tile[z].colomn)
        for (int i = 0; i < 8; i++) {

            if (row == tile[z].row) {
                return z;
            }
            z += 1;
        }
        z += 8;
    }

    return -1;
}

int CORE_CheckPieceMovement(int pos) {

  switch (tile[pos].piece->identifier) {

    case (DPAWN):
    CORE_CreatePatternDarkPawn(pos);
    break;

    case (PAWN):
    CORE_CreatePatternPawn(pos);
    break;

    case (KNIGHT):
    CORE_CheckMovementKnight(pos);
    break;

    case (BISHOP):
    CORE_CheckMovementBishop(pos);
    break;

    case (ROOK):
    CORE_CheckMovementRook(pos);
    break;

    case (QUEEN):
    CORE_CheckMovementQueen(pos);
    break;

    case (KING):
    CORE_CheckMovementKing(pos);
    break;

  }

  return 0;
}

int CORE_UpdateMovementPieceFromPoint(int pos) {

  // bar = tile selected with old position of piece
  int bar = -1;
  for (int n = 0; n < 64; ++n) if (tile[n].toggle == true) { bar = n; break; }
  if (bar == -1) return -1;

  tile[bar].toggle = false;

  // it dosent turn off the toggles behind, cousing to move to a different position prevoiusly selected pieces
  GAME_UpdatePositionPiece(tile[bar].piece->player, tile[bar].piece, tile[bar].piece->identifier, tile[pos].colomn, tile[pos].row);

  return 0;
}

int CORE_SwitchPlayerTurn(int player) {

    // white
    if (player == 2) player = 1;
    else if (player == 1) player = 2;

    return player;
}

int CORE_CheckCapturePiece_DarkPawn(int pos) {

    if (tile[pos+7].piece != NULL && tile[pos+7].piece->player == WHITE) point[pos+7].toggle = true;
    if (tile[pos+9].piece != NULL && tile[pos+9].piece->player == WHITE) point[pos+9].toggle = true;

    return 0;
}

int CORE_CheckCapturePiece_Pawn(int pos) {

    if (tile[pos-9].piece != NULL && tile[pos-9].piece->player == BLACK) point[pos-9].toggle = true;
    if (tile[pos-7].piece != NULL && tile[pos-7].piece->player == BLACK) point[pos-7].toggle = true;

    return 0;
}

int CORE_CreatePatternDarkPawn(int pos) {

  // better ro use colomns and rows for creation of valid moves (using CORE_ReturnTilePosition())
  // it needs to check if the "valid" position is outside of reach (ex. pawn on H3 -> not valid A1 || or pawn on A1 -> not valid H8)
  if (tile[pos].piece->player == BLACK && tile[pos].piece->identifier == DPAWN) {

    if (tile[pos].colomn == 7) {

      if (tile[pos+8].piece == NULL) point[pos+8].toggle = true;
      if (tile[pos+16].piece == NULL) point[pos+16].toggle = true;

    } else if (tile[pos].colomn != 7) if (tile[pos+8].piece == NULL) point[pos+8].toggle = true;

    CORE_CheckCapturePiece_DarkPawn(pos);
  }

  return 0;
}

int CORE_CreatePatternPawn(int pos) {

  // better ro use colomns and rows for creation of valid moves (using CORE_ReturnTilePosition())
  // it needs to check if the "valid" position is outside of reach (ex. pawn on H3 -> not valid A1 || or pawn on A1 -> not valid H8)
  if (tile[pos].piece->player == WHITE && tile[pos].piece->identifier == PAWN) {

    if (tile[pos].colomn == 2) {

      if (tile[pos-8].piece == NULL) point[pos-8].toggle = true;
      if (tile[pos-16].piece == NULL) point[pos-16].toggle = true;

    } else if (tile[pos].colomn != 2) if (tile[pos-8].piece == NULL) point[pos-8].toggle = true;

    CORE_CheckCapturePiece_Pawn(pos);
  }

  return 0;
}

int CORE_CheckMovementKnight(int pos) {



  return 0;
}

int CORE_CheckMovementBishop(int pos){return 0;}
int CORE_CheckMovementRook(int pos){return 0;}
int CORE_CheckMovementQueen(int pos){return 0;}
int CORE_CheckMovementKing(int pos){return 0;}

void CORE_InitializationPoint(void) {

  // initialization of points which goes ontop of tiles for creating patterns (a single automated switchable on/off toggle)

  int PointTile_X = (1280 / 2) - 190;
  int PointTile_Y = (720 / 2) - 190;

  int row_position = 0;

  SDL_Color fade_black = {0, 0, 0, 255};

  for (int n = 0; n < 64; n++) {

    // on every 8 rows, the colomn goes down
    if (row_position > 7) {
      PointTile_X = (SCREEN_WIDTH / 2) - 190;
      PointTile_Y += 50;
      row_position = 0;
    }

    point[n].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, fade_black, &point[n].pp4m.rect, PointTile_X, PointTile_Y, 30, 30);
    SDL_SetTextureBlendMode(point[n].pp4m.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(point[n].pp4m.texture, 100);

    PointTile_X += 50;
    row_position += 1;
  }

  return;
}

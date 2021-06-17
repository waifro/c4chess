#include "core.h"
#include "game.h"

// initialization of global (external) variable
CORE_TILE tile[64];

// initialization of points which goes ontop of tiles for creating patterns (a single automated switchable on/off toggle)
PP4M_SDL point[64];
point.rect = {0, 0, 25, 25};
point.color = {0, 0, 0, 100};

void CORE_InitializationPoint(void) {

  for (int n = 0; n < 64; n++) {

    

  }

}

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

int CORE_PieceIdentification(int pos) {

  switch (tile[pos].piece->identifier) {

    case (DPAWN):
    CORE_CheckMovementDarkPawn(tile[pos].piece, );
    break;

    case (PAWN):
    CORE_CheckMovementPawn(tile[pos].piece);
    break;

    case (KNIGHT):
    CORE_CheckMovementKnight(tile[pos].piece);
    break;

    case (BISHOP):
    CORE_CheckMovementBishop(tile[pos].piece);
    break;

    case (ROOK):
    CORE_CheckMovementRook(tile[pos].piece);
    break;

    case (QUEEN):
    CORE_CheckMovementQueen(tile[pos].piece);
    break;

    case (KING):
    CORE_CheckMovementKing(tile[pos].piece);
    break;

  }

}

pp4m_DRAW_TextureRect(global_renderer, tile[foo].color[1], &tile[foo].pp4m.rect, tile[foo].pp4m.rect.x, tile[foo].pp4m.rect.y, 50, 50);

int CORE_CreatePatternDarkPawn(int pos) {

  if (tile[pos].colomn == 7) {

    pp4m_DRAW_TextureRect(global_renderer, )
  }

  tile[pos].piece

  return 0;
}

int CORE_CheckMovementPawn(GAME_PIECE *piece, CORE_MOVEMENT movement, int space) {
  // "int space" is for how many tiles to move from point A to B

  // if addon is more then z tile, exit
  int addon = piece->colomn + space;

  // if movement not correct, exit
  if (movement != UP) return -1;

  // if movement goes out of scope, exit
  else if (addon > 8) return -2;

  // else, update position
  GAME_UpdatePositionPiece(piece, PAWN, addon, piece->row);
  return 0;
}

int CORE_CheckMovementDarkPawn(GAME_PIECE *piece, CORE_MOVEMENT movement, int space) {
  // "int space" is for how many tiles to move from point A to B

  // if addon is more then z tile, exit
  int addon = piece->colomn - space;

  // if movement not correct, exit
  if (movement != DOWN) return -1;

  // if movement goes out of scope, exit
  else if (addon < 1) return -2;

  // else, update position
  else GAME_UpdatePositionPiece(piece, DPAWN, addon, piece->row);
  return 0;
}

int CORE_CheckMovementKnight(GAME_PIECE *piece, CORE_MOVEMENT movement, int space) {
  // "int space" is for how many tiles to move from point A to B
  // needs to be finished

  // if addon is more then z tile, exit
  int addon = piece->colomn + space;

  // if movement not correct, exit
  if (movement != UP) return -1;

  // if movement goes out of scope, exit
  else if (addon > 8) return -2;

  // else, update position
  else GAME_UpdatePositionPiece(piece, KNIGHT, addon, piece->row);
  return 0;
}

#include "core.h"
#include "game.h"

CORE_TILE tile[64];

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


/*
int CORE_CheckMovementDarkPawn(GAME_PIECE *piece, CORE_MOVEMENT movement) {
  if (movement != DOWN) return -1;
  else GAME_UpdatePositionPiece(piece, (piece->colomn++), row);
  return 0;
}


int CORE_MovementPiece(GAME_PIECE *piece, CORE_MOVEMENT movement) {

  //ciao

  return 0;
}

int CORE_MovementPawn(GAME_PIECE *piece, CORE_MOVEMENT movement) {

  //ciao

  return 0;
}

*/


/* obsolete
int CORE_MovementPawn(GAME_PIECE *piece) {

    // up
    if ((piece->colomn + 1) > 8) return -1;
    else return 0;

}

int CORE_MovementDarkPawn(GAME_PIECE *piece) {

    // down
    if ((piece->colomn - 1) < 1) return -1;
    else return 0;

}

int CORE_MovementRook(GAME_PIECE *piece) {

    // up
    if ((piece->colomn + 1) > 8) return -1;
    else return 0;

}
*/

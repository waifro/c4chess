#include <stdio.h>

#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "core.h"
#include "game.h"
#include "debug.h"

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

int CORE_ReturnValidTilePosition(int colomn, char row) {

    int foo = -1;
    foo = CORE_ReturnTilePosition(colomn, row);

    // non esiste un pezzo all'interno della casella, dunque movimento valido
    if (tile[foo].piece == NULL) return foo;
    // altrimenti movimento non valido
    else return -1;
}

int CORE_ReturnRowPosition(char row) {
    int foo = -1;

    char alpha[] = "ABCDEFGH";

    for (int n = 0; n < 8; n++) {

        if (alpha[n] == row) { foo = n; break; }

    }

    return foo;
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
    CORE_CreatePatternKnight(pos);
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
  for (int n = 0; n < 64; n++) if (tile[n].toggle == true) { bar = n; break; }
  if (bar == -1) return -1;

  tile[bar].toggle = false;

  // it dosent turn off the toggles behind, cousing to move to a different position prevoiusly selected pieces
  GAME_UpdatePositionPiece(bar, tile[bar].piece, tile[bar].piece->player, tile[bar].piece->identifier, tile[pos].colomn, tile[pos].row);

  return 0;
}

int CORE_SwitchPlayerTurn(int player) {

    // white
    if (player == 2) player = 1;
    else if (player == 1) player = 2;

    return player;
}

int CORE_CheckCapturePiece_DarkPawn(int pos) {

    int colomn = (tile[pos].colomn) - 1;
    char row = tile[pos].row;

    int row_pos = (CORE_ReturnRowPosition(row)) - 1;
    char alpha[] = "ABCDEFGH";

    int foo;
    static int debug = 3;

    for (int n = 0; n < 3; n++) {

        if (colomn < 1 || row_pos > 7) break;
        if (n == 1) { row_pos += 1; continue; }

        foo = CORE_ReturnTilePosition(colomn, alpha[row_pos]);

        if (tile[foo].piece != NULL && tile[foo].piece->player == WHITE) {

            sprintf(DebugInfo[debug].text, "tile[%d].piece: 0x%x", foo, &tile[foo].piece);
            DEBUG_WriteTextureFont(DebugInfo[debug].text, debug);

            point[foo].toggle = true;
            debug += 1;
        }

        row_pos += 1;

    }

    debug = 3;
    return 0;
}

int CORE_CheckCapturePiece_Pawn(int pos) {

    int colomn = (tile[pos].colomn) + 1;
    char row = tile[pos].row;

    int row_pos = (CORE_ReturnRowPosition(row)) - 1;
    char alpha[] = "ABCDEFGH";

    int foo;

    for (int n = 0; n < 3; n++) {

        if (colomn > 8 || row_pos > 7) break;
        if (n == 1) { row_pos += 1; continue; }

        foo = CORE_ReturnTilePosition(colomn, alpha[row_pos]);

        if (tile[foo].piece != NULL && tile[foo].piece->player == BLACK) point[foo].toggle = true;

        row_pos += 1;

    }

    return 0;
}

// activates point[n].toggle for the patterns
int CORE_CreatePatternDarkPawn(int pos) {

    if (tile[pos].piece->player == BLACK && tile[pos].piece->identifier == DPAWN) {

        int foo = -1;
        int colomn = (tile[pos].colomn) - 1;
        char row = tile[pos].row;

            if (tile[pos].colomn == 7) {

                for (int n = 0; n < 2; n++) {

                    foo = CORE_ReturnValidTilePosition(colomn, row);
                    if (foo == -1) return foo;

                    point[foo].toggle = true;

                    colomn -= 1;
                    if (colomn == 0) return -1;
                }

            } else if (tile[pos].colomn != 7) {

                foo = CORE_ReturnValidTilePosition(colomn, row);
                if (foo == -1) return foo;

                point[foo].toggle = true;

            }

            CORE_CheckCapturePiece_DarkPawn(pos);

    }

    return 0;
}

int CORE_CreatePatternPawn(int pos) {

    if (tile[pos].piece->player == WHITE && tile[pos].piece->identifier == PAWN) {

        int foo = -1;
        int colomn = (tile[pos].colomn) + 1;
        char row = tile[pos].row;

            if (tile[pos].colomn == 2) {

                for (int n = 0; n < 2; n++) {

                    foo = CORE_ReturnValidTilePosition(colomn, row);
                    if (foo == -1) return foo;

                    point[foo].toggle = true;

                    colomn += 1;
                    if (colomn == 0) return -1;
                }

            } else if (tile[pos].colomn != 2) {

                foo = CORE_ReturnValidTilePosition(colomn, row);
                if (foo == -1) return foo;

                point[foo].toggle = true;

            }

            CORE_CheckCapturePiece_Pawn(pos);

    }

    return 0;
}




int CORE_CreatePatternKnight(int pos) {

    if (tile[pos].piece->identifier == KNIGHT) {

        point[pos-16+1].toggle = true;

    }

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

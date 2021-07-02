#include <stdio.h>

#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "core.h"
#include "game.h"
#include "debug.h"
#include "fs_management.h"

// initialization of global (external) variable
CORE_TILE tile[64];
CORE_TILE point[64];

CORE_CASTLING global_whitecastling;
CORE_CASTLING global_blackcastling;

int global_castling;

int CORE_ReturnTilePosition(int colomn, char row) {

    if (colomn < 1 || colomn > 8) return -1;
    if (CORE_ReturnRowPosition(row) == -1) return -1;

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
        CORE_CreatePatternBishop(pos);
        break;

        case (ROOK):
        CORE_CreatePatternRook(pos);
        break;

        case (QUEEN):
        CORE_CreatePatternQueen(pos);
        break;

        case (KING):
        CORE_CreatePatternKing(pos);
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
  FS_RecordPieceMovement(bar, pos);

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

    // needs to be added en passant after recording the moves
    int colomn = (tile[pos].colomn) - 1;
    char row = tile[pos].row;

    int row_pos = (CORE_ReturnRowPosition(row)) - 1;
    char alpha[] = "ABCDEFGH";

    int foo;

    for (int n = 0; n < 3; n++) {

        if (colomn < 1 || row_pos > 7) break;
        if (n == 1) { row_pos += 1; continue; }

        foo = CORE_ReturnTilePosition(colomn, alpha[row_pos]);

        if (tile[foo].piece != NULL && tile[foo].piece->player == WHITE) point[foo].toggle = true;

        row_pos += 1;

    }

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

int CORE_CheckCapturePiece(int colomn, char row) {

    int foo;
    foo = CORE_ReturnTilePosition(colomn, row);

    if (tile[foo].piece != NULL && tile[foo].piece->player != global_player) point[foo].toggle = true;

    return 0;
}

int GAME_UpdateRookCastling(int pos) {

    // new information trusted gets the array position of tile[]
    int foo;
    int bar;

    if (tile[pos].colomn == 1 || tile[pos].colomn == 8) {

      if (tile[pos].row == 'C') {
        foo = CORE_ReturnTilePosition(tile[pos].colomn, 'D');
        bar = CORE_ReturnTilePosition(tile[pos].colomn, 'A');

        tile[foo].piece = tile[bar].piece;
        tile[foo].piece->rect.x = tile[foo].pp4m.rect.x;
        tile[foo].piece->rect.y = tile[foo].pp4m.rect.y;

        // resets the pointer of previous tile
        tile[bar].piece = NULL;

        // passing instruction for FS_RecordPieceMovement to castle (LONG)
        global_castling = 1;
        FS_RecordPieceMovement(0, 0);
      }

      else if (tile[pos].row == 'G') {
        foo = CORE_ReturnTilePosition(tile[pos].colomn, 'F');
        bar = CORE_ReturnTilePosition(tile[pos].colomn, 'H');

        tile[foo].piece = tile[bar].piece;
        tile[foo].piece->rect.x = tile[foo].pp4m.rect.x;
        tile[foo].piece->rect.y = tile[foo].pp4m.rect.y;

        // resets the pointer of previous tile
        tile[bar].piece = NULL;

        // passing instruction for FS_RecordPieceMovement to castle (SHORT)
        global_castling = 2;
        FS_RecordPieceMovement(0, 0);
      }
    }

    return 0;
}

int CORE_UpdateValidCastling(int new, int old) {

    if (old == -1) {
      global_whitecastling = BOTH;
      global_blackcastling = BOTH;
    }

    if (old != -1) switch (tile[new].piece->identifier) {

      case (KING):

      if (global_player == WHITE) {

        if (new == -1) global_whitecastling = BOTH;
        else if (new != -1) {
          global_whitecastling = DISABLE;
          GAME_UpdateRookCastling(new);
        }

      } else if (global_player == BLACK) {

        if (new == -1) global_blackcastling = BOTH;
        else if (new != -1) {
          global_blackcastling = DISABLE;
          GAME_UpdateRookCastling(new);
        }
      }

      break;
    case (ROOK):

        if (tile[new].row == 'A') {

          if (global_player == WHITE && global_whitecastling != DISABLE) {

            // if rook H already moved, disable castling
            if (global_whitecastling == LONG) global_whitecastling = DISABLE;
            // if none rooks has been moved, just short castling
            else if (global_whitecastling == NONE || global_whitecastling == BOTH) global_whitecastling = SHORT;

          } else if (global_player == BLACK && global_blackcastling != DISABLE) {

            // if rook H already moved, disable castling
            if (global_blackcastling == LONG) global_blackcastling = DISABLE;
            // if none rooks has been moved, just short castling
            else if (global_blackcastling == NONE || global_blackcastling == BOTH) global_blackcastling = SHORT;

          }
        }

        else if (tile[new].row == 'H') {

          if (global_player == WHITE && global_whitecastling != DISABLE) {

            // if rook A already moved, disable castling
            if (global_whitecastling == SHORT) global_whitecastling = DISABLE;
            // if none rooks has been moved, just short castling
            else if (global_whitecastling == NONE || global_whitecastling == BOTH) global_whitecastling = LONG;

          } else if (global_player == BLACK && global_blackcastling != DISABLE) {

            // if rook A already moved, disable castling
            if (global_blackcastling == SHORT) global_blackcastling = DISABLE;
            // if none rooks has been moved, just short castling
            else if (global_blackcastling == NONE || global_blackcastling == BOTH) global_blackcastling = LONG;

          }

        }
        break;
        default: break;
      }

    return 0;
}

int CORE_CheckKingCastling(CORE_CASTLING castle, int pos) {

    if (castle == DISABLE) return -1;

    int colomn = tile[pos].colomn;
    char row[] = "ABCDEFGH";

    if (castle == BOTH) {

        int foo;
        for (int n = 1; n < 4; n++) {

            foo = CORE_ReturnValidTilePosition(colomn, row[n]);
            if (foo == -1) break;

            // castling long is available
            if (n == 3) {
                    int bar = CORE_ReturnTilePosition(colomn, row[2]);
                    point[bar].toggle = true;
            }

        }

        for (int n = 5; n < 7; n++) {

            foo = CORE_ReturnValidTilePosition(colomn, row[n]);
            if (foo == -1) break;

            // castling short is available
            if (n == 6) {
                    int bar = CORE_ReturnTilePosition(colomn, row[6]);
                    point[bar].toggle = true;
            }
        }
    }

    else if (castle == LONG) {

        int foo;
        for (int n = 1; n < 4; n++) {

            foo = CORE_ReturnValidTilePosition(colomn, row[n]);
            if (foo == -1) break;

            // castling long is available
            if (n == 3) {
                    int bar = CORE_ReturnTilePosition(colomn, row[2]);
                    point[bar].toggle = true;
            }
        }
    }

    else if (castle == SHORT) {

        int foo;
        for (int n = 5; n < 7; n++) {

            foo = CORE_ReturnValidTilePosition(colomn, row[n]);
            if (foo == -1) break;

            // castling short is available
            if (n == 6) {
                    int bar = CORE_ReturnTilePosition(colomn, row[6]);
                    point[bar].toggle = true;
            }
        }
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
                    if (foo == -1) break;

                    point[foo].toggle = true;

                    colomn -= 1;
                    if (colomn < 1) break;
                }

            } else if (tile[pos].colomn != 7) {

                foo = CORE_ReturnValidTilePosition(colomn, row);
                if (foo != -1) point[foo].toggle = true;

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
                    if (foo == -1) break;

                    point[foo].toggle = true;

                    colomn += 1;
                    if (colomn == 9) break;
                }

            } else if (tile[pos].colomn != 2) {

                foo = CORE_ReturnValidTilePosition(colomn, row);
                if (foo != -1) point[foo].toggle = true;

            }

            CORE_CheckCapturePiece_Pawn(pos);

    }

    return 0;
}

int CORE_CreatePatternKnight(int pos) {

    if (tile[pos].piece->player == global_player && tile[pos].piece->identifier == KNIGHT) {

        int colomn = tile[pos].colomn;

        int row_pos = CORE_ReturnRowPosition(tile[pos].row);
        char alpha[] = "ABCDEFGH";

        int foo;

        for (int n = 0; n < 4; n++) {

            if (n == 0) row_pos -= 2;
            if (n == 1) row_pos += 1;
            if (n == 2) row_pos += 2;
            if (n == 3) row_pos += 1;

            if (n == 0) colomn -= 1;
            // 2
            if (n == 1) colomn -= 3;
            // 4
            if (n == 2) colomn -= 4;
            // 6
            if (n == 3) colomn -= 3;

            for (int i = 0; i < 2; i++) {

                // 1
                if (n == 0 && i == 1) colomn += 2;
                // 3
                if (n == 1 && i == 1) colomn += 4;
                // 5
                if (n == 2 && i == 1) colomn += 4;
                // 7
                if (n == 3 && i == 1) colomn += 2;

                if (colomn < 1 || colomn > 8) continue;
                if (row_pos < 0 || row_pos > 7) continue;

                foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
                if (foo != -1) point[foo].toggle = true;

                CORE_CheckCapturePiece(colomn, alpha[row_pos]);

            }

        }

    }

    return 0;
}

int CORE_CreatePatternBishop(int pos) {

    if (tile[pos].piece->player == global_player && (tile[pos].piece->identifier == BISHOP || tile[pos].piece->identifier == QUEEN)) {

        int colomn = tile[pos].colomn;

        int row_pos = CORE_ReturnRowPosition(tile[pos].row);
        char alpha[] = "ABCDEFGH";

        int foo;

        for (int n = 0; n < 8; n++) {

            colomn += 1;
            row_pos += 1;

            if (colomn > 8) break;
            if (row_pos > 7) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }
        }

        colomn = tile[pos].colomn;
        row_pos = CORE_ReturnRowPosition(tile[pos].row);

        for (int n = 0; n < 8; n++) {

            colomn -= 1;
            row_pos += 1;

            if (colomn < 1) break;
            if (row_pos > 7) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }

        }

        colomn = tile[pos].colomn;
        row_pos = CORE_ReturnRowPosition(tile[pos].row);

        for (int n = 0; n < 8; n++) {

            colomn += 1;
            row_pos -= 1;

            if (colomn > 8) break;
            if (row_pos < 0) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }
        }

        colomn = tile[pos].colomn;
        row_pos = CORE_ReturnRowPosition(tile[pos].row);

        for (int n = 0; n < 8; n++) {

            colomn -= 1;
            row_pos -= 1;

            if (colomn < 1) break;
            if (row_pos < 0) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }
        }
    }

    return 0;
}

int CORE_CreatePatternRook(int pos) {

    if (tile[pos].piece->player == global_player && (tile[pos].piece->identifier == ROOK || tile[pos].piece->identifier == QUEEN)) {

        int colomn = tile[pos].colomn;

        int row_pos = CORE_ReturnRowPosition(tile[pos].row);
        char alpha[] = "ABCDEFGH";

        int foo;

        for (int n = 0; n < 8; n++) {

            colomn += 1;

            if (colomn > 8) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }
        }

        colomn = tile[pos].colomn;
        row_pos = CORE_ReturnRowPosition(tile[pos].row);

        for (int n = 0; n < 8; n++) {

            colomn -= 1;

            if (colomn < 1) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }

        }

        colomn = tile[pos].colomn;
        row_pos = CORE_ReturnRowPosition(tile[pos].row);

        for (int n = 0; n < 8; n++) {

            row_pos -= 1;

            if (row_pos < 0) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }
        }

        colomn = tile[pos].colomn;
        row_pos = CORE_ReturnRowPosition(tile[pos].row);

        for (int n = 0; n < 8; n++) {

            row_pos += 1;

            if (row_pos > 8) break;

            foo = CORE_ReturnValidTilePosition(colomn, alpha[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, alpha[row_pos]);
                break;
            }
        }
    }

    return 0;
}

int CORE_CreatePatternQueen(int pos) {

    if (tile[pos].piece->player == global_player && tile[pos].piece->identifier == QUEEN) {
        CORE_CreatePatternBishop(pos);
        CORE_CreatePatternRook(pos);
    }

    return 0;
}

int CORE_CreatePatternKing(int pos) {

    if (tile[pos].piece->player == global_player) {

        int colomn = tile[pos].colomn;

        int row_pos = CORE_ReturnRowPosition(tile[pos].row);
        char row[] = "ABCDEFGH";

        int foo;
        for (int n = 0; n < 8; n++) {

            if (n == 0) {
                colomn += 1;
                row_pos -= 1;
            }

            if (n == 1) {
                row_pos += 1;
            }

            if (n == 2) {
                row_pos += 1;
            }

            if (n == 3) {
                colomn -= 1;
            }

            if (n == 4) {
                colomn -= 1;
            }

            if (n == 5) {
                row_pos -= 1;
            }

            if (n == 6) {
                row_pos -= 1;
            }

            if (n == 7) {
                colomn += 1;
            }

            if (row_pos < 1 || row_pos > 8) continue;
            if (colomn < 1 || colomn > 8) continue;

            foo = CORE_ReturnValidTilePosition(colomn, row[row_pos]);
            if (foo != -1) point[foo].toggle = true;
            else if (foo == -1) {
                CORE_CheckCapturePiece(colomn, row[row_pos]);
                continue;
            }
        }

        if (global_player == WHITE) CORE_CheckKingCastling(global_whitecastling, pos);
        else if (global_player == BLACK) CORE_CheckKingCastling(global_blackcastling, pos);
    }

    return 0;
}

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

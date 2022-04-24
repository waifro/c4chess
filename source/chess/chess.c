#include <stdbool.h>

#include "event.h"
#include "chess.h"
#include "middle.h"
#include "core.h"
#include "dot.h"

int CHESS_RedirectPiecePattern(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    switch(glo_chess_core_tile[tile].piece->enum_piece) {
        case KING: CHESS_PiecePattern_King(tile, player, check);
        break;
        case BKING: CHESS_PiecePattern_King(tile, player, check);
        break;
        case PAWN: CHESS_PiecePattern_Pawn(tile, player, check);
        break;
        case BPAWN: CHESS_PiecePattern_BPawn(tile, player, check);
        break;
        case KNIGHT: CHESS_PiecePattern_Knight(chess_tile, tile, player, check);
        break;
        case BKNIGHT: CHESS_PiecePattern_Knight(chess_tile, tile, player, check);
        break;
        case BISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case BBISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case ROOK: CHESS_PiecePattern_Rook(chess_tile, tile, player, check);
        break;
        case BROOK: CHESS_PiecePattern_Rook(chess_tile, tile, player, check);
        break;
        case QUEEN: CHESS_PiecePattern_Queen(tile, player, check);
        break;
        case BQUEEN: CHESS_PiecePattern_Queen(tile, player, check);
        break;
        default:
        break;
    }

    return tile;
}

int CHESS_PiecePattern_King(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    char alpha[] = "abcdefgh";
    CHESS_CORE_TILE_TAG tag;

    int col_pos = MIDDLE_ReturnColTile(tile) - 1;
    tag.row = MIDDLE_ReturnRowTile(tile) - 1;
    tag.col = alpha[col_pos];

    int result = -1;

    for (int n = 0; n < 9; n++) {

        if (n == 4) { tag.row += 1; continue; }
        if (n == 3 || n == 6) { col_pos += 1; tag.row -= 3; }

        if (col_pos < 0) { tag.row += 1; continue; }
        if (col_pos > 7) { tag.row += 1; continue; }
        if (tag.row < 1) { tag.row += 1; continue; }
        if (tag.row > 8) { tag.row += 1; continue; }

        tag.col = alpha[col_pos];
        result = MIDDLE_TagToTile(tag);

        if (check == CHECK) {
            if (glo_chess_core_tile[result].piece != NULL) {
                if (glo_chess_event_layer[result] == false) {
                    glo_chess_core_tile[tile].piece->range[result] = true;
                }
            }

            else if (glo_chess_core_tile[result].piece == NULL) {
                if (glo_chess_event_layer[result] == false) glo_chess_core_tile[tile].piece->range[result] = true;
            }
        }

        else if (check == ATTACK) {
            if (glo_chess_core_tile[result].piece != NULL) {
                if (glo_chess_core_tile[result].piece->player != player) {

                    if (glo_chess_event_layer[result] == false) {
                        glo_chess_dot[result].state = true;
                    }
                }
            }

            else if (glo_chess_core_tile[result].piece == NULL) {
                if (glo_chess_event_layer[result] == false) glo_chess_dot[result].state = true;
            }
        }

        tag.row += 1;
    }

    return 0;
}

int CHESS_PiecePattern_Pawn(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    if (check == ATTACK /*&& glo_chess_core_tile[tile].piece->lock != true*/) {

        CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
        int result = -1;

        if (tag.row == 8) return 0;

        if (tag.row == 2) {

            tag.row += 1;
            if (tag.row == 8) return 0;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) {
                glo_chess_dot[result].state = true;

                tag.row += 1;
                if (tag.row == 8) return 0;
                result = MIDDLE_TagToTile(tag);

                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
            }
        } else {

            tag.row += 1;
            if (tag.row == 8) return 0;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;

        }

    }

    CHESS_PiecePattern_PawnAttack(tile, player, check);

    return 0;
}

int CHESS_PiecePattern_BPawn(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    if (check == ATTACK /*&& glo_chess_core_tile[tile].piece->lock != true*/) {

        CHESS_CORE_TILE_TAG tag = glo_chess_core_tile[tile].tag;
        int result = -1;

        if (tag.row == 1) return 0;

        if (tag.row == 7) {

            tag.row -= 1;
            if (tag.row == 1) return 0;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) {
                glo_chess_dot[result].state = true;

                tag.row -= 1;
                if (tag.row == 1) return 0;
                result = MIDDLE_TagToTile(tag);

                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
            }
        } else {

            tag.row -= 1;
            if (tag.row == 1) return 0;
            result = MIDDLE_TagToTile(tag);

            if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;

        }

    }

    CHESS_PiecePattern_BPawnAttack(tile, player, check);

    return 0;
}

int CHESS_PiecePattern_Knight(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    // initialization
    char alpha[] = "abcdefgh";
    CHESS_CORE_TILE_TAG tag;

    int col_pos = MIDDLE_ReturnColTile(tile) - 2;
    tag.row = MIDDLE_ReturnRowTile(tile) - 2;
    tag.col = alpha[col_pos];

    int result = -1;

    // todo:
    // make range of piece stop if king is inside, while keeping glo_chess_event_layer range continue, or viceversa (probably better)
    // or
    // make complex use of loops to identify where attack is coming from, to glow dot on enemy direction of attack.


    // another solution
    // on event, on checking king if under attack, check (before accepting middle) ,
    // every piece in defense if can "stop" the attack.
    // make another check afterwards (always before accepting middle) ,
    // which detects if the global variable was untouched.
    // if was untouched, it means that the king NEEDS to move itself, else ,
    // continue the defense, from the first checking the first check which signals a possible move.

    for (int n = 0; n < 4; n++) {

        for (int i = 0; i < 4; i++) {

            if (n == 0) col_pos += 1;
            else if (n == 1) tag.row += 1;
            else if (n == 2) col_pos -= 1;
            else if (n == 3) tag.row -= 1;

            tag.col = alpha[col_pos];
            result = MIDDLE_TagToTile(tag);

            if (result == -1) continue;

            if (i == 0 || i == 2)
            {
                if (check == CHECK) {
                    chess_tile[tile].piece->range[result] = true;
                    continue;
                }

                if (check == CHECK_KING) {

                    printf("hello\n");

                    glo_chess_event_king_uatk = false;

                    // create copy of tile
                    CHESS_CORE_TILE unsafe_tile[64]; CHESS_CORE_PLAYER unsafe_player;
                    MIDDLE_UnsafePosition_Copy(unsafe_tile);

                    // apply changes to unsafe tile
                    MIDDLE_UpdatePositionPiece(unsafe_tile, tile, result);

                    // check if any opposite attack still attack king
                    for (int u = 0; u < 64; u++) {
                        if (unsafe_tile[u].piece != NULL) {
                            if (unsafe_tile[u].piece->player != player) {

                                if (player == WHITE_PLAYER) unsafe_player = BLACK_PLAYER;
                                else unsafe_player = WHITE_PLAYER;

                                CHESS_RedirectPiecePattern(unsafe_tile, u, unsafe_player, CHECK);
                            }
                        }
                    }

                    // now apply position if attack stops king_uatk
                    EVENT_CheckPieceLayer(unsafe_tile, player);

                    if (glo_chess_event_king_uatk == false) {
                        glo_chess_dot[result].state = true;
                    }

                    continue;
                }

                if (chess_tile[result].piece == NULL) {
                    glo_chess_dot[result].state = true;
                    continue;
                }

                if (chess_tile[result].piece->player != player) glo_chess_dot[result].state = true;

            }
        }
    }

    return 0;
}

int CHESS_PiecePattern_Bishop(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    CHESS_CORE_TILE_TAG tag;

    // save current state on col_pos
    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile);

    int result = -1;
    for (int n = 0; n < 4; n++)
    {
        col_pos = MIDDLE_ReturnColTile(tile);
        tag.row = MIDDLE_ReturnRowTile(tile);
        tag.col = alpha[col_pos];

        for (int i = 0; i < 8; i++)
        {
            if      (n == 0) { col_pos -= 1; tag.row += 1; }
            else if (n == 1) { col_pos += 1; tag.row += 1; }
            else if (n == 2) { col_pos -= 1; tag.row -= 1; }
            else if (n == 3) { col_pos += 1; tag.row -= 1; }
            tag.col = alpha[col_pos];

            result = MIDDLE_TagToTile(tag);

            if (result == -1) continue;

            // temporary fix to standardize locking piece through lock variable(?)
            //if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

            if (check == CHECK) {
                glo_chess_core_tile[tile].piece->range[result] = true;

                if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                    if (glo_chess_core_tile[result].piece->enum_piece != KING && glo_chess_core_tile[result].piece->enum_piece != BKING) break;
                }
            }

            else {
                if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                else if (glo_chess_core_tile[result].piece != NULL)
                {
                    if (glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                    break;
                }
            }
        }
    }

    return 0;
}

int CHESS_PiecePattern_Rook(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    CHESS_CORE_TILE_TAG tag;

    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile);

    int result = -1;
    for (int n = 0; n < 4; n++)
    {
        col_pos = MIDDLE_ReturnColTile(tile);
        tag.row = MIDDLE_ReturnRowTile(tile);
        tag.col = alpha[col_pos];

        for (int i = 0; i < 8; i++)
        {
            if      (n == 0) tag.row += 1;
            else if (n == 1) col_pos += 1;
            else if (n == 2) tag.row -= 1;
            else if (n == 3) col_pos -= 1;
            tag.col = alpha[col_pos];

            result = MIDDLE_TagToTile(tag);

            if (result == -1) continue;

            if (check == CHECK) {

                chess_tile[tile].piece->range[result] = true;

                if (chess_tile[result].piece != NULL && chess_tile[result].piece->player != player) {
                    if (chess_tile[result].piece->enum_piece != KING && chess_tile[result].piece->enum_piece != BKING) break;
                }

                continue;
            }

            if (chess_tile[result].piece == NULL) glo_chess_dot[result].state = true;
            else if (chess_tile[result].piece != NULL)
            {
                if (chess_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                break;
            }
        }
    }

    return 0;
}

int CHESS_PiecePattern_Queen(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    CHESS_CORE_TILE_TAG tag;

    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile);

    int result = -1;
    for (int n = 0; n < 2; n++)
    {
        for (int i = 0; i < 4; i++)
        {
            col_pos = MIDDLE_ReturnColTile(tile);
            tag.row = MIDDLE_ReturnRowTile(tile);
            tag.col = alpha[col_pos];

            for (int u = 0; u < 8; u++)
            {
                if (n == 0)
                {
                    if      (i == 0) tag.row += 1;
                    else if (i == 1) col_pos += 1;
                    else if (i == 2) tag.row -= 1;
                    else if (i == 3) col_pos -= 1;
                }

                else if (n == 1)
                {
                    if      (i == 0) { col_pos -= 1; tag.row += 1; }
                    else if (i == 1) { col_pos += 1; tag.row += 1; }
                    else if (i == 2) { col_pos -= 1; tag.row -= 1; }
                    else if (i == 3) { col_pos += 1; tag.row -= 1; }
                }

                tag.col = alpha[col_pos];
                result = MIDDLE_TagToTile(tag);

                if (result == -1) continue;

                // temporary fix to standardize locking piece through lock variable(?)
                //if (check == false && glo_chess_core_tile[tile].piece->lock == true) break;

                if (check == CHECK) {
                    glo_chess_core_tile[tile].piece->range[result] = true;

                    if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                        if (glo_chess_core_tile[result].piece->enum_piece != KING && glo_chess_core_tile[result].piece->enum_piece != BKING) break;
                    }
                }

                else {
                    if (glo_chess_core_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                    else if (glo_chess_core_tile[result].piece != NULL)
                    {
                        if (glo_chess_core_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                        break;
                    }
                }
            }
        }
    }

    return 0;
}

int CHESS_PiecePattern_PawnAttack(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    char alpha[] = "abcdefgh";
    int row = MIDDLE_ReturnRowTile(tile) + 1;
    int col_pos = MIDDLE_ReturnColTile(tile) - 1;

    // if piece is already at (ex.) e8, exit
    if (row > 7) return 0;

    CHESS_CORE_TILE_TAG tag;
    tag.row = row;
    tag.col = alpha[col_pos];

    int result = -1;
    for (int n = 0; n < 3; n++) {

        if (col_pos < 0) { col_pos += 1; tag.col = alpha[col_pos]; continue; }
        else if (col_pos > 7) { break; }

        if (n == 1) { col_pos += 1; tag.col = alpha[col_pos]; continue; }

        result = MIDDLE_TagToTile(tag);

        if (check == CHECK)
        {
            glo_chess_core_tile[tile].piece->range[result] = true;
        }

        else if (check == ATTACK /* && glo_chess_core_tile[tile].piece->lock != true */)
        {
            if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                glo_chess_dot[result].state = true;
            }
        }

        col_pos += 1; tag.col = alpha[col_pos];
    }

    return 0;
}

int CHESS_PiecePattern_BPawnAttack(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    char alpha[] = "abcdefgh";
    int row = MIDDLE_ReturnRowTile(tile) - 1;
    int col_pos = MIDDLE_ReturnColTile(tile) - 1;

    // if piece is already at (ex.) e1, exit
    if (row < 0) return 0;

    CHESS_CORE_TILE_TAG tag;
    tag.row = row;
    tag.col = alpha[col_pos];

    int result = -1;
    for (int n = 0; n < 3; n++) {

        if (col_pos < 0) { col_pos += 1; tag.col = alpha[col_pos]; continue; }
        else if (col_pos > 7) { break; }

        if (n == 1) { col_pos += 1; tag.col = alpha[col_pos]; continue; }

        result = MIDDLE_TagToTile(tag);

        if (check == CHECK)
        {
            glo_chess_core_tile[tile].piece->range[result] = true;
        }

        else if (check == ATTACK /* && glo_chess_core_tile[tile].piece->lock != true */)
        {
            if (glo_chess_core_tile[result].piece != NULL && glo_chess_core_tile[result].piece->player != player) {
                glo_chess_dot[result].state = true;
            }
        }

        col_pos += 1; tag.col = alpha[col_pos];
    }

    return 0;
}

#include <stdbool.h>

#include "event.h"
#include "chess.h"
#include "middle.h"
#include "core.h"
#include "dot.h"

int CHESS_PiecePattern_UpdateState(CHESS_CORE_TILE *core_tile, CHESS_CORE_PLAYER player) {

    static CHESS_CORE_PLAYER pl_bak;

    static bool check_state_once;
    if (!check_state_once) {
        pl_bak = CORE_ReversePlayer_State(player);
        check_state_once = true;
    }

    if (pl_bak != player) {

        printf("CHESS_PiecePattern_UpdateState:\n  updating state pieces...\n");
        glo_chess_event_king_uatk = false;

        // create copy of tile
        CHESS_CORE_TILE unsafe_tile[64];
        MIDDLE_UnsafePosition_Copy(core_tile, unsafe_tile);

        for (int n = 0; n < 64; n++) {
            if (core_tile[n].piece != NULL) {
                CHESS_PiecePattern_RangeReset(core_tile, n);
                if (core_tile[n].piece->enum_piece != KING && core_tile[n].piece->enum_piece != BKING)
                    CHESS_RedirectPiecePattern(core_tile, n, player, CHECK);
            }
        }

        // pieces
        for (int n = 0; n < 64; n++)
        {
            if (unsafe_tile[n].piece != NULL && unsafe_tile[n].piece->player == player)
            {
                // range
                for (int i = 0; i < 64; i++)
                {
                    if (unsafe_tile[n].piece->range[i] == true)
                    {
                        MIDDLE_Unsafe_UpdatePositionPiece(unsafe_tile, n, i);

                        for (int x = 0; x < 64; x++) {
                            if (unsafe_tile[x].piece != NULL && unsafe_tile[x].piece->player != player) {

                                CHESS_PiecePattern_RangeReset(unsafe_tile, x);

                                CHESS_RedirectPiecePattern(unsafe_tile, x, pl_bak, CHECK);

                                for (int u = 0; u < 64; u++)
                                    if (unsafe_tile[x].piece->range[u] == true)
                                        glo_chess_event_layer[u] = true;
                            }
                        }

                        EVENT_CheckKingState(unsafe_tile, player);

                        if (glo_chess_event_king_uatk == true)
                            core_tile[n].piece->range[i] = false;

                        // reset
                        EVENT_BlankLayer_Global();
                        MIDDLE_UnsafePosition_Copy(core_tile, unsafe_tile);
                    }
                }
            }
        }

        for (int n = 0; n < 64; n++)
            if (core_tile[n].piece != NULL && core_tile[n].piece->player != player)
                if (core_tile[n].piece->enum_piece != KING && core_tile[n].piece->enum_piece != BKING)
                {
                    CHESS_PiecePattern_RangeReset(core_tile, n);
                    CHESS_RedirectPiecePattern(core_tile, n, pl_bak, CHECK);

                    for (int i = 0; i < 64; i++)
                        if (core_tile[n].piece->range[i] == true)
                            glo_chess_event_layer[i] = true;
                }

        for (int n = 0; n < 64; n++)
            if (core_tile[n].piece != NULL && core_tile[n].piece->player == player)
                if (core_tile[n].piece->enum_piece == KING || core_tile[n].piece->enum_piece == BKING)
                    CHESS_RedirectPiecePattern(core_tile, n, player, CHECK);

        pl_bak = player;
        MIDDLE_UnsafePosition_Copy(NULL, unsafe_tile);
    }

    return 0;
}

int CHESS_PiecePattern_RangeReset(CHESS_CORE_TILE *core_tile, int tile) {
    for (int n = 0; n < 64; n++)
        core_tile[tile].piece->range[n] = false;
    return 0;
}

int CHESS_PiecePattern_RangeAllowed(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    for (int n = 0; n < 64; n++)
        if (core_tile[tile].piece->range[n] == true) {
            if (core_tile[n].piece != NULL && core_tile[n].piece->player == player)
                continue;

            glo_chess_dot[n].state = true;
        }

    return 0;
}

int CHESS_RedirectPiecePattern(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    switch(glo_chess_core_tile[tile].piece->enum_piece) {
        case KING: CHESS_PiecePattern_King(core_tile, tile, player);
        break;
        case BKING: CHESS_PiecePattern_King(core_tile, tile, player);
        break;
        case PAWN: CHESS_PiecePattern_Pawn(core_tile, tile, player);
        break;
        case BPAWN: CHESS_PiecePattern_BPawn(core_tile, tile, player);
        break;
        case KNIGHT: CHESS_PiecePattern_Knight(core_tile, tile, player);
        break;
        case BKNIGHT: CHESS_PiecePattern_Knight(core_tile, tile, player);
        break;
        case BISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case BBISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case ROOK: CHESS_PiecePattern_Rook(core_tile, tile, player);
        break;
        case BROOK: CHESS_PiecePattern_Rook(core_tile, tile, player);
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

int CHESS_PiecePattern_King(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    (void)player;

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

        if (core_tile[result].piece == NULL) {
            if (glo_chess_event_layer[result] == false) core_tile[tile].piece->range[result] = true;
        }

        else if (core_tile[result].piece->player != player) {
            if (glo_chess_event_layer[result] == false) {
                core_tile[tile].piece->range[result] = true;
            }
        }

        tag.row += 1;
    }

    return 0;
}

int CHESS_PiecePattern_Pawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = core_tile[tile].tag;
    int result = -1;

    for (int n = 0; n < 2; n++) {

        tag.row += 1;
        if (tag.row > 7) break;
        result = MIDDLE_TagToTile(tag);

        if (core_tile[result].piece == NULL)
            core_tile[tile].piece->range[result] = true;

        if (core_tile[tile].tag.row != 2) break;
    }

    CHESS_PiecePattern_PawnAttack(core_tile, tile, player);

    return 0;
}

int CHESS_PiecePattern_BPawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = core_tile[tile].tag;
    int result = -1;

    for (int n = 0; n < 2; n++) {

        tag.row -= 1;
        if (tag.row < 2) break;
        result = MIDDLE_TagToTile(tag);

        if (core_tile[result].piece == NULL)
            core_tile[tile].piece->range[result] = true;

        if (core_tile[tile].tag.row != 7) break;
    }

    CHESS_PiecePattern_PawnAttack(core_tile, tile, player);

    return 0;
}

int CHESS_PiecePattern_Knight(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player) {

    char alpha[] = "abcdefgh";
    CHESS_CORE_TILE_TAG tag;

    int col_pos = MIDDLE_ReturnColTile(tile) - 2;
    tag.row = MIDDLE_ReturnRowTile(tile) - 2;
    tag.col = alpha[col_pos];

    int result = -1;

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
                if (chess_tile[result].piece == NULL)
                    chess_tile[tile].piece->range[result] = true;

                else if (chess_tile[result].piece != NULL && chess_tile[result].piece->player != player) {
                    chess_tile[tile].piece->range[result] = true;
                }
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

int CHESS_PiecePattern_Rook(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player) {

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
            if (result == tile) continue;

            if (chess_tile[result].piece == NULL) {
                chess_tile[tile].piece->range[result] = true;
            }

            else if (chess_tile[result].piece->player != player) {

                chess_tile[tile].piece->range[result] = true;
                if (chess_tile[result].piece->enum_piece == KING || chess_tile[tile].piece->enum_piece == BKING)
                    continue;

                break;
            }

            else if (chess_tile[result].piece->player == player) {
                chess_tile[tile].piece->range[result] = true;
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

int CHESS_PiecePattern_PawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = core_tile[tile - 9].tag;

    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile - 9);

    int result = -1;
    for (int n = 0; n < 3; n++) {

        result = MIDDLE_TagToTile(tag);

        col_pos += 1;
        tag.col = alpha[col_pos];

        if (result == -1) continue;
        if (n == 1) continue;

        if (core_tile[result].piece != NULL && core_tile[result].piece->player != player) {
            core_tile[tile].piece->range[result] = true;
        }
    }

    return 0;
}

int CHESS_PiecePattern_BPawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = core_tile[tile + 9].tag;

    char alpha[] = "abcdefgh";
    int col_pos = MIDDLE_ReturnColTile(tile + 9);

    int result = -1;
    for (int n = 0; n < 3; n++) {

        result = MIDDLE_TagToTile(tag);

        col_pos += 1;
        tag.col = alpha[col_pos];

        if (result == -1) continue;
        if (n == 1) continue;

        if (core_tile[result].piece != NULL && core_tile[result].piece->player != player) {
            core_tile[tile].piece->range[result] = true;
        }
    }

    return 0;
}

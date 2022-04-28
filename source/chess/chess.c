#include <stdbool.h>

#include "event.h"
#include "chess.h"
#include "middle.h"
#include "core.h"
#include "dot.h"

int CHESS_PiecePattern_UpdateState(CHESS_CORE_TILE *core_tile, CHESS_CORE_PLAYER player) {

    static CHESS_CORE_PLAYER pl_bak;

    // only needed first itineraction, every time it will change player, it will sum up by itselfs
    static bool check_state_once;
    if (!check_state_once) {
        pl_bak = CORE_ReversePlayer_State(player);
        check_state_once = true;
    }

    // crea un singolo loop da 64 per creare i pattern di ogni singolo pezzo sulla scacchiera
    // controllare gli stati dei pezzi bianchi e neri e confrontarli per attacchi
    // per permette ai pezzi di abilitare glo_chess_dot, controllano semplicemente in un altro loop da 64
    // che il loro raggio sia attivo (chiamando successivamente con ATTACK)


    // resettare i raggi di ogni singolo pezzo
    // ps: si potrebbe anche semplicemente aggiornare il pattern, senza resettarlo,
    // ma richiederebbe del tempo che non ho (prima voglio finire il gioco lol)
    // le ottimizzazioni si fanno a fine game ;)

    if (pl_bak != player) {

        glo_chess_event_king_uatk = false;

        for (int n = 0; n < 64; n++) {

            // seperated because first loop needs to unlock any piece before locking again
            /*if (core_tile[n].piece != NULL && core_tile[n].piece->player == player) {
                core_tile[n].piece->lock = false;
            }*/

            if (core_tile[n].piece != NULL) {

                for (int i = 0; i < 64; i++) core_tile[n].piece->range[i] = false;

                /*
                if (core_tile[n].piece->enum_piece != KING && core_tile[n].piece->enum_piece != BKING) {
                    // creare i pattern di ogni singolo pezzo
                    CHESS_RedirectPiecePattern(core_tile, n, player, CHECK);
                }
                */
                CHESS_RedirectPiecePattern(core_tile, n, player, CHECK);

            }
        }

        // leaving this here (but requires modification)
        // because i need to check if king of player is under attack

        // if king is indeed under attack go inside here the knight copypaste

        /* copy pasted from Knight pattern;
        // im going to use this for intercepting the attack,
        // with each piece from player.

        after
        */

        // create copy of tile
        CHESS_CORE_TILE unsafe_tile[64];
        MIDDLE_UnsafePosition_Copy(unsafe_tile);

        //MIDDLE_UpdatePositionPiece(unsafe_tile, tile, result);

        // pieces
        for (int n = 0; n < 64; n++)
        {
            if (unsafe_tile[n].piece != NULL && unsafe_tile[n].piece->player == player && unsafe_tile[n].piece->enum_piece != KING && unsafe_tile[n].piece->enum_piece != BKING)
            {
                // range
                for (int i = 0; i < 64; i++)
                {
                    if (unsafe_tile[n].piece->range[i] == true)
                    {
                        MIDDLE_UpdatePositionPiece(unsafe_tile, n, i);

                        EVENT_BlankLayer_Global();

                        for (int x = 0; x < 64; x++) {
                            if (unsafe_tile[x].piece != NULL && unsafe_tile[x].piece->player != player) {

                                for (int u = 0; u < 64; u++) if (unsafe_tile[x].piece->range[u] == true)
                                    unsafe_tile[x].piece->range[u] = false;

                                CHESS_RedirectPiecePattern(unsafe_tile, x, pl_bak, CHECK);

                                for (int u = 0; u < 64; u++)
                                    if (unsafe_tile[x].piece->range[u] == true) glo_chess_event_layer[u] = true;
                            }


                            EVENT_CheckKingState(unsafe_tile, player);

                            if (glo_chess_event_king_uatk == true) {

                                printf("coretile: %d !! range: %d\n", n, i);
                                core_tile[n].piece->range[i] = false;
                            }

                            // reset
                            //MIDDLE_UpdatePositionPiece(unsafe_tile, i, n);
                            MIDDLE_UnsafePosition_Copy(unsafe_tile);
                        }
                    }
                }
            }
        }

        // king safe or not, create pattern for both kings (USING GLO_CHESS_EVENT_LAYER!!!)
        // thats why check before calling the kings at the top if need to move (before copypaste)
        for (int n = 0; n < 64; n++) {
            if (core_tile[n].piece != NULL && core_tile[n].piece->player == player && (core_tile[n].piece->enum_piece == KING || core_tile[n].piece->enum_piece == BKING)) {
                for (int i = 0; i < 64; i++) core_tile[n].piece->range[i] = false;

                CHESS_RedirectPiecePattern(core_tile, n, player, CHECK);
            }

        }

        pl_bak = player;

    }

    return 0;
}

int CHESS_PiecePattern_RangeAllowed(CHESS_CORE_TILE *core_tile, int tile) {

    for (int n = 0; n < 64; n++)
        if (core_tile[tile].piece->range[n] == true) glo_chess_dot[n].state = true;

    return 0;
}

int CHESS_RedirectPiecePattern(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    switch(glo_chess_core_tile[tile].piece->enum_piece) {
        case KING: CHESS_PiecePattern_King(core_tile, tile, player, check);
        break;
        case BKING: CHESS_PiecePattern_King(core_tile, tile, player, check);
        break;
        case PAWN: CHESS_PiecePattern_Pawn(tile, player, check);
        break;
        case BPAWN: CHESS_PiecePattern_BPawn(tile, player, check);
        break;
        case KNIGHT: CHESS_PiecePattern_Knight(core_tile, tile, player, check);
        break;
        case BKNIGHT: CHESS_PiecePattern_Knight(core_tile, tile, player, check);
        break;
        case BISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case BBISHOP: CHESS_PiecePattern_Bishop(tile, player, check);
        break;
        case ROOK: CHESS_PiecePattern_Rook(core_tile, tile, player, check);
        break;
        case BROOK: CHESS_PiecePattern_Rook(core_tile, tile, player, check);
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

int CHESS_PiecePattern_King(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

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

        if (check == CHECK) {

            if (core_tile[result].piece == NULL) {
                if (glo_chess_event_layer[result] == false) core_tile[tile].piece->range[result] = true;
            }

            else if (core_tile[result].piece->player != player) {
                if (glo_chess_event_layer[result] == false) {
                    core_tile[tile].piece->range[result] = true;
                }
            }
        }

        tag.row += 1;
    }

    return 0;
}

int CHESS_PiecePattern_Pawn(int tile, CHESS_CORE_PLAYER player, CHESS_PIECE_ATK check) {

    if (check == ATTACK) {

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

    if (check == ATTACK) {

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
                //if (chess_tile[tile].piece->lock == true) continue;

                /*
                if (check == ATTACK) {
                    printf("check ATTACK\n");
                    if (chess_tile[result].piece == NULL) {
                        glo_chess_dot[result].state = true;
                        continue;
                    }

                    if (chess_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                }
                */

                if (check == CHECK) {

                    if (chess_tile[result].piece == NULL)
                        chess_tile[tile].piece->range[result] = true;

                    if (chess_tile[result].piece != NULL && chess_tile[result].piece->player != player) {
                        if (chess_tile[result].piece->enum_piece == KING || chess_tile[result].piece->enum_piece == BKING) {
                            //glo_chess_event_king_uatk = true;
                        }

                        chess_tile[tile].piece->range[result] = true;
                    }

                    continue;
                }

                /*
                else if (check == ATTACK || check == CHECK_KING) {
                    for (int x = 0; x < 64; x++) {
                        if (chess_tile[tile].range[x] == true) glo_chess_dot = true;
                    }
                }

                if (check == ATTACK || check == CHECK_KING) {

                    CHESS_CORE_PLAYER unsafe_player = CORE_ReversePlayer_State(player);
                    if (chess_tile[result].piece == NULL || chess_tile[result].piece->player != player) {

                        //glo_chess_event_king_uatk = true;

                        // create copy of tile
                        CHESS_CORE_TILE unsafe_tile[64];
                        MIDDLE_UnsafePosition_Copy(unsafe_tile);

                        // apply changes to unsafe tile
                        MIDDLE_UpdatePositionPiece(unsafe_tile, tile, result);

                        // check if any opposite attack still attack king
                        for (int u = 0; u < 64; u++) {
                            if (unsafe_tile[u].piece != NULL) {
                                if (unsafe_tile[u].piece->player == unsafe_player) {

                                    CHESS_RedirectPiecePattern(unsafe_tile, u, unsafe_player, CHECK);
                                }
                            }
                        }

                        // now apply position if attack stops king_uatk
                        //EVENT_CheckPieceLayer(unsafe_tile, player);
                        EVENT_BlankLayer_Global();
                        EVENT_BlankLayer_Piece(unsafe_tile);

                        for (int u = 0; u < 64; u++)
                        {
                            // piece range copy
                            if (unsafe_tile[u].piece != NULL && unsafe_tile[u].piece->player == unsafe_player) {

                                CHESS_RedirectPiecePattern(unsafe_tile, u, unsafe_player, CHECK);

                                for (int x = 0; x < 64; x++) {

                                    if (unsafe_tile[u].piece->range[x] == true) {
                                        glo_chess_event_layer[x] = true;
                                    }

                                    // better to create a visible layer
                                    //printf("EVENT_CheckPieceLayer: piece[%c%d] range[%c%d] = piece[%d] layer[%d]\n", chess_tile[n].tag.col, chess_tile[n].tag.row, chess_tile[i].tag.col, chess_tile[i].tag.row, chess_tile[n].piece->range[i], glo_chess_event_layer[i]);
                                }
                            }
                        }

                        EVENT_CheckKingState(unsafe_tile, player);

                        if (glo_chess_event_king_uatk == false) glo_chess_dot[result].state = true;
                    }

                    if (n == 3 && i == 2) {

                        printf("im in\n");
                        EVENT_BlankLayer_Global();
                        EVENT_BlankLayer_Piece(chess_tile);

                        for (int u = 0; u < 64; u++)
                        {
                            // piece range copy
                            if (chess_tile[u].piece != NULL && chess_tile[u].piece->player == unsafe_player) {

                                CHESS_RedirectPiecePattern(chess_tile, u, unsafe_player, CHECK);

                                for (int x = 0; x < 64; x++) {

                                    if (chess_tile[u].piece->range[x] == true) {
                                        glo_chess_event_layer[x] = true;
                                    }

                                    // better to create a visible layer
                                    //printf("EVENT_CheckPieceLayer: piece[%c%d] range[%c%d] = piece[%d] layer[%d]\n", chess_tile[n].tag.col, chess_tile[n].tag.row, chess_tile[i].tag.col, chess_tile[i].tag.row, chess_tile[n].piece->range[i], glo_chess_event_layer[i]);
                                }
                            }
                        }

                        EVENT_CheckKingState(chess_tile, player);
                    }
                }
                */
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

    // save previous piece encountered (useful if second piece encountered is king the lock this)
    CHESS_CORE_PIECE *prev_piece = NULL;

    // using this variable to allow the algorithm to sign his range.
    // i call this "ghosting" because if i want to lock a piece, i need to know if king is behind
    // i cant just break if any piece (not king) was encountered
    bool allow_range;

    int result = -1;
    for (int n = 0; n < 4; n++)
    {
        // reset to default settings
        allow_range = true;
        prev_piece = NULL;

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

            // testing approach v2
            if (check == CHECK) {

                if (chess_tile[result].piece == NULL) {
                    if (allow_range == true) chess_tile[tile].piece->range[result] = true;
                }

                /*
                else if (chess_tile[result].piece->player != player) {

                    // compare if king is hit after
                    if (allow_range == false) {

                        if (chess_tile[result].piece->enum_piece == KING || chess_tile[result].piece->enum_piece == BKING) {
                            //if (prev_piece != NULL) prev_piece->lock = true;
                            //glo_chess_event_king_uatk = true;
                        }

                        break;
                    }

                    // save previous piece
                    // just for sake of simplicity, i will leave it here even if is ghosting
                    prev_piece = chess_tile[result].piece;

                    if (allow_range == true) {
                        //if (chess_tile[result].piece->enum_piece == KING || chess_tile[result].piece->enum_piece == BKING) glo_chess_event_king_uatk = true;
                        chess_tile[tile].piece->range[result] = true;
                    }

                    if (chess_tile[result].piece->enum_piece != KING && chess_tile[result].piece->enum_piece != BKING) allow_range = false;

                }
                */
                else if (chess_tile[result].piece->player != player) {

                    chess_tile[tile].piece->range[result] = true;
                    if (chess_tile[result].piece->enum_piece == KING || chess_tile[tile].piece->enum_piece == BKING) continue;
                    break;
                }

                else if (chess_tile[result].piece->player == player) {
                    chess_tile[tile].piece->range[result] = true;
                    break;
                }
            }

                /*
                else if (chess_tile[result].piece != NULL && chess_tile[result].piece->player != player) {

                    // compare if king is hit after
                    if (allow_range == false) {

                        if (chess_tile[result].piece->enum_piece == KING || chess_tile[result].piece->enum_piece == BKING) {
                            if (prev_piece != NULL) prev_piece->lock = true;
                            glo_chess_event_king_uatk = true;
                        }

                        break;
                    }

                    // save previous piece
                    // just for sake of simplicity, i will leave it here even if is ghosting
                    prev_piece = chess_tile[result].piece;

                    if (allow_range == true) chess_tile[tile].piece->range[result] = true;
                    if (chess_tile[result].piece->enum_piece != KING && chess_tile[result].piece->enum_piece != BKING) allow_range = false;
                }

                else if (chess_tile[result].piece != NULL && chess_tile[result].piece->player == player) {
                    if (allow_range == true) chess_tile[tile].piece->range[result] = true;
                    break;
                }
            }
            */

            /*
            if (check == CHECK)
            {
                chess_tile[tile].piece->range[result] = true;

                if (chess_tile[result].piece != NULL && chess_tile[result].piece->player != player) {
                    if (chess_tile[result].piece->enum_piece != KING && chess_tile[result].piece->enum_piece != BKING) break;
                }

                continue;
            }

            if (check == ATTACK)
            {
                if (chess_tile[result].piece == NULL) glo_chess_dot[result].state = true;
                else if (chess_tile[result].piece != NULL)
                {
                    if (chess_tile[result].piece->player != player) glo_chess_dot[result].state = true;
                    break;
                }
            }
            */
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

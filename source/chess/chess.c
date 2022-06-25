#include <stdbool.h>

#include "../dashboard/gui.h"
#include "../dashboard/popup.h"
#include "../security/debug.h"
#include "../global.h"
#include "archive.h"
#include "event.h"
#include "chess.h"
#include "middle.h"
#include "core.h"
#include "dot.h"

int CHESS_PiecePattern_UpdateState(CHESS_CORE_TILE *core_tile, CHESS_CORE_PLAYER player) {

    int result = 0;
    static CHESS_CORE_PLAYER pl_bak;

    static bool check_state_once;
    if (!check_state_once) {
        pl_bak = CORE_ReversePlayer_State(player);
        check_state_once = true;
    }

    if (pl_bak != player) {

        DEBUG_PrintBox(1, "CHESS_PiecePattern_UpdateState:");
        DEBUG_PrintBox(1, "  updating state pieces... ");

        // create copy of tile
        CHESS_CORE_TILE unsafe_tile[64];
        MIDDLE_UnsafePosition_Copy(core_tile, unsafe_tile);
        EVENT_BlankLayer_Global();

        for (int n = 0; n < 64; n++) {
            if (core_tile[n].piece != NULL) {
                CHESS_PiecePattern_RangeReset(core_tile, n);
                if (core_tile[n].piece->enum_piece != KING && core_tile[n].piece->enum_piece != BKING)
                    CHESS_Redirect_PiecePattern(core_tile, n, player);
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
                        glo_chess_event_king_uatk = false;
                        MIDDLE_Unsafe_UpdatePositionPiece(unsafe_tile, n, i);

                        for (int x = 0; x < 64; x++) {
                            if (unsafe_tile[x].piece != NULL && unsafe_tile[x].piece->player != player) {

                                CHESS_PiecePattern_RangeReset(unsafe_tile, x);

                                CHESS_Redirect_PiecePattern(unsafe_tile, x, pl_bak);

                                for (int u = 0; u < 64; u++)
                                    if (unsafe_tile[x].piece->range[u] == true)
                                        glo_chess_event_layer[u] = true;
                            }
                        }

                        EVENT_CheckKing_UnderAttack(unsafe_tile, player);

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
                {
                    CHESS_PiecePattern_RangeReset(core_tile, n);

                    if (CHESS_Redirect_EnumPawn(core_tile, n) == 0) {
                        if (core_tile[n].piece->enum_piece == PAWN) CHESS_PiecePattern_PawnAttack(core_tile, n, player, 1);
                        if (core_tile[n].piece->enum_piece == BPAWN) CHESS_PiecePattern_BPawnAttack(core_tile, n, player, 1);
                    } else CHESS_Redirect_PiecePattern(core_tile, n, pl_bak);

                    for (int i = 0; i < 64; i++)
                        if (core_tile[n].piece->range[i] == true)
                            glo_chess_event_layer[i] = true;
                }

        for (int n = 0; n < 64; n++)
            if (core_tile[n].piece != NULL && core_tile[n].piece->player == player)
                if (CHESS_Redirect_EnumKing(core_tile, n) == 0)
                    CHESS_Redirect_PiecePattern(core_tile, n, player);

        ARCHIVE_Notation_RecordMove(core_tile, glo_chess_event_king_uatk, glo_chess_archive_tmp_ptr, glo_chess_archive_tmp_tile[0], glo_chess_archive_tmp_tile[1]);

        result = EVENT_HandleKingState(core_tile, player);

        pl_bak = player;
        DEBUG_PrintBox(1, "done");
    }

    return result;
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

int CHESS_Redirect_PiecePattern(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

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
        case BISHOP: CHESS_PiecePattern_Bishop(core_tile, tile, player);
        break;
        case BBISHOP: CHESS_PiecePattern_Bishop(core_tile, tile, player);
        break;
        case ROOK: CHESS_PiecePattern_Rook(core_tile, tile, player);
        break;
        case BROOK: CHESS_PiecePattern_Rook(core_tile, tile, player);
        break;
        case QUEEN: CHESS_PiecePattern_Queen(core_tile, tile, player);
        break;
        case BQUEEN: CHESS_PiecePattern_Queen(core_tile, tile, player);
        break;
        default:
        break;
    }

    return tile;
}

int CHESS_Redirect_EnumKing(CHESS_CORE_TILE *chess_tile, int slot) {
    return ((chess_tile[slot].piece->enum_piece == KING || chess_tile[slot].piece->enum_piece == BKING) ? 0 : -1);
}

int CHESS_Redirect_EnumPawn(CHESS_CORE_TILE *chess_tile, int slot) {
    return ((chess_tile[slot].piece->enum_piece == PAWN || chess_tile[slot].piece->enum_piece == BPAWN) ? 0 : -1);
}

int CHESS_Redirect_EnumBishop(CHESS_CORE_TILE *chess_tile, int slot) {
    return ((chess_tile[slot].piece->enum_piece == BISHOP || chess_tile[slot].piece->enum_piece == BBISHOP) ? 0 : -1);
}

int CHESS_Redirect_EnumKnight(CHESS_CORE_TILE *chess_tile, int slot) {
    return ((chess_tile[slot].piece->enum_piece == KNIGHT || chess_tile[slot].piece->enum_piece == BKNIGHT) ? 0 : -1);
}

int CHESS_Redirect_EnumRook(CHESS_CORE_TILE *chess_tile, int slot) {
    return ((chess_tile[slot].piece->enum_piece == ROOK || chess_tile[slot].piece->enum_piece == BROOK) ? 0 : -1);
}

int CHESS_Redirect_EnumQueen(CHESS_CORE_TILE *chess_tile, int slot) {
    return ((chess_tile[slot].piece->enum_piece == QUEEN || chess_tile[slot].piece->enum_piece == BQUEEN) ? 0 : -1);
}

int CHESS_PiecePattern_King(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag;

    tag.col = MIDDLE_ReturnColTile(tile) - 1;
    tag.row = MIDDLE_ReturnRowTile(tile) - 1;

    int result = -1;

    if (EVENT_CheckPlayerCastling(glo_chess_event_king_castle, player) == 1) {
        for (int n = 0; n < (int)strlen(glo_chess_event_king_castle); n++)
            if ((result = CHESS_CheckState_CastleAvailable(core_tile, glo_chess_event_king_castle, tile, n)) != -1)
                core_tile[tile].piece->range[result] = true;
    }

    for (int n = 0; n < 9; n++) {

        if (n == 4) {
            tag.row += 1;
            continue;
        }

        else if (n == 3 || n == 6) {
            tag.col += 1;
            tag.row = MIDDLE_ReturnRowTile(tile) - 1;
        }

        result = MIDDLE_TagToTile(tag);
        tag.row += 1;

        if (result == -1) continue;

        if (glo_chess_event_layer[result] == false)
            core_tile[tile].piece->range[result] = true;

    }

    return 0;
}

int CHESS_PiecePattern_Pawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = core_tile[tile].tag;
    int result = -1; int state = 0;

    for (int n = 0; n < 2; n++) {

        tag.row += 1;
        if (tag.row > 8) break;
        result = MIDDLE_TagToTile(tag);

        if (result == -1) break;
        if (core_tile[result].piece != NULL) break;

        core_tile[tile].piece->range[result] = true;

        if (core_tile[tile].tag.row != 2) break;

        state++;
    }

    CHESS_PiecePattern_PawnAttack(core_tile, tile, player, 0);

    return (state);
}

int CHESS_PiecePattern_BPawn(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = core_tile[tile].tag;
    int result = -1; int state = 0;

    for (int n = 0; n < 2; n++) {

        tag.row -= 1;
        if (tag.row < 1) break;
        result = MIDDLE_TagToTile(tag);

        if (result == -1) break;
        if (core_tile[result].piece != NULL) break;

        core_tile[tile].piece->range[result] = true;

        if (core_tile[tile].tag.row != 7) break;

        state++;
    }

    CHESS_PiecePattern_BPawnAttack(core_tile, tile, player, 0);

    return (state);
}

int CHESS_PiecePattern_Knight(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player) {
    (void)player;

    CHESS_CORE_TILE_TAG tag;

    tag.col = MIDDLE_ReturnColTile(tile) - 2;
    tag.row = MIDDLE_ReturnRowTile(tile) - 2;

    int result = -1;
    for (int n = 0; n < 4; n++) {

        for (int i = 0; i < 4; i++) {

            if (n == 0) tag.col += 1;
            else if (n == 1) tag.row += 1;
            else if (n == 2) tag.col -= 1;
            else if (n == 3) tag.row -= 1;

            result = MIDDLE_TagToTile(tag);
            if (result == -1) continue;

            if (i == 0 || i == 2)
              chess_tile[tile].piece->range[result] = true;
        }
    }

    return 0;
}

int CHESS_PiecePattern_Bishop(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = chess_tile[tile].tag;
    int result = -1;

    for (int n = 0; n < 4; n++) {

        tag = chess_tile[tile].tag;

        for (int i = 0; i < 8; i++) {

            if (n == 0) {
                tag.col -= 1; tag.row += 1;
            } else if (n == 1) {
                tag.col += 1; tag.row += 1;
            } else if (n == 2) {
                tag.col += 1; tag.row -= 1;
            } else if (n == 3) {
                tag.col -= 1; tag.row -= 1;
            }

            result = MIDDLE_TagToTile(tag);
            if (result == -1) break;

            chess_tile[tile].piece->range[result] = true;

            if (chess_tile[result].piece == NULL)
                continue;

            if ((chess_tile[result].piece->enum_piece == KING ||
                chess_tile[result].piece->enum_piece == BKING) &&
                chess_tile[result].piece->player != player)
                continue;

            break;
        }

    }

    return 0;
}

int CHESS_PiecePattern_Rook(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = chess_tile[tile].tag;
    int result = -1;

    for (int n = 0; n < 4; n++) {

        tag = chess_tile[tile].tag;

        for (int i = 0; i < 8; i++) {

            if (n == 0) tag.row += 1;
            else if (n == 1) tag.col += 1;
            else if (n == 2) tag.row -= 1;
            else if (n == 3) tag.col -= 1;

            result = MIDDLE_TagToTile(tag);
            if (result == -1) break;

            chess_tile[tile].piece->range[result] = true;

            if (chess_tile[result].piece == NULL)
                continue;

            if ((chess_tile[result].piece->enum_piece == KING ||
                chess_tile[result].piece->enum_piece == BKING) &&
                chess_tile[result].piece->player != player)
                continue;

            break;
        }
    }

    return 0;
}

int CHESS_PiecePattern_Queen(CHESS_CORE_TILE *chess_tile, int tile, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = chess_tile[tile].tag;
    int result = -1;

    for (int n = 0; n < 2; n++) {

        for (int i = 0; i < 4; i++) {

            tag = MIDDLE_TileToTag(tile);

            for (int u = 0; u < 8; u++) {

                if (n == 0)
                {
                    if (i == 0) {
                        tag.col -= 1; tag.row += 1;
                    } else if (i == 1) {
                        tag.col += 1; tag.row += 1;
                    } else if (i == 2) {
                        tag.col -= 1; tag.row -= 1;
                    } else if (i == 3) {
                        tag.col += 1; tag.row -= 1;
                    }

                } else if (n == 1)
                {
                    if (i == 0) tag.row += 1;
                    else if (i == 1) tag.col += 1;
                    else if (i == 2) tag.row -= 1;
                    else if (i == 3) tag.col -= 1;
                }

                result = MIDDLE_TagToTile(tag);
                if (result == -1) break;

                chess_tile[tile].piece->range[result] = true;

                if (chess_tile[result].piece == NULL)
                    continue;

                if (CHESS_Redirect_EnumKing(chess_tile, result) == 0 &&
                    chess_tile[result].piece->player != player)
                    continue;

                break;
            }
        }
    }

    return 0;
}

int CHESS_CheckState_CastleAvailable(CHESS_CORE_TILE *chess_tile, char *fen_castle, int tile, int index) {

    CHESS_CORE_TILE_TAG king_castle;
    if (fen_castle[index] == 'K' || fen_castle[index] == 'k') {

        king_castle = MIDDLE_TileToTag(tile);
        int res = -1; int u;

        for (u = 0; u < 3; u++) {
            res = MIDDLE_TagToTile(king_castle); king_castle.col += 1;
            if (glo_chess_event_layer[res] == true || (u != 0 && chess_tile[res].piece != NULL)) break;
        }

        if (u == 3) return (res);

    } else if (fen_castle[index] == 'Q' || fen_castle[index] == 'q') {

        king_castle = MIDDLE_TileToTag(tile);
        int res = -1; int u;

        for (u = 0; u < 4; u++) {
            res = MIDDLE_TagToTile(king_castle); king_castle.col -= 1;
            if ((u < 3 && glo_chess_event_layer[res] == true) || (u != 0 && chess_tile[res].piece != NULL)) break;
        }

        king_castle.col += 2; res = MIDDLE_TagToTile(king_castle);
        if (u == 4) return (res);
    }

    return (-1);
}

int CHESS_CheckState_KingCastling(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player) {
    if (strcmp(glo_chess_event_king_castle, "-") == 0) return -1;

    DEBUG_PrintBox(2, "before player[%d] %s", player, glo_chess_event_king_castle);

    int result = -1;
    for (int n = 0; n < (int)strlen(glo_chess_event_king_castle); n++)
        if ((result = CHESS_CheckState_CastleAvailable(chess_tile, glo_chess_event_king_castle, position_old, n)) == position_new) {

            //CHESS_CORE_TILE_TAG tag;
            if (chess_tile[result].tag.col == 'c') {
                result -= 2;
                MIDDLE_UpdatePositionPiece(chess_tile, result, result + 3);
            } else if (chess_tile[result].tag.col == 'g') {
                result += 1;
                MIDDLE_UpdatePositionPiece(chess_tile, result, result - 2);
            }
        }

    // update fen castle by trimming
    EVENT_UpdateFenCastling_King(glo_chess_event_king_castle, player);

    DEBUG_PrintBox(2, "after player[%d] %s", player, glo_chess_event_king_castle);

    return 0;
}

int CHESS_CheckState_RookCastling(int position_old, CHESS_CORE_PLAYER player) {
    if (strcmp(glo_chess_event_king_castle, "-") == 0) return -1;

    DEBUG_PrintBox(2, "before player[%d] %s", player, glo_chess_event_king_castle);

    EVENT_UpdateFenCastling_Rook(glo_chess_event_king_castle, position_old, player);

    DEBUG_PrintBox(2, "after player[%d] %s", player, glo_chess_event_king_castle);

    return 0;
}

int CHESS_CheckState_PawnPromotion(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player) {

    if (glo_chess_event_pawn_promotn != -1) {
        CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, glo_chess_event_pawn_promotn);
        return 0;
    }

    if (MIDDLE_ReturnRowTile(position_new) == 8 || MIDDLE_ReturnRowTile(position_new) == 1) {

        if (chess_tile[position_old].piece->enum_piece == PAWN) {

            PP4M_HOOK *list_hook = GUI_PopupWindow_Init(525, 525);

            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, BISHOP, "Fante", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 10, 250, 250);
            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, KNIGHT, "Cavallo", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 265, 10, 250, 250);
            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, ROOK, "Torre", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 265, 250, 250);
            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, QUEEN, "Regina", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 265, 265, 250, 250);

            int result = 0;
            SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);

            result = GUI_PopupWindow_Core(list_hook, txr_snapshot);

            if (result == BISHOP) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, BISHOP);
            } else if (result == KNIGHT) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, KNIGHT);
            } else if (result == ROOK) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, ROOK);
            } else if (result == QUEEN) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, QUEEN);
            }

            for (int n = pp4m_HOOK_Size(list_hook); n > 0; n--)
                pp4m_HOOK_Remove(list_hook);
        } else if (chess_tile[position_old].piece->enum_piece == BPAWN) {

            PP4M_HOOK *list_hook = GUI_PopupWindow_Init(525, 525);

            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, BBISHOP, "Fante", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 10, 250, 250);
            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, BKNIGHT, "Cavallo", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 265, 10, 250, 250);
            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, BROOK, "Torre", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 265, 250, 250);
            GUI_PopupWindow_Button(list_hook, OPENSANS_REGULAR, BQUEEN, "Regina", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 265, 265, 250, 250);

            int result = 0;
            SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);

            result = GUI_PopupWindow_Core(list_hook, txr_snapshot);

            if (result == BBISHOP) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, BBISHOP);
            } else if (result == BKNIGHT) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, BKNIGHT);
            } else if (result == BROOK) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, BROOK);
            } else if (result == BQUEEN) {
                CORE_UpdateState_PieceStruct(position_old, chess_tile[position_old].piece, player, BQUEEN);
            }

            for (int n = pp4m_HOOK_Size(list_hook); n > 0; n--)
                pp4m_HOOK_Remove(list_hook);
        }

        DEBUG_PrintBox(1, "CHESS_CheckState_PawnPromotion: Updated pawn[%d] -> %d", position_old, chess_tile[position_old].piece->enum_piece);
        glo_chess_event_pawn_promotn = chess_tile[position_old].piece->enum_piece;
    }

    return 0;
}

int CHESS_CheckState_PawnEnPassant(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player) {

    CHESS_CORE_TILE_TAG tag = chess_tile[position_old].tag;
    if (chess_tile[position_old].piece->enum_piece == PAWN) {

        if (CHESS_PiecePattern_Pawn(chess_tile, position_old, player) != 2) {

            if (glo_chess_event_tile_passant != -1) {

                if (glo_chess_event_tile_passant == position_new) {
                    CHESS_CORE_TILE_TAG tag_pl = chess_tile[position_new].tag;
                    tag_pl.row -= 1;

                    CORE_GlobalDestroyPiece(chess_tile[MIDDLE_TagToTile(tag_pl)].piece);
                }
            }

            glo_chess_event_tile_passant = -1;
            return (-1);
        }

        tag.row += 1;
        glo_chess_event_tile_passant = MIDDLE_TagToTile(tag);

    } else if (chess_tile[position_old].piece->enum_piece == BPAWN) {

        if (CHESS_PiecePattern_BPawn(chess_tile, position_old, player) != 2) {

            if (glo_chess_event_tile_passant != -1) {

                if (glo_chess_event_tile_passant == position_new) {
                    CHESS_CORE_TILE_TAG tag_pl = chess_tile[position_new].tag;
                    tag_pl.row -= 1;

                    CORE_GlobalDestroyPiece(chess_tile[MIDDLE_TagToTile(tag_pl)].piece);
                }
            }

            glo_chess_event_tile_passant = -1;
            return (-1);
        }

        tag.row -= 1;
        glo_chess_event_tile_passant = MIDDLE_TagToTile(tag);
    }

    return (0);
}

int CHESS_PiecePattern_PawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, int king_check) {

    CHESS_CORE_TILE_TAG tag;

    tag.col = MIDDLE_ReturnColTile(tile) - 1;
    tag.row = MIDDLE_ReturnRowTile(tile) + 1;

    int result = -1;
    for (int n = 0; n < 3; n++) {

        result = MIDDLE_TagToTile(tag);

        tag.col += 1;

        if (result == -1) continue;
        if (n == 1) continue;

        if (king_check == 1) {
            core_tile[tile].piece->range[result] = true;
            continue;
        }

        if (glo_chess_event_tile_passant == result)
            core_tile[tile].piece->range[result] = true;

        else if (core_tile[result].piece != NULL && core_tile[result].piece->player != player)
            core_tile[tile].piece->range[result] = true;
    }

    return 0;
}

int CHESS_PiecePattern_BPawnAttack(CHESS_CORE_TILE *core_tile, int tile, CHESS_CORE_PLAYER player, int king_check) {

    CHESS_CORE_TILE_TAG tag;

    tag.col = MIDDLE_ReturnColTile(tile) - 1;
    tag.row = MIDDLE_ReturnRowTile(tile) - 1;

    int result = -1;
    for (int n = 0; n < 3; n++) {

        result = MIDDLE_TagToTile(tag);

        tag.col += 1;

        if (result == -1) continue;
        if (n == 1) continue;

        if (king_check == 1) {
            core_tile[tile].piece->range[result] = true;
            continue;
        }

        if (glo_chess_event_tile_passant == result)
            core_tile[tile].piece->range[result] = true;

        else if (core_tile[result].piece != NULL && core_tile[result].piece->player != player)
            core_tile[tile].piece->range[result] = true;
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "chess.h"
#include "archive.h"

char *glo_chess_archive_record = NULL;
char *glo_chess_archive_flow_mark = "KBNRQ";
int glo_chess_archive_tmp_tile[2];
void *glo_chess_archive_tmp_piece = NULL;

void ARCHIVE_Notation_RecordMove(CHESS_CORE_TILE *chess_tile, bool king_uatk, void *piece, int slot_old, int slot_new) {

    if (!glo_chess_archive_record)
        glo_chess_archive_record = malloc(sizeof(char) * 1);

    char notation[16];
    ARCHIVE_NOTATION_PIECE piece_ntt;
    char buffer[3] = ""; int ind = 0;
    CHESS_CORE_TILE_TAG tag = chess_tile[slot_new].tag;

    piece_ntt = ARCHIVE_Redirect_StateMove(chess_tile, slot_old);

    if (piece_ntt != NONE_A && piece_ntt != PAWN_A) {
        buffer[ind++] = glo_chess_archive_flow_mark[piece_ntt];
        if (piece != NULL) buffer[ind++] = 'x';
    }

    else if (piece_ntt == PAWN_A && piece != NULL) {
        buffer[ind++] = chess_tile[slot_old].tag.col;
        buffer[ind++] = 'x';
    }

    sprintf(notation, " %s%c%d", buffer, tag.col, tag.row);

    if (king_uatk == true)
        strcat(notation, "+");

    printf("move: [%s]\n", notation);

    glo_chess_archive_record = realloc(glo_chess_archive_record, sizeof(glo_chess_archive_record) + strlen(notation));
    strncat(glo_chess_archive_record, notation, strlen(notation));

    printf("archive: [%s]\n", glo_chess_archive_record);

    return;
}


ARCHIVE_NOTATION_PIECE ARCHIVE_Redirect_StateMove(CHESS_CORE_TILE *chess_tile, int slot) {

    if (chess_tile[slot].piece == NULL) return NONE_A;
    if (CHESS_Redirect_EnumKing(chess_tile, slot) == 0) return KING_A;
    else if (CHESS_Redirect_EnumBishop(chess_tile, slot) == 0) return BISHOP_A;
    else if (CHESS_Redirect_EnumKnight(chess_tile, slot) == 0) return KNIGHT_A;
    else if (CHESS_Redirect_EnumRook(chess_tile, slot) == 0) return ROOK_A;
    else if (CHESS_Redirect_EnumQueen(chess_tile, slot) == 0) return QUEEN_A;
    else if (CHESS_Redirect_EnumPawn(chess_tile, slot) == 0) return PAWN_A;
}

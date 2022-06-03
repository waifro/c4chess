#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "chess.h"
#include "archive.h"

char *glo_chess_archive_record;
int glo_chess_archive_tmp_tile[2];
void *glo_chess_archive_tmp_ptr = NULL;

void ARCHIVE_Notation_RecordMove(CHESS_CORE_TILE *chess_tile, bool king_uatk, void *ptr, int slot_old, int slot_new) {

    /*
    if (!slot_old || !slot_new) return;

    char *piece_idnt = "KBNRQ";

    char buf[6] = {0};
    char tag[3] = {0};

    int piece_ntt = ARCHIVE_Redirect_StateMove(chess_tile, slot_old);


    if (piece_ntt != NONE_A && piece_ntt != PAWN_A) {
        buf[0] = piece_idnt[piece_ntt];
        if (ptr != NULL) buf[1] = 'x';
    } else if (piece_ntt == PAWN_A) {
        if (ptr != NULL) {
            buf[0] = chess_tile[slot_old].tag.col;
            buf[1] = 'x';
        } else buf[0] = '\0';
    }

    sprintf(tag, "%c%d", chess_tile[slot_new].tag.col, chess_tile[slot_new].tag.row);

    printf("\nbuf [%s]\n", buf);
    printf("buf [%s]\n", tag);

    strcat(buf, tag);

    if (king_uatk == true) strcat(buf, "+ ");
    else strcat(buf, " ");

    printf("buf [%s]\n", buf);

    char notation[16];
    ARCHIVE_NOTATION_PIECE piece_ntt;
    char buffer[3] = ""; int ind = 0;
    CHESS_CORE_TILE_TAG tag = chess_tile[slot_new].tag;

    piece_ntt = ARCHIVE_Redirect_StateMove(chess_tile, slot_old);

    if (piece_ntt != NONE_A && piece_ntt != PAWN_A) {
        buffer[ind++] = piece_idnt[piece_ntt];
        if (piece != NULL) buffer[ind] = 'x';
    }

    else if (piece_ntt == PAWN_A && piece != NULL) {
        buffer[ind++] = chess_tile[slot_old].tag.col;
        buffer[ind] = 'x';
    }

    sprintf(notation, "%s%c%d", buffer, tag.col, tag.row);

    if (king_uatk == true) strcat(notation, "+ ");
    else strcat(notation, " ");

    printf("buf [%s]\n", notation);

    glo_chess_archive_record = realloc(glo_chess_archive_record, sizeof(glo_chess_archive_record) + strlen(buf));
    strncat(glo_chess_archive_record, buf, strlen(buf));

    printf("archive: [%s]\n", glo_chess_archive_record);
    */
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

    return -1;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core.h"
#include "chess.h"
#include "archive.h"

char *glo_chess_record_move;
int glo_chess_archive_tmp_tile[2];
void *glo_chess_archive_tmp_ptr;

void ARCHIVE_Notation_RecordMove(CHESS_CORE_TILE *chess_tile, bool king_uatk, void *ptr, int tile_old, int tile_new) {

    if (!tile_old) return;

    char *notation_piece_char = "KBNRQ";
    char buf[6]; char foo[3];
    char tag[3]; sprintf(tag, "%c%d", chess_tile[tile_new].tag.col, chess_tile[tile_new].tag.row);

    ARCHIVE_NOTATION_PIECE notation_piece = ARCHIVE_Redirect_StateMove(chess_tile, tile_new);

    switch(notation_piece) {
        case PAWN_A:
            if (ptr == NULL) break;
            foo[0] = chess_tile[tile_old].tag.col;
            foo[1] = 'x';
            break;

        case NONE_A:
            break;

        default:
            foo[0] = notation_piece_char[notation_piece];
            if (ptr != NULL) foo[1] = 'x';
            else foo[1] = '\0';
            break;
    }

    if (strlen(foo) > 0) strcpy(buf, foo);
    else buf[0] = '\0';

    strcat(buf, tag);

    if (king_uatk == true) strcat(buf, "+");
    else { /*buf[strlen(buf)] = ' ';*/ buf[strlen(buf)] = '\0'; }

    if (!glo_chess_record_move) glo_chess_record_move = malloc(strlen(buf) + 1);
    strcpy(glo_chess_record_move, buf);

    printf("[%s]\n", glo_chess_record_move);

    return;
}

ARCHIVE_NOTATION_PIECE ARCHIVE_Redirect_StateMove(CHESS_CORE_TILE *chess_tile, int slot) {

    if (chess_tile[slot].piece == NULL) return -1;

    if (CHESS_Redirect_EnumKing(chess_tile, slot) == 0) return KING_A;
    else if (CHESS_Redirect_EnumBishop(chess_tile, slot) == 0) return BISHOP_A;
    else if (CHESS_Redirect_EnumKnight(chess_tile, slot) == 0) return KNIGHT_A;
    else if (CHESS_Redirect_EnumRook(chess_tile, slot) == 0) return ROOK_A;
    else if (CHESS_Redirect_EnumQueen(chess_tile, slot) == 0) return QUEEN_A;
    else if (CHESS_Redirect_EnumPawn(chess_tile, slot) == 0) return PAWN_A;
    else return NONE_A;
}

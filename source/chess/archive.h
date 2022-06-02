#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "core.h"

extern char *glo_chess_archive_record;
extern int glo_chess_archive_tmp_tile[2];
extern void *glo_chess_archive_tmp_ptr;

typedef enum {
    KING_A,
    BISHOP_A,
    KNIGHT_A,
    ROOK_A,
    QUEEN_A,
    PAWN_A,
    NONE_A
} ARCHIVE_NOTATION_PIECE;

void ARCHIVE_Notation_RecordMove(CHESS_CORE_TILE *chess_tile, bool king_uatk, void *ptr, int slot_old, int slot_new);
ARCHIVE_NOTATION_PIECE ARCHIVE_Redirect_StateMove(CHESS_CORE_TILE *chess_tile, int slot);

#endif // ARCHIVE_H

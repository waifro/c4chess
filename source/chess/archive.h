#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "core.h"

typedef enum {
    KING_A,
    BISHOP_A,
    KNIGHT_A,
    ROOK_A,
    QUEEN_A,
    PAWN_A,
    NONE_A
} ARCHIVE_NOTATION_PIECE;

extern char *glo_chess_archive_flow;
extern char *glo_chess_archive_flow_mark;

void ARCHIVE_Notation_RecordMove(CHESS_CORE_TILE *chess_tile, bool king_uatk, int position_old, int position_new);
ARCHIVE_NOTATION_PIECE ARCHIVE_Redirect_StateMove(CHESS_CORE_TILE *chess_tile, int slot);

#endif // ARCHIVE_H

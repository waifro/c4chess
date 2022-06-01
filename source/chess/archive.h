#ifndef ARCHIVE_H
#define ARCHIVE_H

typedef enum {
    NONE_A,
    KING_A,
    PAWN_A,
    BISHOP_A,
    KNIGHT_A,
    ROOK_A,
    QUEEN_A

} ARCHIVE_NOTATION_PIECE;

extern char *glo_chess_archive_flow;

void ARCHIVE_Notation_EnrollMove(CHESS_CORE_TILE *chess_tile, int position_old, int position_new);
ARCHIVE_NOTATION_PIECE ARCHIVE_Redirect_StateMove(CHESS_CORE_TILE *chess_tile, int slot);

#endif // ARCHIVE_H

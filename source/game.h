#ifndef _GAME_H
#define _GAME_H

typedef enum {

    DPAWN,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING

} GAME_IDENTIFIER;

typedef struct {

    SDL_Texture *texture;
    SDL_Rect rect;
    int alive;
    GAME_IDENTIFIER identifier;

    // deprecated
    int colomn;
    char row;

} GAME_PIECE;

void GAME_InitializeChessboard(void);
void GAME_UpdatePositionPiece(GAME_PIECE *piece, GAME_IDENTIFIER identifier, int colomn, char row);
void GAME_InitializePieces(void);
void GAME_CreateChessboard_Tiles(void);

#endif

#ifndef _GAME_H
#define _GAME_H

typedef enum {

    DPAWN = 1,
    PAWN = 2,
    KNIGHT = 3,
    BISHOP = 4,
    ROOK = 5,
    QUEEN = 6,
    KING = 7

} GAME_IDENTIFIER;

typedef enum {

  WHITE = 1,
  BLACK = 2

} GAME_PLAYER;

typedef struct {

    SDL_Texture *texture;
    SDL_Rect rect;
    GAME_IDENTIFIER identifier;
    GAME_PLAYER player;

} GAME_PIECE;

extern GAME_PLAYER global_player;

void GAME_InitializeChessboard(void);
void GAME_UpdatePositionPiece(int pos, GAME_PIECE *piece, GAME_PLAYER player, GAME_IDENTIFIER identifier, int colomn, char row);
void GAME_InitializePieces(void);
void GAME_CreateChessboard_Tiles(void);

#endif

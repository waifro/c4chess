#ifndef _GAME_H
#define _GAME_H

typedef struct {

    SDL_Texture *texture;
    SDL_Rect pp4m;
    int x;
    int y;
    int colomn;
    char row;

} GAME_PIECE;

void GAME_InitializeChessboard(void);
void GAME_UpdatePositionPiece(GAME_PIECE *piece, int colomn, char row);
void GAME_InitializePieces(void);
void GAME_CreateChessboard_Tiles(void);
void GAME_CreateChessboard_RightTiles(void);
void GAME_CreateChessboard_LeftTiles(void);

#endif

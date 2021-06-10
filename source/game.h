#ifndef _GAME_H
#define _GAME_H

typedef struct {

    int x;
    int y;
    int colomn;
    char row;
    char f_object[128];

} GAME_PIECE;

void GAME_InitializeChessboard(void);
void GAME_InitializePieces(void);
void GAME_CreateChessboard_Tiles(void);
void GAME_CreateChessboard_RightTiles(void);
void GAME_CreateChessboard_LeftTiles(void);

#endif

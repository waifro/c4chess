#ifndef CORE_H
#define CORE_H

/* libraries */
#include <SDL2/SDL.h>

/* structs & enums */
typedef enum {
    WHITE_PLAYER,
    BLACK_PLAYER
} CHESS_CORE_PLAYER;

typedef enum {
    KING,
    PAWN,
    BPAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QWEEN
} CHESS_CORE_ENUM_PIECE;

typedef struct {
    CHESS_CORE_PLAYER player;
    int enum_piece;
    SDL_Texture *texture;
    SDL_Rect rect;
} CHESS_CORE_PIECE;

typedef struct {
    CHESS_CORE_PIECE *piece;
    char tag[2];
    SDL_Texture *texture;
    SDL_Rect rect;
} CHESS_CORE_TILE;

extern CHESS_CORE_TILE glo_chess_core_tile[64];

/* prototypes */
void CORE_InitPiecePlayer(CHESS_CORE_PLAYER player);
void CORE_InitPiece(CHESS_CORE_PLAYER player);
void CORE_ChessCreateBoard(void);
void CORE_ChessInitTag(CHESS_CORE_PLAYER player);
void CORE_ChessInitPlacePiece(void);

void CORE_Testing(CHESS_CORE_PLAYER player);

#endif

#ifndef CORE_H
#define CORE_H

/* libraries */
#include <stdbool.h>
#include <SDL2/SDL.h>

/* structs & enums */
typedef enum {
    WHITE_PLAYER = 0,
    BLACK_PLAYER = 1
} CHESS_CORE_PLAYER;

typedef struct {
    char col;
    int row;
}   CHESS_CORE_TILE_TAG;

typedef enum {
    KING,
    PAWN,
    BPAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN
} CHESS_CORE_ENUM_PIECE;

typedef struct {
    CHESS_CORE_PLAYER player; // which piece is of player
    int enum_piece; // piece identificitaion
    SDL_Texture *texture; // texture piece
    SDL_Rect rect; // rect pos of texture
    bool lock; // is used to lock a piece if encounters an attack directly behind the king
    bool range[64]; // layer of the range attack each piece have
} CHESS_CORE_PIECE;

typedef struct {
    CHESS_CORE_PIECE *piece;
    CHESS_CORE_TILE_TAG tag;
    SDL_Texture *texture;
    SDL_Rect rect;
} CHESS_CORE_TILE;

extern CHESS_CORE_PLAYER glo_chess_core_player;
extern CHESS_CORE_TILE glo_chess_core_tile[64];
extern CHESS_CORE_PIECE glo_chess_core_piece[32];

/* prototypes */
void CORE_InitPiecePlayer(CHESS_CORE_PLAYER player);
void CORE_DestroyPiece(CHESS_CORE_PIECE *piece);
void CORE_InitPiece(CHESS_CORE_PLAYER player);
void CORE_ChessCreateBoard(void);
void CORE_ChessInitTag(CHESS_CORE_PLAYER player);
void CORE_ChessInitPlacePiece(void);

void CORE_GlobalDestroyPiece(CHESS_CORE_PIECE *piece);
void CORE_GlobalClearCorePiece(void);
void CORE_GlobalClearChessTile(void);

void CORE_Testing(CHESS_CORE_PLAYER player);

#endif

#ifndef CORE_H
#define CORE_H

/* libraries */
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../network/net.h"

/* structs & enums */
typedef enum {
    WHITE_PLAYER = 0,
    BLACK_PLAYER = 1
} CHESS_CORE_PLAYER;

typedef struct {
    char col;
    int row;
} CHESS_CORE_TILE_TAG;

typedef enum {
    NONE = 0,
    KING,
    BKING,
    PAWN,
    BPAWN,
    KNIGHT,
    BKNIGHT,
    BISHOP,
    BBISHOP,
    ROOK,
    BROOK,
    QUEEN,
    BQUEEN
} CHESS_CORE_ENUM_PIECE;

typedef struct {
    CHESS_CORE_PLAYER player;   // which piece is of player
    int enum_piece;             // piece identificitaion
    SDL_Texture *texture;       // texture piece
    SDL_Rect rect;              // rect pos of texture
    bool range[64];             // layer of the range attack each piece have
} CHESS_CORE_PIECE;

typedef struct {
    CHESS_CORE_PIECE *piece;
    CHESS_CORE_TILE_TAG tag;
    SDL_Texture *texture;
    SDL_Rect rect;
} CHESS_CORE_TILE;

// TODO: save space from global variables and structs
extern CHESS_CORE_PLAYER glo_chess_core_player;
extern CHESS_CORE_TILE glo_chess_core_tile[64];
extern CHESS_CORE_PIECE glo_chess_core_piece[32];

extern bool glo_chess_event_king_uatk;
extern CHESS_CORE_TILE_TAG glo_chess_core_passant;

/* prototypes */
void CORE_Chessboard_Init(void);
void CORE_ChessTag_Init(CHESS_CORE_TILE *chess_tile);

void CORE_GlobalDestroyPiece(CHESS_CORE_PIECE *piece);
void CORE_GlobalClearCorePiece(void);
void CORE_GlobalClearChessTile(void);

void CORE_UpdateState_PieceStruct(int tile, CHESS_CORE_PIECE *piece, CHESS_CORE_PLAYER player, int enum_piece);

void CORE_ResetGlobal_CorePiece(void);
void CORE_ResetGlobal_CorePiece(void);

void CORE_Chessboard_Reverse(CHESS_CORE_TILE *core_tile);
CHESS_CORE_PLAYER CORE_ReversePlayer_State(CHESS_CORE_PLAYER player);

void CORE_GlobalUpdate_StateRender(void);

char *CORE_NET_ChessboardInit(int *socket, CHESS_CORE_PLAYER *player, char *buffer);

int CORE_NET_SendRoomState(int *socket, int *running, int *restrict tile_old, int *restrict tile_new) ;
int CORE_NET_RecvRoomState(int *socket, CHESS_CORE_PLAYER *player_turn, int *tile_old, int *tile_new);

int CORE_NET_SocketRedirect(int *socket, CHESS_CORE_PLAYER *player);

void CORE_InitChess_Play(CHESS_CORE_PLAYER player_view, char *fen_init, int *socket);

#endif

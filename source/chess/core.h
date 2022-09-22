#ifndef CORE_H
#define CORE_H

/* libraries */
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../c4network/net.h"

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

/* prototypes */

/* on success, creates an array of CHESS_CORE_TILE in the middle */
void CORE_Chessboard_Init(void);

/* on success, initializes every tile with tags */
void CORE_ChessTag_Init(CHESS_CORE_TILE *chess_tile);

/* on success, destroyes a selected piece from memory */
void CORE_GlobalDestroyPiece(CHESS_CORE_PIECE **piece);

/* on success, transforms a piece into another piece */
void CORE_UpdateState_PieceStruct(int tile, CHESS_CORE_PIECE *piece, CHESS_CORE_PLAYER player, int enum_piece);

/* on success, resets every tile */
void CORE_ResetGlobal_CoreTile(void);

/* on success, resets every piece */
void CORE_ResetGlobal_CorePiece(void);

/* on success, inverts every tile */
void CORE_Chessboard_Reverse(CHESS_CORE_TILE *core_tile);

/* on success, returns inverted player */
CHESS_CORE_PLAYER CORE_ReversePlayer_State(CHESS_CORE_PLAYER player);

/* on success, updates rendering chessboard, debugging and stuff */
void CORE_GlobalUpdate_StateRender(void);

char *CORE_NET_ChessboardInit(CHESS_CORE_PLAYER *player, char *buffer);

int CORE_NET_UpdateLobby(int *code, int *socket, char **buf_arr, int *position_old, int *position_new, int *promotn);

int CORE_NET_SocketRedirect(int *socket, CHESS_CORE_PLAYER *player);

/* on success, it follows the sequence of the requested behavior */
int CORE_NET_CommandSequence_REQ_ASSIGN_LOBBY(int *socket, int *buf_cmd);

/* on success, redirects to requested behavior */
int CORE_NET_CommandSequence(int *socket, CLIENT_CMD *master_cmd, int *buf_cmd);

/*
    on success, returns a *SV_STATE* command
     0 on nothing
    -1 on error
*/
int CORE_NET_AwaitCommand_SV_STATE(int *socket);

void CORE_RenderUpdate(SDL_Texture *background, int frames_per_sec, int *timer);

void CORE_InitChess_Play(CHESS_CORE_PLAYER player_view, char *fen_init, int *socket);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m_image.h"
#include "../security/debug.h"

#include "../global.h"
#include "fen.h"
#include "chess.h"
#include "middle.h"
#include "core.h"

#define TEX_WKING "resources/wking.png"
#define TEX_WPAWN "resources/wpawn.png"
#define TEX_WKNIGHT "resources/wknight.png"
#define TEX_WBISHOP "resources/wbishop.png"
#define TEX_WROOK "resources/wrook.png"
#define TEX_WQUEEN "resources/wqueen.png"

#define TEX_BKING "resources/bking.png"
#define TEX_BPAWN "resources/bpawn.png"
#define TEX_BKNIGHT "resources/bknight.png"
#define TEX_BBISHOP "resources/bbishop.png"
#define TEX_BROOK "resources/brook.png"
#define TEX_BQUEEN "resources/bqueen.png"

// TODO: init chess pieces using Forsyth-Edwards Notation
// > https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation

// legend for piece FEN:
// uppercase is White player
// lowercase is Black player

/*

Forsyth-Edwards Notation (FEN): "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

board:      [rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R]
player:     [b]
castling:   [KQkq]
passant:    [-]
halfmoves:  [3]
fullmoves:  [2]

( ͡° ͜ʖ ͡°)

*/

int FEN_Init(CHESS_CORE_PLAYER *init_player, char *fen_notation) {

    char fen_board[128];    // chess board pieces
    char fen_play[4];       // player turn [w] or [b]
    char fen_castle[4];     // [-] none | [QKq]castling available for king both ends, black king only queen
    char fen_passant[4];    // [-] none | ex. [f6] is signed for en passant
    int fen_halfmove;       // [+1] if no capture of pieces or pawn advance, else resets (draw on 100 moves)
    int fen_fullmove;       // [+1] a complete cycle by both players

    DEBUG_PrintBox(1, "FEN_Init:");
    DEBUG_PrintBox(1, "  notation: ");

    FEN_StrTrunk(fen_notation, fen_board, fen_play, fen_castle, fen_passant, &fen_halfmove, &fen_fullmove);

    FEN_PlayerTurn((int*)init_player, fen_play[0]);
    FEN_InitBoard(*init_player, fen_board);
    _glo_chess_tile_passant = FEN_StrTrunk_TagToTile(fen_passant);
    strncpy(_glo_chess_king_castling, fen_castle, strlen(fen_castle) -1);

    DEBUG_PrintBox(1, "[%s] [%s] [%s] [%d] [%d] [%d]", fen_board, fen_play, _glo_chess_king_castling, _glo_chess_tile_passant, fen_halfmove, fen_fullmove);

    return (0);
}

int FEN_StrTrunk_TagToTile(char *str) {

    if (str[0] == '-') return -1;
    CHESS_CORE_TILE_TAG tag;
    tag.col = str[0]; tag.row = atoi(&str[1]);

    return (MIDDLE_TagToTile(tag));
}

void FEN_StrTrunk(char *restrict str, char *restrict a, char *restrict b, char *restrict c, char *restrict d, int *restrict e, int *restrict f) {

    int i = 0; int ind = 0;
    for (int n = 0; n < (int)strlen(str); n++) {

        if (str[n] == ' ') {

            if (i == 0) a[++ind] = '\0';
            if (i == 1) b[++ind] = '\0';
            if (i == 2) c[++ind] = '\0';
            if (i == 3) d[++ind] = '\0';

            ind = 0;
            i++;

            continue;
        }

        if (i == 0) strncpy(&a[ind], &str[n], 1);
        if (i == 1) strncpy(&b[ind], &str[n], 1);
        if (i == 2) strncpy(&c[ind], &str[n], 1);
        if (i == 3) strncpy(&d[ind], &str[n], 1);
        if (i == 4) *e = atoi(&str[n]);
        if (i == 5) *f = atoi(&str[n]);

        ind++;
    }

    return;
}

int FEN_PlayerTurn(int *init_player, char fen_play) {

    if (fen_play == 'w') *init_player = WHITE_PLAYER;
    else if (fen_play == 'b') *init_player = BLACK_PLAYER;

    return (0);
}

int FEN_InitBoard(CHESS_CORE_PLAYER init_player, char *fen_board) {

    // switch from one player to another
    CHESS_CORE_PLAYER player = init_player;

    // grab length of ptr (the length could varie)
    short int fen_length = strlen(fen_board);

    char character = 0;

    // index keeps track of pieces initialized
    short int tile = 0;
    short int index = 0;

    DEBUG_PrintBox(2, "  length FEN board: %d", fen_length);

    for (int n = 0; n <= fen_length; n++) {
        if (tile >= 64) break;

        character = fen_board[n];
        DEBUG_PrintBox(2, "  character[%c] - tile[%d]", character, tile);

        if (isalpha(character) != 0)
        {
            // when '/' is used to skip ranks
            if (character == '/' || character == '\\') continue;

            if (isupper(character) != 0) player = WHITE_PLAYER;
            else player = BLACK_PLAYER;

            // init piece from special case
            FEN_InitPiece(player, &glo_chess_core_piece[index], FEN_EnumPiece(character), tile);

            index += 1;
            tile += 1;
        }

        else if (isdigit(character) != 0)
        {
            short int foo = 0;
            // increase number of tiles from character

            char bar[5];
            sprintf(bar, "%c", character);

            sscanf(bar, "%hd", &foo);
            tile += foo;
        }

    }

    return (0);
}

int FEN_InitPiece(CHESS_CORE_PLAYER player, CHESS_CORE_PIECE *piece, CHESS_CORE_ENUM_PIECE name, int tile) {

    DEBUG_PrintBox(2, "FEN_InitPiece:");

    switch(name)
    {
        case KING:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case BKING:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case PAWN:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case BPAWN:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case KNIGHT:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case BKNIGHT:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case BISHOP:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case BBISHOP:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case ROOK:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case BROOK:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case QUEEN:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WQUEEN, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;
        case BQUEEN:
            piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BQUEEN, &piece->rect, glo_chess_core_tile[tile].rect.x, glo_chess_core_tile[tile].rect.y, 50, 50);
        break;

        default:
            DEBUG_PrintBox(2, " error NONE case");
            exit(1);
        break;
    }

    if (piece->texture == NULL)
    {
        DEBUG_PrintBox(2, "  piece->texture not initialized");
        return (EXIT_FAILURE);
    }

    DEBUG_PrintBox(2, "  piece->texture = %p", &piece->texture);

    glo_chess_core_tile[tile].piece = piece;
    glo_chess_core_tile[tile].piece->player = player;
    glo_chess_core_tile[tile].piece->enum_piece = name;

    return EXIT_SUCCESS;
}

CHESS_CORE_ENUM_PIECE FEN_EnumPiece(char character) {

    switch(character)
    {
        case 'K':
        return KING;
        break;

        case 'k':
        return BKING;
        break;

        case 'P':
        return PAWN;
        break;

        case 'p':
        return BPAWN;
        break;

        case 'N':
        return KNIGHT;
        break;

        case 'n':
        return BKNIGHT;
        break;

        case 'B':
        return BISHOP;
        break;

        case 'b':
        return BBISHOP;
        break;

        case 'R':
        return ROOK;
        break;

        case 'r':
        return BROOK;
        break;

        case 'Q':
        return QUEEN;
        break;

        case 'q':
        return BQUEEN;
        break;

        default:
        return NONE;
        break;
    }
}

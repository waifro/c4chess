#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m_image.h"

#include "../global.h"
#include "fen.h"
#include "chess.h"
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
moveWhite:  [1]
moveBlack:  [2]

( ͡° ͜ʖ ͡°)

*/

int FEN_Init(CHESS_CORE_PLAYER player, char *fen_board) {

    // switch from one player to another
    CHESS_CORE_PLAYER init_player = WHITE_PLAYER;
    if (player == WHITE_PLAYER) init_player = BLACK_PLAYER;

    // grab length of ptr (the length could varie)
    short int fen_length = strlen(fen_board);

    char character = 0;

    //volatile short int result = -1;
    volatile short int tile = 0;

    printf("len: %d\n", fen_length);

    for (int n = 0; n <= fen_length; n++) {

        character = fen_board[n];

        if (isalpha(character) != 0)
        {
            // when '/' is used to skip ranks
            if (character == '/' || character == '\\') continue;

            // white pieces
            if (isupper(character) != 0)
            {
                printf("FEN_Init: char white = %c\n", character);
                //CORE_CaseEnumPiece()
                //CORE_FenNotation_InitPiece(init_player, &piece);
            }

            // black pieces
            else if (islower(character) != 0)
            {
                printf("FEN_Init: char black = %c\n", character);
                //CORE_FenNotation_InitPiece();
            }
        }

        else if (isdigit(character) != 0)
        {
            int foo = -1;
            // increase number of tiles from character
            sscanf(&character, "%d", &foo);

            printf("FEN_Init: foo = %d\n", foo);

            tile += foo;
            continue;
        }

        // if (result != EXIT_SUCCESS) return (EXIT_FAILURE);
        tile += 1;
    }

    return 0;
}

int FEN_InitPieceCase(CHESS_CORE_PLAYER player, CHESS_CORE_PIECE *piece, CHESS_CORE_ENUM_PIECE name, int tile) {

    /*
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
    */

    // promemoria
    // invertire i movimenti quando sei nero
    // processo per inizializzare i pezzi:

    // 1 - inizializza scacchiera con i pezzi pronti
    // 2 - invertire scacchiera se sei nero (equivale a reverse list)
    // 3 - teoricamente non devo invertire i movimenti se sono nero perche
    //      praticamente se inverto la scacchiera, i pezzi sono gia allocati e pronti per la pos. dei tile

    if (player == WHITE_PLAYER)
    {
        switch(name)
        {
            case KING:
                piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &piece->rect, 0, 0, 50, 50);
            break;
            case PAWN:
                piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &piece->rect, 0, 0, 50, 50);
            break;
            case BPAWN:
                piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &piece->rect, 0, 0, 50, 50);
            break;
            default:
                printf("error\n");
                exit(1);
            break;
        }
    }

    else if (player == BLACK_PLAYER)
    {
        switch(name)
        {
            case KING:
                piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &piece->rect, 0, 0, 50, 50);
            break;
            case PAWN:
                piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &piece->rect, 0, 0, 50, 50);
            break;
            case BPAWN:
                piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &piece->rect, 0, 0, 50, 50);
            break;
            default:
                printf("error\n");
                exit(1);
            break;
        }
    }

    if (piece->texture == NULL)
    {
        printf("CORE_InitPiece:\n  piece->texture not initialized\n");
        return (EXIT_FAILURE);
    }

    printf("CORE_InitPiece:\n  piece->texture = %p\n", &piece->texture);

    glo_chess_core_tile[tile].piece = piece;
    glo_chess_core_tile[tile].piece->player = player;
    glo_chess_core_tile[tile].piece->enum_piece = name;


    return EXIT_SUCCESS;
}


/*
int CORE_InitPiece(CHESS_CORE_PIECE *piece, int tile, CHESS_CORE_ENUM_PIECE name, CHESS_CORE_PLAYER player) {

    printf("CORE_InitPiece:\n  player = %p\n", &player);

    if (glo_chess_core_player == WHITE_PLAYER) {

        if (player == WHITE_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN:
                break;
                case NONE:
                break;
            }
        }

        else if (player == BLACK_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN:
                break;
                case NONE:
                break;
            }
        }

    }

    else if (glo_chess_core_player == BLACK_PLAYER) {

        if (player == WHITE_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKING, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WPAWN, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_WQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN:
                break;
                case NONE:
                break;
            }
        }

        else if (player == BLACK_PLAYER) {
            switch (name) {
                case KING: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKING, &piece->rect, 0, 0, 50, 50);
                break;
                case PAWN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BPAWN, &piece->rect, 0, 0, 50, 50);
                break;
                case KNIGHT: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BKNIGHT, &piece->rect, 0, 0, 50, 50);
                break;
                case BISHOP: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BBISHOP, &piece->rect, 0, 0, 50, 50);
                break;
                case ROOK: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BROOK, &piece->rect, 0, 0, 50, 50);
                break;
                case QUEEN: piece->texture = pp4m_IMG_ImageToTexture(glo_render, NULL, TEX_BQUEEN, &piece->rect, 0, 0, 50, 50);
                break;
                case BPAWN:
                break;
                case NONE:
                break;
            }
        }

    }

    if (piece->texture == NULL)
    {
        printf("CORE_InitPiece:\n  piece->texture not initialized\n");
        return (EXIT_FAILURE);
    }

    printf("CORE_InitPiece:\n  piece->texture = %p\n", &piece->texture);

    glo_chess_core_tile[tile].piece = piece;
    glo_chess_core_tile[tile].piece->player = player;
    glo_chess_core_tile[tile].piece->enum_piece = name;

    return (EXIT_SUCCESS);
}
*/

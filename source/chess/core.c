#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_net.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "../network/net.h"
#include "../dashboard/gui.h"
#include "../dashboard/gui_alias.h"
#include "../global.h"
#include "event.h"
#include "chess.h"
#include "dot.h"
#include "archive.h"
#include "middle.h"
#include "core.h"
#include "fen.h"

CHESS_CORE_PLAYER glo_chess_core_player;
CHESS_CORE_TILE glo_chess_core_tile[64];
CHESS_CORE_PIECE glo_chess_core_piece[32];
char glo_chess_core_castle[4];

void CORE_Chessboard_Init(void) {

    int size_tile = 50;
    int x_b = glo_screen_w / 2 - (size_tile * 4);
    int y_b = glo_screen_h / 2 - (size_tile * 4);

    int colomn = 1;

    bool toggle = false;

    for(int n = 0; n < 64; n++) {

        if (colomn > 8) {
            y_b += size_tile;
            x_b = glo_screen_w / 2 - (size_tile * 4);
            toggle ^= 1;
            colomn = 1;
        }

        if (toggle == false) {
            toggle = true;
            glo_chess_core_tile[n].texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, &glo_chess_core_tile[n].rect, x_b, y_b, size_tile, size_tile);
        }

        else if (toggle == true) {
            toggle = false;
            glo_chess_core_tile[n].texture = pp4m_DRAW_TextureInitColor(glo_render, PP4M_GREY_NORMAL, &glo_chess_core_tile[n].rect, x_b, y_b, size_tile, size_tile);
        }

        x_b += size_tile; colomn += 1;
    }

    return;
}

void CORE_ChessTag_Init(CHESS_CORE_TILE *chess_tile) {

    char alpha[] = "abcdefgh";
    int col_pos = 0;
    int row = 8;

    for (int n = 0; n < 64; n++) {

        chess_tile[n].tag.col = alpha[col_pos];
        chess_tile[n].tag.row = row;

        col_pos++;
        if (col_pos > 7) {
            col_pos = 0;
            if (row != 1) row--;
        }
    }

    return;
}

void CORE_GlobalDestroyPiece(CHESS_CORE_PIECE *piece) {

    if (piece != NULL) {
        printf("CORE_GlobalDestroyPiece:\n  destroy piece = %p\n", piece);
        SDL_DestroyTexture(piece->texture);
        piece->enum_piece = NONE;
    }

    return;
}

void CORE_GlobalClearCorePiece(void) {
    for (int n = 0; n < 32; n++) if (sizeof(glo_chess_core_piece[n]) != 0) CORE_GlobalDestroyPiece(&glo_chess_core_piece[n]);
    return;
}

void CORE_GlobalClearChessTile(void) {

    for (int n = 0; n < 64; n++) {
        if (sizeof(glo_chess_core_tile[n].piece) != sizeof(CHESS_CORE_PIECE*)) CORE_GlobalDestroyPiece(glo_chess_core_tile[n].piece);
        SDL_DestroyTexture(glo_chess_core_tile[n].texture);
    }

    return;
}

void CORE_ResetGlobal_CorePiece(void) {

    for (int n = 0; n < 32; n++)
    {
        glo_chess_core_piece[n].player = -1;
        glo_chess_core_piece[n].enum_piece = NONE;
        glo_chess_core_piece[n].texture = NULL;

        glo_chess_core_piece[n].rect.x = 0;
        glo_chess_core_piece[n].rect.y = 0;
        glo_chess_core_piece[n].rect.w = 0;
        glo_chess_core_piece[n].rect.h = 0;
    }

    return;
}

CHESS_CORE_PLAYER CORE_ReversePlayer_State(CHESS_CORE_PLAYER player) {
    if (player == WHITE_PLAYER) return BLACK_PLAYER;
    else return WHITE_PLAYER;
}

void CORE_Chessboard_Reverse(CHESS_CORE_TILE *core_tile) {

    CHESS_CORE_TILE core_tile_bak[64];

    for (int n = 0; n < 64; n++) {
        core_tile_bak[n] = core_tile[63 - n];
    }

    for (int n = 0; n < 64; n++) {
        core_tile[n] = core_tile_bak[n];
    }

    return;
}

void CORE_GlobalUpdate_StateRender(void) {

    for (int n = 0; n < 64; n++) {
        SDL_RenderCopy(glo_render, glo_chess_core_tile[n].texture, NULL, &glo_chess_core_tile[n].rect);
        if (glo_chess_core_tile[n].piece != NULL) SDL_RenderCopy(glo_render, glo_chess_core_tile[n].piece->texture, NULL, &glo_chess_core_tile[n].piece->rect);
        DOT_StateGlobalDot(n);
    }

    return;
}

int CORE_NET_InitGlobal(net_sockrid_t *sockrid, CHESS_CORE_PLAYER *player, char *fen) {
    int result = -1;

    // client side
    char buf[512];
    char buf_plvl[4];

    // fen notes
    char buf_fen[128];
    char buf_play[4];
    char buf_castle[4];
    char buf_passant[4];

    int buf_halfm;
    int buf_fullm;

    result = recv(*sockrid->socket, buf, 511, MSG_WAITALL);
    if (result == -1) {
        printf("CORE_NET_InitGlobal: recv(): %s\n", strerror(errno));
        return -1;
    }

    printf("socket recieved: [%s]\n", buf);

    sscanf(buf, "%d %s %*s", &sockrid->roomId, buf_plvl);
    FEN_StrTrunk(&buf[4], buf_fen, buf_play, buf_castle, buf_passant, &buf_halfm, &buf_fullm);

    //sscanf(buf, "%d %c %s %s %s %*d %d %d", &buf_rid, &buf_plvl[0], buf_fen, buf_play, buf_castle, &buf_halfm, &buf_fullm);

    printf("recieved parsed: [%s] [%s] [%s] [%s] [%d] [%d]\n", buf_fen, buf_play, buf_castle, buf_passant, buf_halfm, buf_fullm);

    FEN_PlayerTurn((int*)player, buf_plvl[0]);
    sprintf(fen, "%s %s %s %s %d %d", buf_fen, buf_play, buf_castle, buf_passant, buf_halfm, buf_fullm);

    return (0);
}

int CORE_NET_CloseSocketState(net_sockrid_t *sockrid, int running) {

    if (running == -1)
        if (sockrid->socket != NULL) {
            close(*sockrid->socket);
            sockrid->socket = NULL;
            sockrid->roomId = 0;
        }

    return (0);
}

int CORE_NET_SendRoomState(net_sockrid_t *sockrid, int *running, CHESS_CORE_PLAYER *player_turn, int *tile_old, int *tile_new) {
    if (sockrid->socket == NULL) return -1;
    (void)player_turn;

    // temporary fix
    if (*running == -2) {
        char buf[256];
        sprintf(buf, "%d %d %d", sockrid->roomId, *tile_old, *tile_new);
        write(*sockrid->socket, buf, strlen(buf));
    }

    return 0;
}

int CORE_NET_RecvRoomState(net_sockrid_t *sockrid, int *running, CHESS_CORE_PLAYER *player_turn, int *tile_old, int *tile_new) {
    if (sockrid->socket == NULL) return -1;

    // temporary fix
    char buf[256];

    printf("waiting packet...\n");

    if (read(*sockrid->socket, buf, 255) == -1) {
        *running = -1;
        return 0;
    }

    printf("msg recv: %s\n", buf);
    sscanf(buf, "%d %d %d", &sockrid->roomId, tile_old, tile_new);

    ARCHIVE_UpdateRegister_PieceState(glo_chess_core_tile, *tile_old, *tile_new);
    EVENT_UpdateState_ChessEvent(glo_chess_core_tile, *tile_old, *tile_new, *player_turn);

    MIDDLE_UpdatePositionPiece(glo_chess_core_tile, *tile_old, *tile_new);

    *player_turn = CORE_ReversePlayer_State(*player_turn);

    return -2;
}

int CORE_NET_SocketRedirect(net_sockrid_t *sockrid, CHESS_CORE_PLAYER *player) {
    return (sockrid->socket != NULL && *player != glo_chess_core_player ? -1 : 0);
}

void CORE_InitChess_Play(CHESS_CORE_PLAYER player_view, char *fen_init, net_sockrid_t *sockrid) {

    /* preserve player */
    CHESS_CORE_PLAYER player;
    glo_chess_core_player = player_view;

    /* init chessboard gfx untagged */
    CORE_Chessboard_Init();

    /* setup chessboard tagged */
    CORE_ChessTag_Init(glo_chess_core_tile);

    /* reverse chessboard if needed */
    if (player_view == BLACK_PLAYER) CORE_Chessboard_Reverse(glo_chess_core_tile);

    /* init dot gfx */
    DOT_GlobalDot_Init();

    /* set glo_chess_core_piece to initial state */
    CORE_ResetGlobal_CorePiece();

    /* init pieces for main player */
    FEN_Init(&player, fen_init);

    SDL_Texture *background = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_GREY_DARK, 255, NULL, 0, 0, glo_screen_w, glo_screen_h);

    SDL_Event event;

    SDL_Texture *txr_snapshot = NULL;

    // testing: cap framerate to 30/60 fps
    float deltaTime; int running = 0;

    while(running == 0) {
        deltaTime = pp4m_DeltaFramerate();
        (void)deltaTime;

        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = -1;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {

                int result = 0;
                txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);

                while (1) {

                    if (running == -1) break;

                    PP4M_HOOK *hook_list_pw = GUI_PopupWindow_Init(440, 180);

                    GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, -1, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 15, 410, 70);
                    GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, -2, "Esci dal gioco", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 95, 410, 70);

                    result = GUI_PopupWindow_Core(hook_list_pw, txr_snapshot);

                    if (result == -1) break;
                    if (result == -3) { running = -1; break; }

                    SDL_Texture *txr_snapshot2 = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
                    PP4M_HOOK *hook_list_pw_exit = GUI_PopupWindow_Init(400, 165);

                    GUI_PopupWindow_Title(hook_list_pw_exit, OPENSANS_REGULAR, "Sei sicuro?", PP4M_WHITE, 32);
                    GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, -1, "Annulla", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 85, 185, 70);
                    GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, -2, "Okay", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 205, 85, 185, 70);

                    if (GUI_PopupWindow_Core(hook_list_pw_exit, txr_snapshot2) == -2) running = -1;

                    hook_list_pw_exit = NULL;

                }
            }
        }

        /* checks if king under attack */
        CHESS_PiecePattern_UpdateState(glo_chess_core_tile, player);

        /* makes the in-game changes during gameplay */
        MIDDLE_UpdateChangeState(&event, &player, sockrid);

        SDL_RenderClear(glo_render);
        SDL_RenderCopy(glo_render, background, NULL, NULL);

        /* renders everything chessboard releated */
        CORE_GlobalUpdate_StateRender();
        SDL_RenderPresent(glo_render);

        CORE_NET_CloseSocketState(sockrid, running);
    }

    SDL_DestroyTexture(background);

    return;
}

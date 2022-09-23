#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <sys/time.h> // fd_set

#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_net.h"
#include "../pp4m/pp4m_ttf.h"
#include "../pp4m/pp4m_draw.h"
#include "../pp4m/pp4m_image.h"
#include "../pp4m/pp4m_input.h"

#include "../c4network/net.h"
#include "../c4network/client.h"
#include "../c4network/server.h"

#include "../dashboard/gui.h"
#include "../dashboard/gui_alias.h"
#include "../security/debug.h"
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

void CORE_Chessboard_Init(void) {

    int size_tile = 50;
    int x_b = glo_screen_w / 2 - (size_tile * 4);
    int y_b = glo_screen_h / 2 - (size_tile * 4);

    int colomn = 1;

    bool toggle = false;

    for(int n = 0; n < 64; n++) {

        glo_chess_core_tile[n].piece = NULL;

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

void CORE_GlobalDestroyPiece(CHESS_CORE_PIECE **piece) {

    if (*piece != NULL) {

        CHESS_CORE_PIECE *piece_bl = *piece;

        DEBUG_PrintBox(2, "CORE_GlobalDestroyPiece:");
        DEBUG_PrintBox(2, "  destroy piece = %p", piece_bl);

        SDL_DestroyTexture(piece_bl->texture);
        piece_bl->texture = NULL;
        piece_bl->enum_piece = NONE;

        *piece = NULL;
    }

    return;
}

void CORE_UpdateState_PieceStruct(int tile, CHESS_CORE_PIECE *piece, CHESS_CORE_PLAYER player, int enum_piece) {

    SDL_DestroyTexture(piece->texture);
    FEN_InitPiece(player, piece, enum_piece, tile);

    for (int n = 0; n < 64; n++)
        piece->range[n] = false;

    return;
}

void CORE_ResetGlobal_CoreTile(void) {

    for (int n = 0; n < 64; n++)
        glo_chess_core_tile[n].piece = NULL;

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

void CORE_Chessboard_Reverse(CHESS_CORE_TILE *core_tile) {

    CHESS_CORE_TILE core_tile_bak[64];

    for (int n = 0; n < 64; n++)
        memcpy(&core_tile_bak[n], &core_tile[63 - n], sizeof(core_tile[63 - n]));

    for (int n = 0; n < 64; n++)
        memcpy(&core_tile[n], &core_tile_bak[n], sizeof(core_tile_bak[n]));

    for (int n = 0; n < 64; n++)
        memcpy(&core_tile[n].tag, &core_tile_bak[63 - n].tag, sizeof(CHESS_CORE_TILE_TAG));

    return;
}

CHESS_CORE_PLAYER CORE_ReversePlayer_State(CHESS_CORE_PLAYER player) {
    if (player == WHITE_PLAYER) return BLACK_PLAYER;
    else return WHITE_PLAYER;
}

void CORE_GlobalUpdate_StateRender(void) {

    for (int n = 0; n < 64; n++) {
        SDL_RenderCopy(glo_render, glo_chess_core_tile[n].texture, NULL, &glo_chess_core_tile[n].rect);
        if (glo_chess_core_tile[n].piece != NULL) SDL_RenderCopy(glo_render, glo_chess_core_tile[n].piece->texture, NULL, &glo_chess_core_tile[n].piece->rect);
        DOT_StateGlobalDot(n);
    }

	// gui rendering
    GUI_HookLink_Render(glo_chess_event_hooklist);

	DEBUG_UpdateBox_Render();

    return;
}

char *CORE_NET_ChessboardInit(CHESS_CORE_PLAYER *player, char *buffer) {

    // client side
    char *fen = malloc(sizeof(char) * 256);
    char buf_plvl[4];

    // fen notes
    char buf_fen[128];
    char buf_play[5];
    char buf_castle[5];
    char buf_passant[5];

    int buf_halfm;
    int buf_fullm;

    sscanf(buffer, "%*d %s %s %s %s %s %d %d", buf_plvl, buf_fen, buf_play, buf_castle, buf_passant, &buf_halfm, &buf_fullm);

    DEBUG_PrintBox(2, "recieved parsed: [%s] [%s] [%s] [%s] [%d] [%d]", buf_fen, buf_play, buf_castle, buf_passant, buf_halfm, buf_fullm);

    FEN_PlayerTurn((int*)player, buf_plvl[0]);
    sprintf(fen, "%s %s %s %s %d %d", buf_fen, buf_play, buf_castle, buf_passant, buf_halfm, buf_fullm);

    return fen;
}

int CORE_NET_UpdateLobby(int *code, int *socket, char **buf_arr, int *position_old, int *position_new, int *promotn) {
    if (socket == NULL) return -1;
    int result = -1;

    // send packets (if code is set)
    if (*code != -1) {

        result = cl_clcode_redirect(*code, socket, buf_arr[1], position_old, position_new, promotn);
        if (result > -1) DEBUG_PrintBox(2, "sent buf: [%s]", buf_arr[1]);

    } else memset(buf_arr[1], 0x00, 255);

    // recieve packets
    if (NET_DetectSignal(socket) > 0) {

            cl_svcode_redirect(NET_HandlePacket(socket, buf_arr[0]), buf_arr[0], position_old, position_new, promotn);
            DEBUG_PrintBox(2, "recieved buf: [%s] [%d] [%d] [%d]", buf_arr[0], *position_old, *position_new, *promotn);

    } else memset(buf_arr[0], 0x00, 255);

    return result;
}

int CORE_NET_SocketRedirect(int *socket, CHESS_CORE_PLAYER *player) {
    return (socket != NULL && *player != glo_chess_core_player ? -1 : 0);
}

void CORE_RenderUpdate(SDL_Texture *background, int frames_per_sec, int *timer) {

    if (pp4m_FramerateTimer(frames_per_sec, timer, 0) == true) {
            SDL_RenderClear(glo_render);

            SDL_RenderCopy(glo_render, background, NULL, NULL);
            CORE_GlobalUpdate_StateRender();

            SDL_RenderPresent(glo_render);
    }

    return;
}

int CORE_NET_CommandSequence_REQ_ASSIGN_LOBBY(int *socket, int *buf_cmd, char *buffer) {
    int result = 0;

    // verify that we recieve at least this few packets:

    // first one is SV_STATE_CONFIRM
    // second one is SV_LOBBY_POST_INIT

    if (*buf_cmd == 0) {
        cl_redirect_clcode_REQ(CL_REQ_ASSIGN_LOBBY, buffer);
        result = NET_SendPacket(socket, buffer, strlen(buffer)+1);
        DEBUG_PrintBox(2, "CORE_NET_CommandSequence_REQ_ASSIGN_LOBBY: sent mesg");
        
        if (result != -1)
        	*buf_cmd = CL_REQ_ASSIGN_LOBBY;
    }

    else if (NET_DetectSignal(socket) > 0) {
        result = NET_RecvPacket(socket, buffer, strlen(buffer)+1);
        DEBUG_PrintBox(2, "CORE_NET_CommandSequence_REQ_ASSIGN_LOBBY: recieved mesg");
        
        if (result != -1)
        	*buf_cmd = retrieve_code(buffer);
	}

    // handling at least here few errors
    if (result < 0)
        DEBUG_PrintBox(2, "CORE_NET_CommandSequence_REQ_ASSIGN_LOBBY: error %d", result);

    return result;
}

int CORE_NET_CommandSequence(int *socket, int *master_cmd, int *buf_cmd, char *buffer) {
    int result = 0;

    // buf_cmd is used to keep track of last recorded cmd

    if (*master_cmd == CL_REQ_ASSIGN_LOBBY)
        result = CORE_NET_CommandSequence_REQ_ASSIGN_LOBBY(socket, buf_cmd, buffer);

    if (result == -1) DEBUG_PrintBox(2, "CORE_NET_CommandSequence: error");
    return result;
}

int CORE_InitGame_AwaitServer(SDL_Texture *bg, int *socket) {
	
	char buffer[256];
	int master_cmd = CL_REQ_ASSIGN_LOBBY;
	int buf_cmd = 0;
	int result = 0, result_bak = -1;
	
	PP4M_HOOK *hook_list = pp4m_HOOK_Init();
	
	GUI_TextureAlias *alias = GUI_Alias_InitAlias();
	alias->obj = OBJ_NONE;
	alias->texture = pp4m_TTF_TextureFont(glo_render, OPENSANS_REGULAR, PP4M_BLACK, 24, &alias->dst_rect, 25, 0, ".");
	alias->dst_rect.y = glo_screen_h - alias->dst_rect.h - 25;
	
	pp4m_HOOK_Next(hook_list, alias);
	
	SDL_Event event;
	
	while(1) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) break;
		
		result = CORE_NET_CommandSequence(socket, &master_cmd, &buf_cmd, buffer);
		if (result != result_bak) {
		
			if (result < 0)
				GUI_Alias_UpdateText(alias, OPENSANS_REGULAR, PP4M_RED, 24, "an error occurred");
			else if (result == 0) 
				GUI_Alias_UpdateText(alias, OPENSANS_REGULAR, PP4M_BLACK, 24, "loading...");
			else if (result == 1) {
				GUI_Alias_UpdateText(alias, OPENSANS_REGULAR, PP4M_GREEN, 24, "lobby ready");
				
				DEBUG_PrintBox(2, "buffer: %s", buffer);
			}
			
			result_bak = result;
		}
		
		SDL_RenderClear(glo_render);
		SDL_RenderCopy(glo_render, bg, NULL, NULL);
		GUI_HookLink_Render(hook_list);
		DEBUG_UpdateBox_Render();
		SDL_RenderPresent(glo_render);
		
	}

	return 0;
}

void CORE_InitChess_Play(CHESS_CORE_PLAYER player_view, char *fen_init, int *socket) {

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

    /* set glo_chess_core_ to initial state */
    CORE_ResetGlobal_CorePiece();
    CORE_ResetGlobal_CoreTile();

    /* init pieces for main player */
    FEN_Init(&player, fen_init);

    SDL_Texture *background = pp4m_DRAW_TextureInitColor_Target(glo_render, PP4M_GREY_DARK, 255, NULL, 0, 0, glo_screen_w, glo_screen_h);

    SDL_Event event;
    PP4M_INPUT_POS input_pos;
    MIDDLE_InitTouchPos(&input_pos);

    // testing: cap framerate to 30/60 fps
    int running = 0;
    int fps_timer = clock();
    char buf_in[256], buf_out[256];

    memset(buf_in, 0x00, 255);
    memset(buf_out, 0x00, 255);

    while(running == 0) {

        /* checks if king under attack */
        CHESS_PiecePattern_UpdateState(glo_chess_core_tile, player);

        /* makes the in-game changes during gameplay */
        running = MIDDLE_UpdateChangeState(&event, &player, &input_pos, socket, buf_in, buf_out);

        /* renders everything chessboard releated */
        CORE_RenderUpdate(background, CLOCKS_PER_SEC / 60, &fps_timer);

    }

	// send a leaving command to server
	char buffer[256];
	cl_redirect_clcode_LOBBY_POST(CL_LOBBY_POST_LEAVE, socket, buffer, NULL, NULL, NULL);
    NET_SendPacket(socket, buffer, strlen(buffer)+1);

    SDL_DestroyTexture(background);
    GUI_HookList_Quit(glo_chess_event_hooklist);

    return;
}

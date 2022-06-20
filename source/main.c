// standard libraries
#include <stdio.h>
#include <unistd.h>

#define SDL_MAIN_HANDLED

// local libraries
#include "config.h"
#include "global.h"
#include "pp4m/pp4m.h"
#include "pp4m/pp4m_ttf.h"
#include "pp4m/pp4m_net.h"

#include "version.h"
#include "chess/core.h"
#include "dashboard/gui.h"
#include "security/debug.h"

#include "network/net.h"
#include "network/client.h"
#include "network/server.h"

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    char title_ver[256];
    sprintf(title_ver, "c4chess v%d.%d%s rev%d%d-%s%s%s", MAJOR, MINOR, STATUS_SHORT, BUILD, REVISION, DATE, MONTH, YEAR);

    glo_render = pp4m_Init(glo_window, title_ver, glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    CFG_BootFile_LoadConfig(LANG_DEFAULT);

    CHESS_CORE_PLAYER player = WHITE_PLAYER;

    char *server_addr = NET_DEFAULT_SERVER;
    cli_t socket = pp4m_NET_Init(TCP);

    DEBUG_PrintBox(1, "waiting connection to host...");

    if (NET_ConnectSocketToServer(&socket, server_addr, NET_PORT_TESTNET) < 0) {
        DEBUG_PrintBox(1, "error socket: %s, %d", strerror(errno), pp4m_NET_RecieveError());

        while(1) {
            SDL_RenderClear(glo_render);
            DEBUG_UpdateBox_Render();
            SDL_RenderPresent(glo_render);
        }
    }

    DEBUG_PrintBox(1, "connection established to [%s:%d]", server_addr, NET_PORT_TESTNET);

    // make CL_REQ_ASSIGN_LOBBY in network/
    char buf_1[256];
    char *buf_2 = cli2srv_craftPacket(CL_REQ_ASSIGN_LOBBY);
    NET_SendPacketToServer(&socket, buf_2, strlen(buf_2)+1);
    DEBUG_PrintBox(2, "crafted buf_2: [%s]", buf_2);
    DEBUG_PrintBox(2, "REQ sent, waiting for fillup of lobby...");

    while(1) {

        if (NET_DetectSignal(&socket) > 0) {
            srv2cli_handlePacket(&socket, buf_1);
            DEBUG_PrintBox(2, "recieved buf_1: [%s]", buf_1);
            DEBUG_PrintBox(2, "Initialized room, ready");
            break;
        }

        SDL_RenderClear(glo_render);
        DEBUG_UpdateBox_Render();
        SDL_RenderPresent(glo_render);
    }

    char *fen_notation = CORE_NET_ChessboardInit(&socket, &player, buf_1);
    DEBUG_PrintBox(1, "configured chessboard, ready");

    CORE_InitChess_Play(player, fen_notation, &socket);

    NET_CloseSocket(&socket);

    /*
    GUI_PopupWindow_Core(100, 50, 1080, 590, "test");
    GUI_Testing();
    */

    // Exiting from game
    DEBUG_QuitBox();
    pp4m_TTF_Quit();
    pp4m_Quit(glo_window, glo_render);

    return 0;
}

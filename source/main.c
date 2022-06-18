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
#include "network/net.h"
#include "security/debug.h"

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    char title_ver[256];
    sprintf(title_ver, "c4chess v%d.%d%s rev%d%d-%s%s%s", MAJOR, MINOR, STATUS_SHORT, BUILD, REVISION, DATE, MONTH, YEAR);

    glo_render = pp4m_Init(glo_window, title_ver, glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    CFG_BootFile_LoadConfig(LANG_DEFAULT);

    CHESS_CORE_PLAYER player = WHITE_PLAYER;

    char fen_notation[256];
    char *server_addr = NET_DEFAULT_SERVER;

    DEBUG_PrintBox(1, "waiting connection to host...");

    if (NET_ConnectToServer(server_addr, NET_PORT_TESTNET) < 0) {
        DEBUG_PrintBox(1, "error socket: %s, %d", strerror(errno), pp4m_NET_RecieveError());
        exit(0);
    }

    DEBUG_PrintBox(1, "connection established to [%s]", server_addr);

    // make CL_REQ_ASSIGN_LOBBY in network/

    CORE_NET_ChessboardInit(&socket, &player, fen_notation);
    DEBUG_PrintBox(1, "configured net chessboard, ready");

    CORE_InitChess_Play(player, fen_notation, &socket);

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

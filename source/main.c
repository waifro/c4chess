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

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    char title_ver[256];
    sprintf(title_ver, "c4chess v%d.%d%s rev%d%d-%s%s%s", MAJOR, MINOR, STATUS_SHORT, BUILD, REVISION, DATE, MONTH, YEAR);

    glo_render = pp4m_Init(glo_window, title_ver, glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    CFG_BootFile_LoadConfig();

    CHESS_CORE_PLAYER player = WHITE_PLAYER;

    int socket = 0;
    char fen_notation[256] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";

    // start online 2-player game (wip)
    socket = pp4m_NET_Init(TCP);



    if (socket != -1) {

        printf("waiting connection to host...\n");

        int result = -1;
        while(1) {

            char buf[16];
            pp4m_NET_GetLocalHostname(buf);
            result = pp4m_NETSock_ConnectServerByHostname(&socket, buf, NET_DEFAULT_PORT);

            if (result == 0) break;
            //else if (result == -1) {
                //printf("error socket: %s, %d\n", strerror(errno), pp4m_NET_RecieveError());
               // exit(0);
            //}
        }

        printf("connection established to [%s]\n", "0.0.0.0");

        CORE_NET_ChessboardInit(&socket, &player, fen_notation);
        printf("configured net chessboard, ready\n");

    } else exit(0);

    CORE_InitChess_Play(player, fen_notation, &socket);

    //GUI_PopupWindow_Core(100, 50, 1080, 590, "test");
    //GUI_Testing();

    // Exiting from game
    pp4m_TTF_Quit();
    pp4m_Quit(glo_window, glo_render);

    return 0;
}

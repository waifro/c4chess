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

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    char title_ver[256];
    sprintf(title_ver, "c4chess v%d.%d%s rev%d%d-%s%s%s", MAJOR, MINOR, STATUS_SHORT, BUILD, REVISION, DATE, MONTH, YEAR);

    glo_render = pp4m_Init(glo_window, title_ver, glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    CFG_BootFile_LoadConfig();

    CHESS_CORE_PLAYER player;
    char fen_notation[256];

    // start online 2-player game (wip)
    int socket = pp4m_NET_Init(TCP);

    if (socket != -1) {
        char local_addr[256];
        pp4m_NET_GetLocalAddress(socket, local_addr);

        // connect to the server
        printf("waiting connection to host...\n");

        while(1) {
            pp4m_NETSock_ConnectServerByAddress(&socket, local_addr, 62443);

            if (pp4m_NET_RecieveError() == 0) break;
            else if (pp4m_NET_RecieveError() == -1) {
                printf("error socket: %d\n", pp4m_NET_RecieveError());
                exit(0);
            }
        }

        printf("connection established to [%s]\n", local_addr);

        CORE_NET_InitGlobal(&socket, &player, fen_notation);
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

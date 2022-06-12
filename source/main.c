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

    CHESS_CORE_PLAYER player = WHITE_PLAYER;


    char fen_notation[256] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    int roomId = 0;
    /*
    // start online 2-player game (wip)
    int socket = pp4m_NET_Init(TCP);


    if (socket != -1) {

        printf("waiting connection to host...\n");

        int result = -1;
        while(1) {
            result = pp4m_NETSock_ConnectServerByAddress(&socket, "127.0.0.1", 62443);

            if (result == 0) break;
            else if (result == -1) {
                printf("error socket: %d\n", pp4m_NET_RecieveError());
                exit(0);
            }
        }

        printf("connection established to [%s]\n", "127.0.0.1");

        CORE_NET_InitGlobal(&socket, &roomId, &player, fen_notation);
        printf("configured net chessboard, ready\n");

    } else exit(0);
    */
    CORE_InitChess_Play(player, fen_notation, NULL, roomId);

    //GUI_PopupWindow_Core(100, 50, 1080, 590, "test");
    //GUI_Testing();

    // Exiting from game
    pp4m_TTF_Quit();
    pp4m_Quit(glo_window, glo_render);

    return 0;
}

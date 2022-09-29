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
#include "pp4m/pp4m_draw.h"

#include "version.h"
#include "chess/core.h"
#include "dashboard/gui.h"
#include "dashboard/menu.h"
#include "security/debug.h"
#include "engine/engine.h"

#include "c4network/net.h"
#include "c4network/client.h"
#include "c4network/server.h"

int main (int argc, char *argv[]) {
    (void)argc; (void)argv;

    char title_ver[256];
    sprintf(title_ver, "c4chess v%d.%d%s rev%d%d-%s%s%s", MAJOR, MINOR, STATUS_SHORT, BUILD, REVISION, DATE, MONTH, YEAR);

    glo_render = pp4m_Init(glo_window, title_ver, glo_screen_w, glo_screen_h, WINDOW);
    pp4m_TTF_Init();

    CFG_BootFile_LoadConfig(LANG_DEFAULT);

    GLOBAL_InitUser_Guest(&glo_user);
    
    SDL_Texture *background = pp4m_DRAW_TextureInitColor(glo_render, PP4M_WHITE, NULL, 0, 0, glo_screen_w, glo_screen_h);
    
    char *server_addr = NET_DEFAULT_SERVER;
    cli_t socket = pp4m_NET_Init(TCP);
    
    DEBUG_PrintBox(2, "waiting connection to host...");
    
    if (NET_ConnectSocketToServer(&socket, server_addr, NET_PORT_TESTNET) < 0)
    	DEBUG_PrintBox(2, "error socket: [%d] %s, %d", socket, strerror(errno), pp4m_NET_RecieveError());
	else DEBUG_PrintBox(2, "connection established to [%s:%d]", server_addr, NET_PORT_TESTNET);

    printf("glo_engine strt: %p\n", glo_engine_net_buffer);

    ENGINE_NET_InitFifo(NULL);

    char *sniff = "ciao";
    char *clocf = "bruh";

    ENGINE_NET_AllocBuffer(NULL, sniff);
    ENGINE_NET_AllocBuffer(NULL, clocf);
    
    printf("glo_engine 1: %s\n", (char*)glo_engine_net_buffer->ptr);
    printf("glo_engine 2: %s\n", (char*)glo_engine_net_buffer->next->ptr);

    ENGINE_NET_QuitFifo(NULL);

    printf("glo_engine end: %p\n", glo_engine_net_buffer);

    //MENU_Core(background, &socket);
    
    NET_CloseSocket(&socket);
    GLOBAL_DestrUser(&glo_user);

    // Exiting from game
    DEBUG_QuitBox();
    pp4m_TTF_Quit();
    pp4m_Quit(glo_window, glo_render);

    return 0;
}

#include <stdio.h>

#include "../pp4m/pp4m_net.h"
#include "net.h"

/*
int NET_CreateBroadcastServer(void) {

    int result = 0;

    pp4m_NET_Init(TCP);
    result = pp4m_NET_ServerStart(NET_DEFAULT_PORT);

    return (result);
}

int NET_ConnectToServer(char *server) {

    int result = 0;

    pp4m_NET_Init(TCP);

    if (server == NULL) result = pp4m_NET_ConnectServerByAddress(NET_DEFAULT_SERVER, NET_DEFAULT_PORT);
    else result = pp4m_NET_ConnectServerByAddress(server, NET_DEFAULT_PORT);

    return (server);
}

void NET_CloseConnection(void) {

    pp4m_NET_ClientClose();
    pp4m_NET_ServerClose();
    pp4m_NET_Quit();

    return;
}
*/

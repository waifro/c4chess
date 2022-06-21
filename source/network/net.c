#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "../pp4m/pp4m_net.h"
#include "../chess/core.h"
#include "net.h"

int NET_ConnectSocketToServer(cli_t *socket, char *server, int port) {
    int result = -1;

    pp4m_NET_Init(TCP);

    if (server == NULL) result = pp4m_NETSock_ConnectServerByAddress(*socket, NET_DEFAULT_SERVER, port);
    else result = pp4m_NETSock_ConnectServerByAddress(*socket, server, port);

    return (result);
}

int NET_DetectSignal(cli_t *restrict socket) {

    fd_set setfd;
    FD_ZERO(&setfd);
    FD_SET(*socket, &setfd);
    struct timeval timeout = {0, 0};

    if (select(*socket + 1, &setfd, NULL, NULL, &timeout) == -1) return -2;
    if (FD_ISSET(*socket, &setfd) != 0) return 1;
    else return -1;
}

int NET_CloseSocket(cli_t *socket) {

    if (socket != NULL) {
        close(*socket);
        *socket = 0;
    }

    return 0;
}

int NET_SendPacketToServer(cli_t *socket, char *buffer, size_t n_bytes) {
    int result = -1;

    if (socket != NULL)
        result = send(*socket, buffer, n_bytes, 0);

    return result;
}

/*
int NET_CreateBroadcastServer(void) {

    int result = 0;

    pp4m_NET_Init(TCP);
    result = pp4m_NET_ServerStart(NET_DEFAULT_PORT);

    return (result);
}

void NET_CloseConnection(void) {

    pp4m_NET_ClientClose();
    pp4m_NET_ServerClose();
    pp4m_NET_Quit();

    return;
}
*/

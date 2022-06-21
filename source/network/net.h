#ifndef NET_H
#define NET_H

#include "client.h"

#define NET_DEFAULT_SERVER "195.58.38.13"
#define NET_PORT_MAINNET 62443
#define NET_PORT_TESTNET 61338

int NET_ConnectSocketToServer(cli_t *socket, char *server, int port);

int NET_CloseSocket(cli_t *socket);
int NET_DetectSignal(cli_t *restrict socket);

int NET_SendPacketToServer(cli_t *socket, char *buffer, size_t n_bytes);

/*
int NET_CreateBroadcastServer(void);
void NET_CloseConnection(void);
*/

#endif

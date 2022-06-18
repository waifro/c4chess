#ifndef NET_H
#define NET_H

#define NET_DEFAULT_SERVER "195.58.38.13"
#define NET_PORT_MAINNET 62443
#define NET_PORT_TESTNET 61338

int NET_CreateBroadcastServer(void);
int NET_ConnectToServer(char *server, int port);
void NET_CloseConnection(void);

#endif

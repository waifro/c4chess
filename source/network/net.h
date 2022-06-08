#ifndef NET_H
#define NET_H

#include "../chess/core.h"

#define NET_DEFAULT_SERVER "195.58.38.136"
#define NET_DEFAULT_PORT 62443

int NET_SocketRedirect_ReversePlayer(CHESS_CORE_PLAYER *player, void *socket);

/*
int NET_CreateBroadcastServer(void);
int NET_ConnectToServer(char *server);
void NET_CloseConnection(void);
*/

#endif

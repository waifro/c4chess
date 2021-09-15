#ifndef NET_H
#define NET_H

#define NET_DEFAULT_SERVER "195.58.38.136"
#define NET_DEFAULT_PORT 62443

int NET_CreateBroadcastServer(void);
int NET_ConnectToServer(char *server);
void NET_CloseConnection(void);

#endif

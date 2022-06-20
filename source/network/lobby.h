#ifndef LOBBY_H
#define LOBBY_H

#include "net_utils.h"

int lobby_checkroom_avail(net_lobby *lobby, int room);
int lobby_checkroom_isfull(net_lobby *lobby, int room);
int lobby_checkroom_isready(net_lobby *lobby, int room);
int lobby_checkroom_isbusy(net_lobby *lobby, int room);
int lobby_checkroom_cli(net_lobby *lobby, cli_t *client, int room);

int lobby_assign_cli(net_lobby *lobby, cli_t *client);
int lobby_random_start(net_lobby *lobby, int room, char *fen);
int lobby_updateroom_cli_left(net_lobby *lobby, cli_t *client);
int lobby_redirect_buf(net_lobby *lobby, cli_t *client, int room, char *buffer);

#endif

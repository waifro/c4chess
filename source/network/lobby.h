#ifndef LOBBY_H
#define LOBBY_H

#include "net_utils.h"

int lobby_checkroom_avail(int room);
int lobby_checkroom_isfull(int room);
int lobby_checkroom_isready(int room);
int lobby_checkroom_isbusy(int room);
int lobby_checkroom_cli(cli_t *client, int room);

int lobby_assign_cli(cli_t *client);
int lobby_random_start(int room, char *fen);
int lobby_updateroom_cli_left(cli_t *client);
int lobby_redirect_buf(cli_t *client, int room, char *buffer);

#endif

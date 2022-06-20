#ifndef NET_UTILS_H
#define NET_UTILS_H

#define cli_t int
#include "../pp4m/pp4m_net.h"

#define MAX_CLIENTS 2
#define MAX_LOBBY MAX_CLIENTS/2

typedef enum {
    LB_AVAIL = 0,
    LB_FULL = 1,
    LB_BUSY = 2,
    LB_ERROR = 3,
    LB_BLOCKED = 4
} LOBBY_STATUS;

// room of two clients
typedef struct {
    cli_t *cli_a, *cli_b;
} pair_cli_t;

typedef struct {
    pair_cli_t pair;
    LOBBY_STATUS status;
} net_lobby;

int retrieve_code(char *mesg);
int verify_mesg_recv(char *mesg);
int generate_val(int max);
void init_client_list(cli_t *client_list, int max);
void init_lobby_list(net_lobby *lobby_list, int max);

int client_accept(int master_socket, struct sockaddr_in *addr);
void client_disconnect(cli_t *client);

#endif

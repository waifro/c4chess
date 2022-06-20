#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "../pp4m/pp4m_net.h"

int retrieve_code(char *mesg) {

    int code = 0;
    sscanf(mesg, "%d %*s", &code);

    return code;
}

int verify_mesg_recv(char *mesg) {

    if (strlen(mesg) < 3) return -1;
    else if (strlen(mesg) > 255) return -1;

    return 0;
}

int generate_val(int max) {
    //srand(time(NULL));
    return (rand() % max);
}

void init_client_list(cli_t *client_list, int max) {
    for (int i = 0; i < max; i++) {
        client_list[i] = 0;
    }
}

void init_lobby_list(net_lobby *lobby_list, int max) {
    for (int i = 0; i < max; i++) {
        lobby_list[i].pair.cli_a = NULL;
        lobby_list[i].pair.cli_b = NULL;
        lobby_list[i].status = LB_AVAIL;
    }
}

int client_accept(int master_socket, struct sockaddr_in *addr) {
    cli_t new_client = -1;
    socklen_t addr_size = sizeof(struct sockaddr);
    new_client = accept(master_socket, (struct sockaddr*)addr, &addr_size);
    return new_client;
}

void client_disconnect(cli_t *client) {
    *client = 0;
    return;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lobby.h"
#include "server.h"
#include "client.h"
#include "net.h"
#include "net_utils.h"
#include "../pp4m/pp4m_net.h"

int lobby_checkroom_avail(net_lobby *lobby, int room) {
    if (lobby[room].status == LB_AVAIL) {
        if (*lobby[room].pair.cli_a == 0) return 1;
        else if (*lobby[room].pair.cli_b == 0) return 2;
    }

    return -1;
}

int lobby_checkroom_isfull(net_lobby *lobby, int room) {
    if (lobby[room].status == LB_AVAIL)
        if (*lobby[room].pair.cli_a != 0 && *lobby[room].pair.cli_b != 0) return 1;

    return -1;
}

int lobby_checkroom_isready(net_lobby *lobby, int room) {
    if (lobby[room].status == LB_FULL) return 1;
    else return -1;
}

int lobby_checkroom_isbusy(net_lobby *lobby, int room) {
    if (lobby[room].status == LB_BUSY) return 1;
    else return -1;
}

int lobby_checkroom_cli(net_lobby *lobby, cli_t *client, int room) {
    if (lobby[room].pair.cli_a == client) return 1;
    else if (lobby[room].pair.cli_b == client) return 2;
    else return -1;
}

int lobby_assign_cli(net_lobby *lobby, cli_t *client) {
    int result = -1;

    for (int i = 0; i < MAX_LOBBY; i++) {
        result = lobby_checkroom_avail(lobby, i);
        if (result == -1) continue;

        // assign the lobby
        if (result == 1) lobby[i].pair.cli_a = client;
        else if (result == 2) lobby[i].pair.cli_b = client;

        // target the lobby
        if (lobby_checkroom_isfull(lobby, i) == 1) lobby[i].status = LB_FULL;
        break;
    }

    return 0;
}

int lobby_random_start(net_lobby *lobby, int room, char *fen) {
    int result = -1;

    char buf[256];
    sprintf(buf, "%d w %s", SV_POST_LOBBY_START, fen);

    if (generate_val(100) < 50) {
        result = send(*lobby[room].pair.cli_a, buf, strlen(buf) + 1, 0);
        if (result == -1) { perror("lobby_random_start 1"); return -1; }
        buf[0] = 'b';
        result = send(*lobby[room].pair.cli_b, buf, strlen(buf) + 1, 0);
        if (result == -1) { perror("lobby_random_start 2"); return -1; }
    } else {
        send(*lobby[room].pair.cli_b, buf, strlen(buf) + 1, 0);
        if (result == -1) { perror("lobby_random_start 1"); return -1; }
        buf[0] = 'b';
        send(*lobby[room].pair.cli_a, buf, strlen(buf) + 1, 0);
        if (result == -1) { perror("lobby_random_start 2"); return -1; }
    }

    lobby[room].status = LB_BUSY;

    return result;
}

int lobby_updateroom_cli_left(net_lobby *lobby, cli_t *client) {
    int result = -1;
    int room = -1;

    for (room = 0; room < MAX_LOBBY; room++) {
        result = lobby_checkroom_cli(lobby, client, room);
        if (result == -1) continue;

        if (result == 1) lobby[room].pair.cli_a = NULL;
        else if (result == 2) lobby[room].pair.cli_b = NULL;

        lobby[room].status = LB_ERROR;

        // room cleaned
        if (lobby[room].pair.cli_a == NULL && lobby[room].pair.cli_b == NULL)
            lobby[room].status = LB_AVAIL;

        break;
    }

    return room;
}

int lobby_redirect_buf(net_lobby *lobby, cli_t *client, int room, char *buffer) {
    int result = -1;

    result = lobby_checkroom_cli(lobby, client, room);
    if (result == -1) return -2;
    else if (result == 1) result = send(*lobby[room].pair.cli_b, buffer, strlen(buffer) + 1, 0);
    else if (result == 2) result = send(*lobby[room].pair.cli_a, buffer, strlen(buffer) + 1, 0);

    return result;
}

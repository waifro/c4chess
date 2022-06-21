#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "lobby.h"

#include "net_utils.h"
#include "../pp4m/pp4m_net.h"

int clcode_status_STATE(int code) {
    return (code > CL_STATE_START && code < CL_STATE_END ? 0 : -1);
}

int clcode_status_REQ(int code) {
    return (code > CL_REQ_START && code < CL_REQ_END ? 0 : -1);
}

int clcode_status_LOBBY_REQ(int code) {
    return (code > CL_REQ_LOBBY_START && code < CL_REQ_LOBBY_END ? 0 : -1);
}

int clcode_status_POST(int code) {
    return (code > CL_POST_START && code < CL_POST_END ? 0 : -1);
}

int clcode_status_LOBBY_POST(int code) {
    return (code > CL_POST_LOBBY_START && code < CL_POST_LOBBY_END ? 0 : -1);
}

int clcode_status_LOBBY(int code) {
    return ((code > CL_REQ_LOBBY_START && code < CL_REQ_LOBBY_END) || (code > CL_POST_LOBBY_START && code < CL_POST_LOBBY_END) ? 0 : -1);
}

int cli2srv_REQ_ASSIGN_LOBBY(char *buffer) {
    int result = -1;

    if (buffer != NULL) {
        result = sprintf(buffer, "%d", CL_REQ_ASSIGN_LOBBY);
        if (result > 0) result = 0;
    }

    return result;
}

int cli2srv_REQ_redirect(int code, char *buffer) {
    int result = -1;

    switch(code) {
    case CL_REQ_ASSIGN_LOBBY:
        result = cli2srv_REQ_ASSIGN_LOBBY(buffer);
        break;

    default:
        break;
    }

    return result;
}

char *cli2srv_craftPacket(int code) {

    char *buffer = malloc(sizeof(char) * 256);

    cli2srv_redirect(code, buffer);

    return buffer;
}

int cli2srv_redirect(int code, char *buffer) {
    int result = 0;

    if (clcode_status_STATE(code) == 0) result = 0; // im not sure what to do with this and cli_t.status
    else if (clcode_status_REQ(code) == 0) result = cli2srv_REQ_redirect(code, buffer);
    else if (clcode_status_POST(code) == 0) {}; //result = clcode_POST_redirect(code, socket, buffer);

    return result;
}

int srv2cli_handlePacket(cli_t *socket, char *buffer) {

    if (recv(*socket, buffer, 255, 0) < 0) {
        //client_disconnect(client);
        memset(buffer, 0x00, 255);
        return -1;
    }

    int result = 0;
    result = verify_mesg_recv(buffer);
    if (result < 0) return -1;

    result = retrieve_code(buffer);
    if (result < 0) return -1;

    //clcode_redirect(result, client, room, buffer);

    return result;
}

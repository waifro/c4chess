#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "lobby.h"

#include "net.h"
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

int clcode_REQ_redirect(int code, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer;
    int result = -1;

    switch(code) {
        case CL_REQ_ASSIGN_LOBBY:
            result = lobby_assign_cli(client);
            break;

        default:
            break;
    }

    return result;
}

int clcode_POST_redirect(int code, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer;
    int result = -1;

    switch(code) {

        default:
            break;
    }

    return result;
}

int clcode_LOBBY_REQ_redirect(int code, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer;
    int result = -1;

    switch(code) {

        case CL_REQ_LOBBY_NICKNAME:
            break;

        default:
            break;
    }

    return result;
}

int clcode_LOBBY_POST_redirect(int code, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer;
    int result = -1;

    switch(code) {
        case CL_POST_LOBBY_LEAVE:
            //result = lobby_assign_cli(client);
            break;

        case CL_POST_LOBBY_MOVE:
            result = lobby_redirect_buf(client, room, buffer);
            break;

        case CL_POST_LOBBY_MESG:
            result = lobby_redirect_buf(client, room, buffer);
            break;

        default:
            break;
    }

    return result;
}

int clcode_redirect(int code, cli_t *client, int room, char *buffer) {
    int result = 0;

    if (clcode_status_STATE(code) == 0) result = 0; // im not sure what to do with this and cli_t.status
    else if (clcode_status_REQ(code) == 0) result = clcode_REQ_redirect(code, client, room, buffer);
    else if (clcode_status_POST(code) == 0) result = clcode_POST_redirect(code, client, room, buffer);

    return result;
}

int CL_HandleSrv_Packet(cli_t *client, char *buffer) {

    if (recv(*client, buffer, 255, 0) < 0) {
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

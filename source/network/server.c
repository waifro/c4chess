#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "lobby.h"

#include "net_utils.h"
#include "../pp4m/pp4m_net.h"

int sv_status_STATE(int code) {
    return (code > SV_STATE_START && code < SV_STATE_END ? 0 : -1);
}

int sv_status_REQ(int code) {
    return (code > SV_REQ_START && code < SV_REQ_END ? 0 : -1);
}

int sv_status_POST(int code) {
    return (code > SV_POST_START && code < SV_POST_END ? 0 : -1);
}

int sv_status_LOBBY(int code) {
    return ((code > SV_LOBBY_REQ_START && code < SV_LOBBY_REQ_END) || (code > SV_LOBBY_POST_START && code < SV_LOBBY_POST_END) ? 0 : -1);
}

int sv_status_LOBBY_REQ(int code) {
    return (code > SV_LOBBY_REQ_START && code < SV_LOBBY_REQ_END ? 0 : -1);
}

int sv_status_LOBBY_POST(int code) {
    return (code > SV_LOBBY_POST_START && code < SV_LOBBY_POST_END ? 0 : -1);
}





int sv_redirect_svcode_STATE(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {
        default:
            break;
    }

    return result;
}

int sv_redirect_svcode_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {
        case SV_REQ_NICKNAME:
            break;

        default:
            break;
    }

    return result;
}

int sv_redirect_svcode_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {
        case SV_LOBBY_POST_START:
            result = lobby_random_start(lobby, room, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0");
            break;

        default:
            break;
    }

    return result;
}

int sv_redirect_svcode_LOBBY_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {
        default:
            break;
    }

    return result;
}

int sv_redirect_svcode_LOBBY_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {
        default:
            break;
    }

    return result;
}







int sv_redirect_clcode_STATE(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {

        default:
            break;
    }

    return result;
}

int sv_redirect_clcode_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {
        case CL_REQ_ASSIGN_LOBBY:
            result = lobby_assign_cli(lobby, client);
            break;

        default:
            break;
    }

    return result;
}

int sv_redirect_clcode_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {

        default:
            break;
    }

    return result;
}

int sv_redirect_clcode_LOBBY_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {

        case CL_LOBBY_REQ_NICKNAME:
            break;

        default:
            break;
    }

    return result;
}

int sv_redirect_clcode_LOBBY_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    (void)code; (void)client; (void)room; (void)buffer; (void)lobby;
    int result = -1;

    switch(code) {
        case CL_LOBBY_POST_LEAVE:
            //result = lobby_assign_cli(client);
            break;

        case CL_LOBBY_POST_MOVE:
            result = lobby_SV_POST_LOBBY_MOVE(lobby, client, room, buffer);
            break;

        case CL_LOBBY_POST_MESG:
            result = lobby_SV_POST_LOBBY_MESG(lobby, client, room, buffer);
            break;

        default:
            break;
    }

    return result;
}

// server: client sent a packet to the server
int sv_clcode_redirect(int code, net_lobby *lobby, cli_t *client, int room, char *buffer) {
    int result = 0;

    if (cl_status_STATE(code) == 0) result = 0; // im not sure what to do with this and cli_t.status
    else if (cl_status_REQ(code) == 0) result = sv_redirect_clcode_REQ(code, lobby, client, room, buffer);
    else if (cl_status_POST(code) == 0) result = sv_redirect_clcode_POST(code, lobby, client, room, buffer);
    else if (cl_status_LOBBY_REQ(code) == 0) result = sv_redirect_clcode_LOBBY_REQ(code, lobby, client, room, buffer);
    else if (cl_status_LOBBY_POST(code) == 0) result = sv_redirect_clcode_LOBBY_POST(code, lobby, client, room, buffer);

    return result;
}

int sv_handlePacket(cli_t *client, char *buffer) {

    if (recv(*client, buffer, 255, 0) < 0) {
        memset(buffer, 0x00, 255);
        return -1;
    }

    int result = 0;
    result = verify_mesg_recv(buffer);
    if (result < 0) return -1;

    result = retrieve_code(buffer);
    if (result < 0) return -1;

    return result;
}

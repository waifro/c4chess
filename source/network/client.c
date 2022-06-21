#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "lobby.h"

#include "net_utils.h"
#include "../pp4m/pp4m_net.h"

int cl_status_STATE(int code) {
    return (code > CL_STATE_START && code < CL_STATE_END ? 0 : -1);
}

int cl_status_REQ(int code) {
    return (code > CL_REQ_START && code < CL_REQ_END ? 0 : -1);
}

int cl_status_POST(int code) {
    return (code > CL_POST_START && code < CL_POST_END ? 0 : -1);
}

int cl_status_LOBBY(int code) {
    return ((code > CL_LOBBY_REQ_START && code < CL_LOBBY_REQ_END)|| (code > CL_LOBBY_POST_START && code < CL_LOBBY_POST_END) ? 0 : -1);
}

int cl_status_LOBBY_REQ(int code) {
    return (code > CL_LOBBY_REQ_START && code < CL_LOBBY_REQ_END ? 0 : -1);
}

int cl_status_LOBBY_POST(int code) {
    return (code > CL_LOBBY_POST_START && code < CL_LOBBY_POST_END ? 0 : -1);
}






int cl_redirect_svcode_STATE(int code, char *buffer) {
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_svcode_REQ(int code, char *buffer) {
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_svcode_POST(int code, char *buffer) {
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}
int cl_redirect_svcode_LOBBY_REQ(int code, char *buffer) {
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}
int cl_redirect_svcode_LOBBY_POST(int code, char *buffer) {
    int result = -1;

    switch(code) {

    case SV_LOBBY_POST_MOVE:
        break;

    case SV_LOBBY_POST_MESG:
        break;

    default:
        break;
    }

    return result;
}





int cl_redirect_clcode_STATE(int code, char *buffer) {
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_clcode_REQ(int code, char *buffer) {
    int result = -1;

    switch(code) {
    case CL_REQ_ASSIGN_LOBBY:
        result = cl_REQ_ASSIGN_LOBBY(buffer);
        break;

    default:
        break;
    }

    return result;
}

int cl_redirect_clcode_POST(int code, char *buffer) {
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_clcode_LOBBY_REQ(int code, char *buffer) {
    int result = -1;

    switch(code) {
    case CL_REQ_LOBBY_NICKNAME:
        break;

    default:
        break;
    }

    return result;
}

int cl_redirect_clcode_LOBBY_POST(int code, char *buffer) {
    int result = -1;

    switch(code) {
    case CL_POST_LOBBY_LEAVE:
        break;

    case CL_POST_LOBBY_MOVE:
        break;

    case CL_POST_LOBBY_MESG:
        break;

    default:
        break;
    }

    return result;
}






int cl_REQ_ASSIGN_LOBBY(char *buffer) {
    int result = -1;

    if (buffer != NULL) {
        result = sprintf(buffer, "%d", CL_REQ_ASSIGN_LOBBY);
        if (result > 0) result = 0;
    }

    return result;
}

// client: server sent a packet
int cl_svcode_redirect(int code, char *buffer, int *position_old, int *position_new) {
    int result = 0;

    if (sv_status_STATE(code) == 0) result = 0; // im not sure what to do with this and cli_t.status
    else if (sv_status_REQ(code) == 0) result = cl_redirect_svcode_REQ(code, lobby, client, room, buffer);
    else if (sv_status_POST(code) == 0) result = cl_redirect_svcode_POST(code, lobby, client, room, buffer);
    else if (sv_status_LOBBY_REQ(code) == 0) result = cl_redirect_svcode_LOBBY_REQ(code, lobby, client, room, buffer);
    else if (sv_status_LOBBY_POST(code) == 0) result = cl_redirect_svcode_LOBBY_POST(code, lobby, client, room, buffer);

    return result;
}

// client: client about to create a crafted packet to send
int cl_clcode_redirect(int code, char *buffer, int *position_old, int *position_new) {
    int result = 0;

    if (cl_status_STATE(code) == 0) result = 0; // im not sure what to do with this and cli_t.status
    else if (cl_status_REQ(code) == 0) result = cl_redirect_clcode_REQ(code, buffer);
    else if (cl_status_POST(code) == 0) result = cl_redirect_clcode_POST(code, buffer);
    else if (cl_status_LOBBY_REQ(code) == 0) result = cl_redirect_clcode_LOBBY_REQ(code, buffer, position_old, position_new);
    else if (cl_status_LOBBY_POST(code) == 0) result = cl_redirect_clcode_LOBBY_POST(code, buffer, position_old, position_new);

    return result;
}

int cl_handlePacket(cli_t *client, char *buffer) {

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

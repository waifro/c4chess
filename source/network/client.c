#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net.h"
#include "server.h"
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
    (void)code; (void)buffer;
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_svcode_REQ(int code, char *buffer) {
    (void)code; (void)buffer;
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_svcode_POST(int code, char *buffer) {
    (void)code; (void)buffer;
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_svcode_LOBBY_REQ(int code, char *buffer, int *position_old, int *position_new, int *promotn) {
    (void)code; (void)buffer; (void)position_old; (void)position_new; (void)promotn;
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_svcode_LOBBY_POST(int code, char *buffer, int *position_old, int *position_new, int *promotn) {
    int result = -1;

    switch(code) {

    case SV_LOBBY_POST_MOVE:
        result = cl_SV_LOBBY_POST_MOVE(buffer, position_old, position_new, promotn);
        break;

    case SV_LOBBY_POST_MESG:
        break;

    default:
        break;
    }

    return result;
}






int cl_SV_LOBBY_POST_MOVE(char *buffer, int *position_old, int *position_new, int *promotn) {
    int result = -1;

    if (buffer != NULL) {
        result = sscanf(buffer, "%*d %d %d %d", position_old, position_new, promotn);
        if (result > 0) result = 0;
    }

    return result;
}







int cl_redirect_clcode_STATE(int code, char *buffer) {
    (void)code; (void)buffer;
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_clcode_REQ(int code, char *buffer) {
    (void)code; (void)buffer;
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
    (void)code; (void)buffer;
    int result = -1;

    switch(code) {
    default:
        break;
    }

    return result;
}

int cl_redirect_clcode_LOBBY_REQ(int code, char *buffer, int *position_old, int *position_new, int *promotn) {
    (void)code; (void)buffer; (void)position_old; (void)position_new; (void)promotn;
    int result = -1;

    switch(code) {
    case CL_LOBBY_REQ_NICKNAME:
        break;

    default:
        break;
    }

    return result;
}

int cl_redirect_clcode_LOBBY_POST(int code, cli_t *socket, char *buffer, int *position_old, int *position_new, int *promotn) {
    int result = -1;

    switch(code) {
    case CL_LOBBY_POST_LEAVE:
        break;

    case CL_LOBBY_POST_MOVE:
        result = cl_POST_LOBBY_MOVE(buffer, position_old, position_new, promotn);
        break;

    case CL_LOBBY_POST_MESG:
        break;

    default:
        break;
    }

    if (result > -1)
        result = NET_SendPacketToServer(socket, buffer, strlen(buffer)+1);

    return result;
}







int cl_POST_LOBBY_MOVE(char *buffer, int *position_old, int *position_new, int *promotn) {
    int result = -1;

    if (buffer != NULL) {
        result = sprintf(buffer, "%d %d %d %d", CL_LOBBY_POST_MOVE, *position_old, *position_new, *promotn);
        if (result > 0) result = 0;
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
int cl_svcode_redirect(int code, char *buffer, int *position_old, int *position_new, int *promotn) {
    int result = 0;

    if (sv_status_STATE(code) == 0) result = 0; // im not sure what to do with this and cli_t.status
    else if (sv_status_REQ(code) == 0) result = cl_redirect_svcode_REQ(code, buffer);
    else if (sv_status_POST(code) == 0) result = cl_redirect_svcode_POST(code, buffer);
    else if (sv_status_LOBBY_REQ(code) == 0) result = cl_redirect_svcode_LOBBY_REQ(code, buffer, position_old, position_new, promotn);
    else if (sv_status_LOBBY_POST(code) == 0) result = cl_redirect_svcode_LOBBY_POST(code, buffer, position_old, position_new, promotn);

    return result;
}

// client: client about to create a crafted packet to send
int cl_clcode_redirect(int code, int *socket, char *buffer, int *position_old, int *position_new, int *promotn) {
    if (socket == NULL || code == -1) return -1;
    int result = 0;

    if (cl_status_STATE(code) == 0) result = 0; // im not sure what to do with this and cli_t.status
    else if (cl_status_REQ(code) == 0) result = cl_redirect_clcode_REQ(code, buffer);
    else if (cl_status_POST(code) == 0) result = cl_redirect_clcode_POST(code, buffer);
    else if (cl_status_LOBBY_REQ(code) == 0) result = cl_redirect_clcode_LOBBY_REQ(code, buffer, position_old, position_new, promotn);
    else if (cl_status_LOBBY_POST(code) == 0) result = cl_redirect_clcode_LOBBY_POST(code, socket, buffer, position_old, position_new, promotn);

    return result;
}

int cl_GrabPacket(cli_t *client, char *buffer) {

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

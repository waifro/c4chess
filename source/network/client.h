#ifndef CLIENT_H
#define CLIENT_H

#include "net_utils.h"

#define cli_t int

// client command codes which will be sent to the server/lobby
typedef enum {
    CL_STATE_START = 100,               // STATE START                                      ||
        CL_STATE_IDLE,                  // post idle ...................................... || "1xx", code
        CL_STATE_CONFIRM,               // post confirm ................................... || "1xx", code
        CL_STATE_DENY,                  // post deny ...................................... || "1xx", code
    CL_STATE_END,                       // STATE END                                        ||

    CL_REQ_START = 200,                 // REQ START                                        ||
        CL_REQ_TOTAL_FILE,              // total files to download ........................ || "2xx 21", code
        CL_REQ_TOTAL_USER,              // total users at the moment connected ............ || "2xx 15 128", code, clients_active, max_clients
        CL_REQ_TOTAL_LOBBY,             // total lobbies (AVAIL/BUSY/etc..) ............... || "2xx 2 64", code, lobbies_active, max_lobbies
        CL_REQ_ASSIGN_LOBBY,            // requesting to be assigned to a new lobby ....... || "2xx", code
    CL_REQ_END,                         // REQ END                                          ||

    CL_LOBBY_REQ_START,                 // REQ LOBBY_START                                  ||
        CL_LOBBY_REQ_NICKNAME,          // client asking nickname of the other client ..... || "2xx", code
    CL_LOBBY_REQ_END,                   // REQ LOBBY_END                                    ||

    CL_POST_START = 300,                // POST START                                       ||
        CL_POST_RAW,                    // post raw data to server ........................ || "3xx ...", code, data
    CL_POST_END,                        // POST END                                         ||

    CL_LOBBY_POST_START,                // POST LOBBY_START                                 ||
        CL_LOBBY_POST_LEAVE,            // client decided to exit from lobby .............. || "3xx", code
        CL_LOBBY_POST_MOVE,             // client prompting to the lobby the new move ..... || "3xx 24 43 -1", code, old, new, prom
        CL_LOBBY_POST_MESG,             // client prompting to the lobby the incoming mesg  || "3xx how long have you been playing chess?", code, mesg*
    CL_LOBBY_POST_END,                  // POST LOBBY_END                                   ||

} CLIENT_CMD;

int cl_status_STATE(int code);
int cl_status_REQ(int code);
int cl_status_POST(int code);
int cl_status_LOBBY(int code);
int cl_status_LOBBY_REQ(int code);
int cl_status_LOBBY_POST(int code);

int cl_redirect_svcode_STATE(int code, char *buffer);
int cl_redirect_svcode_REQ(int code, char *buffer);
int cl_redirect_svcode_POST(int code, char *buffer);
int cl_redirect_svcode_LOBBY_REQ(int code, char *buffer, int *position_old, int *position_new, int *promotn);
int cl_redirect_svcode_LOBBY_POST(int code, char *buffer, int *position_old, int *position_new, int *promotn);

int cl_SV_LOBBY_POST_MOVE(char *buffer, int *position_old, int *position_new, int *promotn);

int cl_redirect_clcode_STATE(int code, char *buffer);
int cl_redirect_clcode_REQ(int code, char *buffer);
int cl_redirect_clcode_POST(int code, char *buffer);
int cl_redirect_clcode_LOBBY_REQ(int code, char *buffer, int *position_old, int *position_new, int *promotn);
int cl_redirect_clcode_LOBBY_POST(int code, int *socket, char *buffer, int *position_old, int *position_new, int *promotn);

int cl_POST_LOBBY_MOVE(char *buffer, int *position_old, int *position_new, int *promotn);
int cl_REQ_ASSIGN_LOBBY(char *buffer);

int cl_svcode_redirect(int code, char *buffer, int *position_old, int *position_new, int *promotn);
int cl_clcode_redirect(int code, int *socket, char *buffer, int *position_old, int *position_new, int *promotn);

int cl_GrabPacket(cli_t *client, char *buffer);

#endif

#ifndef SERVER_H
#define SERVER_H

#include "client.h"
#include "net_utils.h"

// server command codes which will be sent to the client/lobby
typedef enum {
    SV_STATE_START = 100,               // STATE START
        SV_STATE_IDLE,                  // post idle ...................................... || "1xx", code
        SV_STATE_CONFIRM,               // post confirm ................................... || "1xx", code
        SV_STATE_DENY,                  // post deny ...................................... || "1xx", code
    SV_STATE_END,                       // STATE END

    SV_REQ_START = 200,                 // REQ START
        SV_REQ_NICKNAME,                // server asking for nickname of client ........... || "2xx", code
    SV_REQ_END,                         // REQ END

    SV_LOBBY_REQ_START,                 // REQ LOBBY_START
        SV_LOBBY_REQ_SYNC,              // server asking confirmation on sync ............. || "2xx", code
    SV_LOBBY_REQ_END,                   // REQ LOBBY_END

    SV_POST_START = 300,                // POST START
    SV_POST_END,                        // POST END

    SV_LOBBY_POST_START,                // POST LOBBY_START
        SV_LOBBY_POST_INIT,             // lobby initialized and ready .................... || "3xx w 8/8/3k4/8/8/3K4/8/8 - - 0 0", code, player_view, fen_board, fen_castle, fen_passant, fen_halfm, fen_fullm
        SV_LOBBY_POST_PARTNER_LEFT,     // the partner left the game ...................... || "3xx", code
		SV_LOBBY_POST_MOVE,				// the partner move a piece ....................... || "3xx 24 43 -1", code, pos_old, pos_new, passant
		SV_LOBBY_POST_MESG,				// messaging chat ................................. || "3xx How long have you been playing here?", code, buf_mesg
        SV_LOBBY_POST_TIME,             // prompting new time for timers to sync .......... || "3xx 2189432", code, time_in_clock
    SV_LOBBY_POST_END                   // POST LOBBY_END

} SERVER_CMD;

int sv_status_STATE(int code);
int sv_status_REQ(int code);
int sv_status_POST(int code);
int sv_status_LOBBY(int code);
int sv_status_LOBBY_REQ(int code);
int sv_status_LOBBY_POST(int code);

int sv_redirect_svcode_STATE(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_svcode_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_svcode_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_svcode_LOBBY_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_svcode_LOBBY_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);

int sv_redirect_clcode_STATE(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_clcode_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_clcode_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_clcode_LOBBY_REQ(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);
int sv_redirect_clcode_LOBBY_POST(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);

int sv_clcode_redirect(int code, net_lobby *lobby, cli_t *client, int room, char *buffer);

int sv_handlePacket(cli_t *client, char *buffer); // server -> handle client packets

#endif

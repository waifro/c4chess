#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "../security/debug.h"
#include "../pp4m/pp4m.h"
#include "../pp4m/pp4m_input.h"
#include "../dashboard/gui.h"
#include "../dashboard/gui_alias.h"
#include "../dashboard/popup.h"
#include "../global.h"

#include "middle.h"
#include "event.h"
#include "core.h"
#include "chess.h"

bool    glo_chess_event_layer[64];
bool    glo_chess_event_king_uatk;
int     glo_chess_event_availmo = -1;
int     glo_chess_event_tile_passant = -1;
int     glo_chess_event_pawn_promotn = -1;
char    glo_chess_event_king_castle[5];

PP4M_HOOK *glo_chess_event_hooklist = NULL;


void EVENT_BlankLayer_Global(void) {
    for (int n = 0; n < 64; n++) {
        glo_chess_event_layer[n] = false;
    }
    return;
}

// instead of searching for eveery tile, search for glo_chess_core_piece[]
void EVENT_BlankLayer_Piece(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {
    for (int n = 0; n < 64; n++) {
        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player) {
            for (int i = 0; i < 64; i++) chess_tile[n].piece->range[i] = false;
        }
    }

    return;
}

void EVENT_UpdateState_ChessEvent(CHESS_CORE_TILE *chess_tile, int position_old, int position_new, CHESS_CORE_PLAYER player) {
    if (MIDDLE_CheckTilePos(position_old, position_new) == -1) return;

    if (CHESS_Redirect_EnumPawn(chess_tile, position_old) == 0) {
        CHESS_CheckState_PawnEnPassant(chess_tile, position_old, position_new, player);
        CHESS_CheckState_PawnPromotion(chess_tile, position_old, position_new, player);
    } else if (CHESS_Redirect_EnumKing(chess_tile, position_old) == 0)
        CHESS_CheckState_KingCastling(chess_tile, position_old, position_new, player);
    else if (CHESS_Redirect_EnumRook(chess_tile, position_old) == 0)
        CHESS_CheckState_RookCastling(position_old, player);

    if (CHESS_Redirect_EnumPawn(chess_tile, position_old) != 0)
        glo_chess_event_tile_passant = -1;

    return;
}

void EVENT_CheckKing_UnderAttack(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    for (int n = 0; n < 64; n++) {

        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player) {
            if (chess_tile[n].piece->enum_piece == KING || chess_tile[n].piece->enum_piece == BKING) {

                if (glo_chess_event_layer[n] == true) glo_chess_event_king_uatk = true;
                else glo_chess_event_king_uatk = false;
                return;
            }
        }
    }

    return;
}

int EVENT_CheckPieceLayer(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {

    static CHESS_CORE_PLAYER pl_bak;

    static bool check_state;
    if (!check_state) {
        pl_bak = CORE_ReversePlayer_State(player);
        check_state = true;
    }

    if (pl_bak != player) {

        EVENT_BlankLayer_Global();

        for (int n = 0; n < 64; n++)
        {
            // piece range copy
            if (chess_tile[n].piece != NULL && chess_tile[n].piece->player != player) {

                for (int i = 0; i < 64; i++) {

                    if (chess_tile[n].piece->range[i] == true) {
                        glo_chess_event_layer[i] = true;
                    }

                    // better to create a visible layer
                    //printf("EVENT_CheckPieceLayer: piece[%c%d] range[%c%d] = piece[%d] layer[%d]\n", chess_tile[n].tag.col, chess_tile[n].tag.row, chess_tile[i].tag.col, chess_tile[i].tag.row, chess_tile[n].piece->range[i], glo_chess_event_layer[i]);
                }
            }
        }

        pl_bak = player;
        EVENT_CheckKing_UnderAttack(chess_tile, player);
    }

    if (glo_chess_event_king_uatk == true) return 2;
    return 0;
}

int EVENT_CheckPlayerCastling(char *fen_castle, CHESS_CORE_PLAYER player) {
    if (strcmp(fen_castle, "-") == 0) return -1;

    int result = 0;
    for (int i = 0; i < (int)strlen(fen_castle); i++) {
        if (player == WHITE_PLAYER) {
            if (fen_castle[i] == 'K' || fen_castle[i] == 'Q') result++;
        }

        else if (player == BLACK_PLAYER) {
            if (fen_castle[i] == 'k' || fen_castle[i] == 'q') result++;
        }
    }

    if (result != 0) return 1;
    else return -1;
}

int EVENT_CheckPlayerCastling_Rook(int tile, CHESS_CORE_PLAYER player) {
    int result = -1;

    CHESS_CORE_TILE_TAG tag = MIDDLE_TileToTag(tile);
    switch(tag.col) {
        case 'a':
            if (player == WHITE_PLAYER) result = 'Q';
            else result = 'q';
            break;

        case 'h':
            if (player == WHITE_PLAYER) result = 'K';
            else result = 'k';
            break;
    }

    return result;
}

int EVENT_UpdateFenCastling_Rook(char *fen_src, int tile, CHESS_CORE_PLAYER player) {
    if (EVENT_CheckPlayerCastling(fen_src, player) == -1) return -1;
    int result = EVENT_CheckPlayerCastling_Rook(tile, player);

    int i = 0;
    for (i = 0; i < (int)strlen(fen_src); i++) {

        if (fen_src[i] == result) {

            for (int n = 0; n < (int)strlen(fen_src) + 1; n++) {
                if (n <= i) continue;

                fen_src[i++] = fen_src[n];
            }

            break;
        }
    }

    if (i == 0) strcpy(fen_src, "-");
    return 0;
}

int EVENT_UpdateFenCastling_King(char *fen_src, CHESS_CORE_PLAYER player) {
    if (EVENT_CheckPlayerCastling(fen_src, player) == -1) return -1;

    int n = 0;
    for (n = 0; n < (int)strlen(fen_src); n++) {

        if (fen_src[n] != 'K' && fen_src[n] != 'Q') {

            for (int i = 0; i < n; i++)
                fen_src[i] = fen_src[(n + i)];

            fen_src[n] = '\0';

            break;
        }
    }

    if (n == 0) strcpy(fen_src, "-");
    return 0;
}

int EVENT_HandlePopup_Pause(int *running) {

    int result = 0;
    SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);

    while (1) {

        if (*running == -1) break;

        PP4M_HOOK *hook_list_pw = GUI_PopupWindow_Init(440, 180);

        GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, OBJ_BUTTON_RETURN, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 15, 410, 70);
        GUI_PopupWindow_Button(hook_list_pw, OPENSANS_REGULAR, OBJ_BUTTON_EXIT, "Esci dal gioco", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 15, 95, 410, 70);

        result = GUI_PopupWindow_Core(hook_list_pw, txr_snapshot);

        if (result == -1) break;
        if (result == -3) { *running = -1; break; }

        SDL_Texture *txr_snapshot2 = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
        PP4M_HOOK *hook_list_pw_exit = GUI_PopupWindow_Init(400, 165);

        GUI_PopupWindow_Title(hook_list_pw_exit, OPENSANS_REGULAR, "Sei sicuro?", PP4M_WHITE, 32);
        GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, OBJ_BUTTON_RETURN, "Annulla", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 85, 185, 70);
        GUI_PopupWindow_Button(hook_list_pw_exit, OPENSANS_REGULAR, OBJ_BUTTON_EXIT, "Okay", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 205, 85, 185, 70);

        if (GUI_PopupWindow_Core(hook_list_pw_exit, txr_snapshot2) == -2) *running = -1;

        hook_list_pw_exit = NULL;
    }

    return *running;
}

int EVENT_HandleKingState(CHESS_CORE_TILE *chess_tile, CHESS_CORE_PLAYER player) {
    int result = -1;
    glo_chess_event_availmo = 0;

    for (int n = 0; n < 64; n++)
        if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player)
            for (int i = 0; i < 64; i++)
                if (chess_tile[n].piece->range[i] == true)
                    if (chess_tile[i].piece == NULL || chess_tile[i].piece->player != player)
                        glo_chess_event_availmo += 1;

    if (glo_chess_event_availmo == 0)
        for (int n = 0; n < 64; n++)
            if (chess_tile[n].piece != NULL && chess_tile[n].piece->player == player)
                if (CHESS_Redirect_EnumKing(chess_tile, n) == 0) {
                    if (glo_chess_event_layer[n] == true) result = EVENT_HandlePopup_Checkmate(NULL, player);
                    else result = EVENT_HandlePopup_Stalemate(NULL);

                    break;
                }

    return result;
}

int EVENT_HandleKeyboard(SDL_Event *event, char *dest) {
    int result = 0;

    // temporary stationed buffer
    static int ind = 0;
    static char buffer[256];

    result = pp4m_INPUT_SdlKeyboard(event);
    if (result == -1 || result == 0) return result;

    if (result == -3 || ind == 256) // enter key
    {
        strncpy(dest, buffer, strlen(buffer) + 1); // save buffer to dest (temporary)
        memset(buffer, 0x00, 255); ind = 0; result = 1;
    }

    else if (result == -6) // escape key
        EVENT_HandlePopup_Pause(&result);

    else if (isprint(result))
        if (ind < 256) buffer[ind++] = result;

    return result;
}

int EVENT_HandlePopup_Stalemate(char *comment) {
    int result = -1;

    SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
    PP4M_HOOK *hook_list = GUI_PopupWindow_Init(450, 150);

    GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, "Stalemate", PP4M_WHITE, 32);

    // leaving this for now
    if (comment != NULL) GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, comment, PP4M_WHITE, 32);
    GUI_PopupWindow_Button(hook_list, OPENSANS_REGULAR, -1, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 90, 430, 50);

    result = GUI_PopupWindow_Core(hook_list, txr_snapshot);
    return result;
}

int EVENT_HandlePopup_Checkmate(char *comment, CHESS_CORE_PLAYER player) {
    (void)player;

    int result = -1;

    SDL_Texture *txr_snapshot = GUI_Alias_CreateSnapshot(glo_render, glo_screen_w, glo_screen_h);
    PP4M_HOOK *hook_list = GUI_PopupWindow_Init(450, 150);

    GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, "Checkmate", PP4M_WHITE, 32);

    // leaving this for now
    if (comment != NULL) GUI_PopupWindow_Title(hook_list, OPENSANS_REGULAR, comment, PP4M_WHITE, 32);
    GUI_PopupWindow_Button(hook_list, OPENSANS_REGULAR, -1, "Continua", PP4M_WHITE, 24, PP4M_GREY_NORMAL, 10, 90, 430, 50);

    result = GUI_PopupWindow_Core(hook_list, txr_snapshot);
    return result;
}

PP4M_HOOK *EVENT_HookList_Init(void) {

    PP4M_HOOK *hook_list = pp4m_HOOK_Init();
    GUI_RenderWindow_Chat_Init(hook_list);

    return hook_list;
}

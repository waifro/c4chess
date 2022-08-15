#ifndef LANG_H
#define LANG_H

#include "config.h"

typedef enum {
    _LANG_SET_ERROR,
    _LANG_SET_ATTENTION,
    _LANG_SET_THERESWRONG,
    _LANG_SET_RETRY,
    _LANG_SET_YES,
    _LANG_SET_NO,
    _LANG_SET_OKAY,

    _LANG_SET_BACKTOTHEGAME,
    _LANG_PROMPT_INPUT_TEXT,
    
    _LANG_SET_BACK,
    _LANG_SET_EXIT,
    _LANG_SET_CONTINUE,
    _LANG_SET_CANCEL,

    _LANG_SET_PLAY,
    _LANG_SET_INFO,
    _LANG_SET_SETTINGS,
    _LANG_SET_STATISTICS,
    _LANG_SET_COMINGSOON,
    _LANG_SET_CONFIRM,
    _LANG_SET_PRESS,

    _LANG_SET_STYLE,
    _LANG_SET_PERSONALIZE,
    _LANG_SET_INSERT,
    _LANG_SET_BG,
    _LANG_SET_SFX,
    _LANG_SET_MUSIC,

    _LANG_SET_LOCAL,
    _LANG_SET_GLOBAL,
    _LANG_SET_PLAYERS,
    _LANG_SET_LOBBYS,
    _LANG_SET_IP_ADDR,

    _LANG_SET_TOEXIT,
    _LANG_SET_TOENTER,
    _LANG_SET_TOCHAT
} _LANG_SET;

/*
"Error", "Attention", "There is something wrong", "Retry", "Yes", "No", "Okay", "Back to the game",
"Back", "Exit", "Continue", "Cancel",
"Play", "Info", "Settings", "Statistics", "Coming soon", "Are you sure?", "Press",
"style", "personalize", "insert", "background", "sounds", "music", "local", "global", "players",
"lobby found", "address Ip", "to exit", "to enter", "in chat" };
*/

extern char *lang_boot_it;
extern char *lang_boot_en;
extern char *lang_boot_nl;

extern const char *lang_cfg_it[];
extern const char *lang_cfg_en[];
extern const char *lang_cfg_nl[];

CFG_LANG LANG_ConfigLanguage(char *value);
int LANG_SetLanguage(CFG_LANG lang);

#endif // LANG_H

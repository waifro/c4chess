#include <stdio.h>
#include <string.h>

#include "pp4m/pp4m_io.h"

#include "main.h"
#include "core.h"
#include "debug.h"
#include "fs_management.h"

int FS_RecordPieceMovement(int old, int foo) {

    int init_pos;
    char initial[] = "rnbqk";

    int row_pos;
    char alpha[] = "abcdefgh";

    char conv[3];
    sprintf(conv, "%c", alpha[tile[foo].colomn]);

    char result[51];

    switch (tile[old].piece->identifier) {

        case (KNIGHT):

        init_pos = 1;

        break;
        case (BISHOP):

        init_pos = 2;

        break;
        case (ROOK):

        init_pos = 0;

        break;
        case (QUEEN):

        init_pos = 3;

        break;
        case (KING):

        init_pos = 4;

        break;
        default:

        init_pos = -1;
        break;
    }

    row_pos = CORE_ReturnRowPosition(tile[old].row);

    // if are not pawns, write the inital
    if (init_pos != -1) sprintf(result, "%c", initial[init_pos]);

    // check if takes another piece
    else if (init_pos == -1 && tile[foo].piece != NULL) sprintf(result, "%c", alpha[old]);

    // x = takes
    if (tile[foo].piece != NULL) strcat(result, "x");

    strcat(result, (char*)&tile[foo].row);

    sprintf(DebugInfo[7].text, "%s", result);
    DEBUG_WriteTextureFont(DebugInfo[7].text, 7);

    strcat(result, conv);

    pp4m_IO_Feedback("test.txt", result);

    return 0;
}

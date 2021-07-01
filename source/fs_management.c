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
    sprintf(conv, "%d ", tile[foo].colomn);

    char result[51] = {0};

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

    if (global_whitecastling == LONG) {

      sprintf(result, "0-0-0");

    }

    // if are not pawns, write the inital
    if (init_pos != -1) sprintf(result, "%c", initial[init_pos]);

    // check if takes another piece
    else if (init_pos == -1 && tile[foo].piece != NULL) {
      row_pos = CORE_ReturnRowPosition(tile[old].row);
      sprintf(result, "%c", alpha[row_pos]);
    }

    // x = takes
    if (tile[foo].piece != NULL) strcat(result, "x");

    row_pos = CORE_ReturnRowPosition(tile[foo].row);

    strncat(result, &alpha[row_pos], sizeof(char));
    strcat(result, conv);

    sprintf(DebugInfo[7].text, "move: %s", result);
    DEBUG_WriteTextureFont(DebugInfo[7].text, 7);

    pp4m_IO_Feedback("test.txt", result);

    return 0;
}

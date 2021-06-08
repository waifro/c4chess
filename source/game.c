#include "pp4m/pp4m.h"
#include "pp4m/pp4m_draw.h"

void GAME_Game(void) {

    int ChessBoard_X = (1280 / 2) - 350;
    int ChessBoard_Y = (720 / 2) - 350;

    PP4M_SDL BlackBoard[32];

    for(int n=0; n < 32; n++) {
        // a
        BlockBoard[n].texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_BLACK, *BlackBoard.rect, 100, 100, ChessBoard_X, ChessBoard_Y);
        ChessBoard_X += 100;

        if (n >= 4) {
        // b

        } else if (n >= 8) {
        //

        } else if (n >= 12) {


        } else if (n >= 16) {


        } else if (n >= 20) {


        } else if (n >= 24) {


        } else if (n >= 28) {


        }

    }



    return;
}

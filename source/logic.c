#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "pp4m/pp4m.h"
#include "pp4m/pp4m_draw.h"

#include "main.h"
#include "core.h"
#include "debug.h"

bool LOGIC_ControlGameplayChessboard(int pos) {

    static int foo = -1;
    static bool result = false;

    if (pos != -1 && tile[pos].piece != NULL) {

            // if new position and "foo" is unused, change tile color, and print possible pattern
            if (pos != foo && foo == -1 && tile[pos].piece->player == global_player) {

                result = false;
                foo = pos;

                // Selecting the piece from tile.toggle
                tile[pos].toggle = true;

                SDL_DestroyTexture(tile[pos].pp4m.texture);
                tile[pos].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, PP4M_BLUE, &tile[pos].pp4m.rect, tile[pos].pp4m.rect.x, tile[pos].pp4m.rect.y, 50, 50);

                CORE_CheckPieceMovement(pos);
            }

            else if (pos != foo && foo != -1) {

                // if deselected, or valid move is choosen, lets reset the toggles and color
                // look if is touching a valid position for the selected piece to move
                SDL_DestroyTexture(tile[foo].pp4m.texture);
                tile[foo].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, tile[foo].color, &tile[foo].pp4m.rect, tile[foo].pp4m.rect.x, tile[foo].pp4m.rect.y, 50, 50);

                // If touch is in a valid position, move piece ( also, tile[pos].toggle = false)
                if (point[pos].toggle == true) {
                        CORE_UpdateMovementPieceFromPoint(pos);
                        global_player = CORE_SwitchPlayerTurn(global_player);
                }

                result = true;
                foo = -1;
            }

    // checks only if a possible valid move, otherwise it resets everything on empty tile
    } else if (pos != -1 && tile[pos].piece == NULL) {

        result = true;
        if (point[pos].toggle == true) {
                CORE_UpdateMovementPieceFromPoint(pos);
                global_player = CORE_SwitchPlayerTurn(global_player);
        }

        SDL_DestroyTexture(tile[foo].pp4m.texture);
        tile[foo].pp4m.texture = pp4m_DRAW_TextureRect(global_renderer, tile[foo].color, &tile[foo].pp4m.rect, tile[foo].pp4m.rect.x, tile[foo].pp4m.rect.y, 50, 50);
    }

    sprintf(DebugInfo[2].text, "tile: %c%d, tile[%d]", tile[foo].row, tile[foo].colomn, foo);
    DEBUG_WriteTextureFont(DebugInfo[2].text, 2);

    sprintf(DebugInfo[5].text, "reset: %d", result);
    DEBUG_WriteTextureFont(DebugInfo[5].text, 5);

    return result;
}

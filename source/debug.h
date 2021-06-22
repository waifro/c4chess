#ifndef _DEBUG_H
#define _DEBUG_H

#define DEBUG_START_POSITION_X 50
#define DEBUG_START_POSITION_Y 50

extern PP4M_SDL DebugInfo[64];

void DEBUG_WriteTextureFont(char *text, int colomn);
void DEBUG_Print(int colomn);

#endif // _DEBUG_H

#ifndef ENGINE_H
#define ENGINE_H

#include "../pp4m/pp4m.h"

extern PP4M_HOOK *glo_engine_net_buffer;

/* prototypes */

// allocates a buffer under first in - last out
int ENGINE_NET_AllocBuffer(char *buffer);

// compares a buffer into the list and returns the pointer to the list, otherwise NULL on error
void *ENGINE_NET_CmpBuffer(char *buffer);

// remove a buffer from the list (requires the original pointer)
int ENGINE_NET_DeallocBuffer(char *buffer);

#endif
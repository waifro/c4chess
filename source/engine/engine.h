#ifndef ENGINE_H
#define ENGINE_H

#include "../pp4m/pp4m.h"

extern PP4M_HOOK *glo_engine_net_buffer;

/* prototypes */

// defaults to glo_engine_net_buffer if head is NULL
void *ENGINE_NET_CheckHead(PP4M_HOOK *head);

// initializes a linked list
int ENGINE_NET_InitFifo(PP4M_HOOK *head);

// quits the entire linked list
int ENGINE_NET_QuitFifo(PP4M_HOOK *head);

// allocates a buffer under first in - first out (FIFO)
int ENGINE_NET_AllocBuffer(PP4M_HOOK *head, char *buffer);

// deallocates a buffer under first in - first out (FIFO)
int ENGINE_NET_DeallocBuffer(PP4M_HOOK *head);

#endif
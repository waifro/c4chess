#include <stdio.h>

#include "../pp4m/pp4m.h"

#include "engine.h"

PP4M_HOOK *glo_engine_net_buffer;

int ENGINE_NET_AllocBuffer(char *buffer) {
    int result = -1;

    result = pp4m_HOOK_Next(glo_engine_net_buffer, buffer);

    return result;
}

void *ENGINE_NET_CmpBuffer(char *buffer) {

    // grab necessary data
    PP4M_HOOK *curr = glo_engine_net_buffer;
    int size = pp4m_HOOK_Size(glo_engine_net_buffer);

    while((size--) >= 0 && curr->ptr != buffer)
        curr = curr->next;

    if (size < 0) return NULL;

    return curr;
}

int ENGINE_NET_DeallocBuffer(char *buffer) {
    if (buffer == NULL) return -1;

    void *list_ptr = ENGINE_NET_CmpBuffer(buffer);
    if (list_ptr == NULL) return -1;

    // we need to deallocate the buffer from the list with this pointer
    
    return 0;
}
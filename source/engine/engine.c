#include <stdio.h>

#include "../pp4m/pp4m.h"

#include "../c4network/net.h"
#include "../c4network/net_utils.h"

#include "engine.h"

PP4M_HOOK *glo_engine_net_buffer;

void *ENGINE_NET_CheckHead(PP4M_HOOK *head) {
    if (head == NULL) return glo_engine_net_buffer;
    return head;
}

int ENGINE_NET_InitFifo(PP4M_HOOK *head) {
    head = pp4m_HOOK_Init();
    glo_engine_net_buffer = head;
    return 0;
}

int ENGINE_NET_QuitFifo(PP4M_HOOK *head) {
    head = ENGINE_NET_CheckHead(head);

    int size = pp4m_HOOK_Size(head);
    for (int i = 0; i < size; i++)
        pp4m_HOOK_Remove(head);

    glo_engine_net_buffer = NULL;

    return 0;
}

int ENGINE_NET_AllocBuffer(PP4M_HOOK *head, char *buffer) {
    head = ENGINE_NET_CheckHead(head);
    pp4m_HOOK_Next(head, buffer);
    return 0;
}

int ENGINE_NET_DeallocBuffer(PP4M_HOOK *head) {
    head = ENGINE_NET_CheckHead(head);
    pp4m_HOOK_RemoveHead(&head);

    // storing new pointer to global buffer
    glo_engine_net_buffer = head;
    
    return 0;
}

int ENGINE_NET_SendToSocket(PP4M_HOOK *head, int *socket) {
    head = ENGINE_NET_CheckHead(head);
    int result = 0;

    // send buffer
    char *buf_ptr = head->ptr;
    result = NET_SendPacket(socket, buf_ptr, 255);

    // remove the buffer from linked list
    if (result == 1) ENGINE_NET_DeallocBuffer(head);
    
    return result;
}
#include <stdio.h>

#include "../pp4m/pp4m.h"

#include "engine.h"

PP4M_HOOK *glo_engine_net_buffer = NULL;

int ENGINE_NET_CheckHead(PP4M_HOOK **head) {
    if (*head == NULL) *head = glo_engine_net_buffer;
    return 0;
}

int ENGINE_NET_InitFifo(PP4M_HOOK *head) {
    ENGINE_NET_CheckHead(&head);
    head = pp4m_HOOK_Init();
    return 0;
}

int ENGINE_NET_QuitFifo(PP4M_HOOK *head) {
    ENGINE_NET_CheckHead(&head);

    int size = pp4m_HOOK_Size(glo_engine_net_buffer);
    for (int i = 0; i < size; i++)
        pp4m_HOOK_Remove(glo_engine_net_buffer);

    return 0;
}

int ENGINE_NET_AllocBuffer(PP4M_HOOK *head, char *buffer) {
    ENGINE_NET_CheckHead(&head);
    pp4m_HOOK_Next(head, buffer);
    return 0;
}

int ENGINE_NET_DeallocBuffer(PP4M_HOOK *head) {
    ENGINE_NET_CheckHead(&head);
    pp4m_HOOK_RemoveHead(&head);
    return 0;
}
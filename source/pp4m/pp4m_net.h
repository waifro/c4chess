/* Private Project Four Me */

#ifndef _PP4M_NET_H
#define _PP4M_NET_H

typedef enum {
    TCP = 1,
    UDP = 2
} PP4M_NET_IPPROTO;

extern int pp4m_socket; //default socket for pp4m
extern struct sockaddr_in pp4m_server; // server side
extern struct sockaddr_in pp4m_client; // client -> server
extern PP4M_NET_IPPROTO pp4m_protocol; // ip protocol

int pp4m_NET_Init(PP4M_NET_IPPROTO protocol);
int pp4m_NET_Quit(void);

int pp4m_NET_ServerStart(int port);
int pp4m_NET_GetLocalAddress(char *destination);

int pp4m_NET_ConnectServerByAddress(char *address, int port);
int pp4m_NET_ConnectServerHostname(char *hostname, int port);

int pp4m_NET_SendData(char *buffer);
int pp4m_NET_RecvData(char *buffer);

void pp4m_NET_ClientClose(void);
void pp4m_NET_ServerClose(void);

#endif // _PP4M_NET_H

/* 2021 @richardwaifro */

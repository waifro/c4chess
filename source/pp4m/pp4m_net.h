/* Private Project Four Me */

#ifndef _PP4M_NET_H
#define _PP4M_NET_H

#ifdef _WIN32
    #include <winsock2.h> //ws2_32
    #include <windows.h>
    #define socklen_t int
#else // _UNIX
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif

typedef enum {
    TCP = 1,
    UDP = 2
} PP4M_NET_IPPROTO;

extern int pp4m_socket; //default socket for pp4m
extern struct sockaddr_in pp4m_server; // server side
extern struct sockaddr_in pp4m_client; // client -> server
extern PP4M_NET_IPPROTO pp4m_protocol; // ip protocol

int pp4m_NET_RecieveError(void);

int pp4m_NET_Init(PP4M_NET_IPPROTO protocol);
int pp4m_NET_Quit(void);

int pp4m_NET_ServerStart(int port);

int pp4m_NET_GetLocalAddress(int socket, char *destination);
int pp4m_NET_GetLocalHostname(char *destination);

int pp4m_NET_ConnectServerByAddress(char *address, int port);
int pp4m_NETSock_ConnectServerByAddress(int socket, char *address, int port);
int pp4m_NET_ConnectServerByHostname(char *hostname, int port);

int pp4m_NET_SendData(char *buffer);
int pp4m_NET_RecvData(char *buffer);

void pp4m_NET_ClientClose(void);
void pp4m_NET_ServerClose(void);

#endif // _PP4M_NET_H

/* 2022 @waifro */

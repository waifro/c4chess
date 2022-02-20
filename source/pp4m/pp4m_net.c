/* Private Project Four Me */

#ifdef _WIN32
#include <winsock2.h> //ws2_32
#include <windows.h>
#else // _UNIX
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "pp4m_io.h"
#include "pp4m_net.h"

int pp4m_socket;
struct sockaddr_in pp4m_server;
struct sockaddr_in pp4m_client;
PP4M_NET_IPPROTO pp4m_protocol;

int pp4m_NET_Init(PP4M_NET_IPPROTO protocol) {
    int result = 0;

    #ifdef _WIN32
        // initializing windows socket
        WSADATA WsaData;
        result = WSAStartup(MAKEWORD(2,2), &WsaData);
        if (result == -1) {
            int error = errno;
            pp4m_IO_Feedback("feedback.txt", strerror(error));
            return result;
        }
    #endif // _WIN32

    switch(protocol) {
    case TCP:
        pp4m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        break;
    case UDP:
        pp4m_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
        break;
    }

    if (pp4m_socket == -1) {
        int error = errno;
        pp4m_IO_Feedback("feedback.txt", strerror(error));
        result = -1;
    }

    if (result == 0) pp4m_protocol = protocol;

	return result;
}

int pp4m_NET_Quit(void) {
    int result = 0;

     result = close(pp4m_socket);

    #ifdef _WIN32
    result = WSACleanup();
    #endif // _WIN32

    if (result != 0) {
        int error = errno;
        pp4m_IO_Feedback("feedback.txt", strerror(error));
    }

    return result;
}

int pp4m_NET_ServerStart(int port) {
    int result = 0;

    pp4m_server.sin_family = AF_INET;
    pp4m_server.sin_addr.s_addr = INADDR_ANY;
    pp4m_server.sin_port = htons(port);

    result = bind(pp4m_socket, (struct sockaddr*)&pp4m_server, sizeof(pp4m_server));
    if (result == -1) {
        int error = errno;
        pp4m_IO_Feedback("feedback.txt", strerror(error));
    }

    return result;
}

int pp4m_NET_GetLocalAddress(char *destination) {
    int result = 0;

    char buf[256];
    result = gethostname(buf, sizeof(buf));
    if (result == -1) {
        int error = errno;
        pp4m_IO_Feedback("feedback.txt", strerror(error));
    }

    if (result == 0) strcpy(destination, buf);

    return result;
}

int pp4m_NET_ConnectServerHostname(char *hostname, int port) {
    int result = 0;

    struct hostent *host;
    host = gethostbyname(hostname);

    pp4m_client.sin_family = AF_INET;
    pp4m_client.sin_addr = *(struct in_addr*)*host->h_addr_list;
    pp4m_client.sin_port = htons(port);

    result = connect(pp4m_socket, (struct sockaddr*)&pp4m_client, sizeof(pp4m_client));
    if (result == -1) {
        int error = errno;
        pp4m_IO_Feedback("feedback.txt", strerror(error));
    }

    return result;
}

int pp4m_NET_ConnectServerByAddress(char *address, int port) {
    int result = 0;

    pp4m_client.sin_family = AF_INET;
    pp4m_client.sin_addr.s_addr = inet_addr(address);
    pp4m_client.sin_port = htons(port);

    result = connect(pp4m_socket, (struct sockaddr*)&pp4m_client, sizeof(pp4m_client));
    if (result == -1) {
        int error = errno;
        pp4m_IO_Feedback("feedback.txt", strerror(error));
    }

    return result;
}

int pp4m_NET_SendData(char *buffer) {
    int result = 0;

    if (pp4m_protocol == TCP) {
        result = send(pp4m_socket, buffer, sizeof(buffer), 0);
        if (result == -1) {
            int error = errno;
            pp4m_IO_Feedback("feedback.txt", strerror(error));
        }
    }

    else if (pp4m_protocol == UDP) {
        result = sendto(pp4m_socket, buffer, sizeof(buffer), 0, NULL, 0);
        if (result == -1) {
            int error = errno;
            pp4m_IO_Feedback("feedback.txt", strerror(error));
        }
    }

    return result;
}

int pp4m_NET_RecvData(char *buffer) {
    int result = 0;

    if (pp4m_protocol == TCP) {
        result = recv(pp4m_socket, buffer, sizeof(buffer), 0);
        if (result == -1) {
            int error = errno;
            pp4m_IO_Feedback("feedback.txt", strerror(error));
        }
    }

    else if (pp4m_protocol == UDP) {
        result = recvfrom(pp4m_socket, buffer, sizeof(buffer), 0, NULL, NULL);
        if (result == -1) {
            int error = errno;
            pp4m_IO_Feedback("feedback.txt", strerror(error));
        }
    }

    return result;
}

void pp4m_NET_ClientClose(void) {

    memset(&pp4m_socket, 0x00, sizeof(pp4m_socket));
    memset(&pp4m_client, 0x00, sizeof(pp4m_client));
    pp4m_protocol = 0;

    return;
}

void pp4m_NET_ServerClose(void) {

    memset(&pp4m_socket, 0x00, sizeof(pp4m_socket));
    memset(&pp4m_server, 0x00, sizeof(pp4m_server));
    pp4m_protocol = 0;

    return;
}

/* 2022 @waifro */

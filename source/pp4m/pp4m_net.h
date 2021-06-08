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

/*                                         Copyright (c) 2021 @richardwaifro //
#                                                                                                                                                    #
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), #
# to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, #
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:         #
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.                     #
#                                                                                                                                                    #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,                                                                                    #
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                                                                #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.                                                                                              #
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,                                                                        #
# DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                                                     #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                            #
#                                                                                                                                                    #
/#                                         Copyright (c) 2021 @richardwaifro */

#include <stdio.h>  
#include <string>
#include <iostream>
#include "SocketServer.h"

#pragma comment(lib,"ws2_32.lib")  





// server

int main(int argc, char* argv[])
{
    //init WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    // create server
    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }

    // bind
    sockaddr_in sin;
    sin.sin_family = AF_INET; 
    sin.sin_port = htons(3019);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;// any local address
    if (bind(server, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }

    // listen
    if (listen(server,1) == SOCKET_ERROR)
    {
        printf("listen error !");
        return 0;
    }
    SOCKET client = NULL;
    while (true)
    {
        if (client == NULL)
        {
            // 等待一个连接
            // slisten.WiteForConnection();
            sockaddr_in remoteAddr;
            int nAddrlen = sizeof(remoteAddr);

            printf("wite for a connection...\n");
            while ((client = accept(server, (SOCKADDR*)&remoteAddr, &nAddrlen)) == INVALID_SOCKET)
            {
                printf("Connect failed, try agin\n");
            }
            printf("accept a connection：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
        }
        else
        {
            fd_set SocketSet;
            // Set up the socket sets we are interested in (just this one)
            FD_ZERO(&SocketSet);
            FD_SET(client, &SocketSet);

            timeval t = { 0,0 };
            // Check the status of the state
            const int SelectStatus = select(client + 1, &SocketSet, NULL, NULL, &t);
            if (SelectStatus > 0) // readable
            {
                char revData[255];

                //接收数据  
                int ret = recv(client, revData, 255, 0);
                if (ret > 0) // have data
                {
                    revData[ret] = 0x00;
                    printf("Receive ---> %s\n", revData);
                }
                else // ret <=0 , connection break
                {
                    printf("DisConnect <<< ===========\n\n");
                    closesocket(client);
                    client = NULL;
                }
            } // Can read 
        }
    } 
    closesocket(client);

    //while (true)
    //{
    //    //WSAGetLastError(1);
    //    if (slisten.GetClientConnectionState() != ESocketConnectionState::SCS_Connected)
    //    {
    //        break;
    //    }
    //    //接收数据  
    //    int ret = recv(slisten.GetNativeClientSocket(), revData, 255, 0);
    //    if (ret > 0)
    //    {
    //        revData[ret] = 0x00;
    //        printf("Receive ---> %s\n",revData);
    //        if (!strcmp(revData, "quit"))
    //        {
    //            break;
    //        }
    //    }
    //    else
    //    {
    //        slisten.MyGetConnectionState(slisten.GetNativeClientSocket());
    //        
    //        //std::string s;
    //        //std::cin >> s;
    //        ////#define SOCKET_ERROR            (-1)
    //        //int BytesSent = send(slisten.GetNativeClientSocket(), s.data(), s.size(), 0);
    //        //printf("sent bytes : %d\n", BytesSent);
    //        printf("WSAGetLastError : %d\n", WSAGetLastError());
    //        
    //    }
    //}
    ////发送数据  
    //const char* sendData = "你好，TCP客户端！\n";
    //send(slisten.GetNativeClientSocket(), sendData, strlen(sendData), 0);
    //closesocket(slisten.GetNativeClientSocket());

    /*closesocket(slisten);
    WSACleanup();*/
    return 0;
}
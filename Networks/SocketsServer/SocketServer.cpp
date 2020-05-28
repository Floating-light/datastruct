#include "SocketServer.h"
#include <stdio.h>  

SocketServer::SocketServer(unsigned short port):serverSocket(NULL), client(NULL)
{
    if (initServer(port))
    {
        printf("Establish Socket server success!\n");
    }
    else
    {
        printf("Socket server failed!\n");
    }
}

SocketServer::~SocketServer()
{
    closesocket(serverSocket);
    WSACleanup();
}

bool SocketServer::initServer(unsigned short port)
{
    //初始化WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    //创建套接字  
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }

    //绑定IP和端口  
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(serverSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }

    //开始监听  
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        printf("listen error !");
        return 0;
    }
    return true;
}

bool SocketServer::WiteForConnection()
{
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);

    printf("等待连接...\n");
    while ((client = accept(serverSocket, (SOCKADDR*)&remoteAddr, &nAddrlen)) == INVALID_SOCKET)
    {
        printf("Connect failed, try agin\n");
    }
    printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
    return true;
}

ESocketBSDReturn SocketServer::HasState(SOCKET socket, ESocketBSDParam State, timeval* TimePointer)
{
    fd_set SocketSet;

    // Set up the socket sets we are interested in (just this one)
    FD_ZERO(&SocketSet);
    FD_SET(socket, &SocketSet);


    // Check the status of the state
    int SelectStatus = 0;
    switch (State)
    {
    case ESocketBSDParam::CanRead:
        SelectStatus = select(socket + 1, &SocketSet, NULL, NULL, TimePointer);
        //printf("ESocketBSDParam::CanRead--->%d\n", SelectStatus);
        break;

    case ESocketBSDParam::CanWrite:
        SelectStatus = select(socket + 1, NULL, &SocketSet, NULL, TimePointer);
        printf("ESocketBSDParam::CanWrite--->%d\n", SelectStatus);
        break;

    case ESocketBSDParam::HasError:
        SelectStatus = select(socket + 1, NULL, NULL, &SocketSet, TimePointer);
        printf("ESocketBSDParam::HasError--->%d\n", SelectStatus);
        break;
    }
    // if the select returns a positive number, the socket had the state, 0 means didn't have it, and negative is API error condition (not socket's error state)
    return SelectStatus > 0 ? ESocketBSDReturn::Yes :
        SelectStatus == 0 ? ESocketBSDReturn::No :
        ESocketBSDReturn::EncounteredError;
}

ESocketConnectionState SocketServer::GetConnectionState(SOCKET socket)
{
	ESocketConnectionState CurrentState = SCS_ConnectionError;

    timeval t = { 0,1 };
	// look for an existing error
	if (HasState(socket,ESocketBSDParam::HasError,&t) == ESocketBSDReturn::No)
	{
		//if (FPlatformTime::Seconds() - LastActivityTime > 5.0)
		//{
		timeval t = { 0,1 };
		// get the write state
		ESocketBSDReturn WriteState = HasState(socket,ESocketBSDParam::CanWrite, &t);
		ESocketBSDReturn ReadState = HasState(socket,ESocketBSDParam::CanRead, &t);

        //printf("------->Write state: %d, Read state: %d\n", WriteState, ReadState);
		// translate yes or no (error is already set)
		if (WriteState == ESocketBSDReturn::Yes || ReadState == ESocketBSDReturn::Yes)
		{
			CurrentState = SCS_Connected;
			//LastActivityTime = FPlatformTime::Seconds();
		}
		else if (WriteState == ESocketBSDReturn::No && ReadState == ESocketBSDReturn::No)
		{
			CurrentState = SCS_NotConnected;
		}
		//}
		//else
		//{
		//	CurrentState = SCS_Connected;
		//}
	}
    else
    {
        printf("_----->ESocketBSDParam::HasError\n");
    }
	return CurrentState;
}

ESocketConnectionState SocketServer::MyGetConnectionState(SOCKET sock)
{
    // only useful in buf != nullptr
    int BytesSent = send(sock, nullptr, 0, 0);
    printf("return number by send: %d", BytesSent);
    return ESocketConnectionState::SCS_Connected;
}

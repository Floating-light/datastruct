# 判断服务端与客户端的连接状态
UE4中判断连接状态是通过`FSocket::GetConnectionState()`,它通过调用`select`函数判断`socket`的读写及错误状态得出的连接状态。

但是，在windows 平台下，UE 4.23的这个函数并不能正确获取到连接状态。当客户端调用`closesocket`关闭连接后，在服务端调用`GetConnectionState()`仍然返回的`ESocketConnectionState::SCS_Connected`。
```c++
ESocketBSDReturn FSocketBSD::HasState(ESocketBSDParam State, FTimespan WaitTime)
{
#if PLATFORM_HAS_BSD_SOCKET_FEATURE_SELECT
	// convert WaitTime to a timeval
	timeval Time;
	Time.tv_sec = (int32)WaitTime.GetTotalSeconds();
	Time.tv_usec = WaitTime.GetFractionMicro();

	fd_set SocketSet;

	// Set up the socket sets we are interested in (just this one)
	FD_ZERO(&SocketSet);
	FD_SET(Socket, &SocketSet);

	timeval* TimePointer = WaitTime.GetTicks() >= 0 ? &Time : nullptr;

	// Check the status of the state
	int32 SelectStatus = 0;
	switch (State)
	{
	case ESocketBSDParam::CanRead:
		SelectStatus = select(Socket + 1, &SocketSet, NULL, NULL, TimePointer);
		break;

	case ESocketBSDParam::CanWrite:
		SelectStatus = select(Socket + 1, NULL, &SocketSet, NULL, TimePointer);
		break;

	case ESocketBSDParam::HasError:
		SelectStatus = select(Socket + 1, NULL, NULL, &SocketSet, TimePointer);
		break;
	}

	// if the select returns a positive number, the socket had the state, 0 means didn't have it, and negative is API error condition (not socket's error state)
	return SelectStatus > 0 ? ESocketBSDReturn::Yes :
		SelectStatus == 0 ? ESocketBSDReturn::No :
		ESocketBSDReturn::EncounteredError;
#else
	UE_LOG(LogSockets, Fatal, TEXT("This platform doesn't support select(), but FSocketBSD::HasState was not overridden"));
	return ESocketBSDReturn::EncounteredError;
#endif
}
```
其主要的判断依据是通过函数`HasState`调用`select`判断`socket`的可读、可写的状态以及错误信息。其判断的逻辑是:

	当一个`socket`没有出现错误信息，且(可读或可写)时就判定为已连接状态。

但是，这样判断是不对的，`socket`可读可写并不代表它的连接状态就正常，这里，我们需要深刻理解`select()`函数的意义。接下来，探讨用`select()`函数判断连接状态的可能性以及其它方法。

## select()

```c++
int select(
  int           nfds,
  fd_set        *readfds,
  fd_set        *writefds,
  fd_set        *exceptfds,
  const timeval *timeout
);
```
此函数用于确定一个或多个`socket`的状态。对每一个`socket`，调用者可查询它的可读性、可写性及错误状态信息。用结构体 `fd_set` 来表示一组等待检查的`socket`。在调用返回时，这个结构存有满足一定条件的`socket`组的子集，并且`select()`返回满足条件的`socket`的数目(大于0)，若返回值为0则表示超时，-1表示出错。有一组宏可用于对 `fd_set` 的操作（宏的功能有：把套接口放入 `fd_set` 结构、清除 `fd_set` 结构中的某个`socket`、检查个`socket`是否可读可写等）


`maxfdp1`：等于成功创建的`socket`个数，`socket()`返回的整数范围为`0~NUM_SOCKETS-1`，`NUM_SOCKETS`为最多能创建的个数，所以`maxfdp1`一般取值为`socket()`返回的最大值`+1`.

`readset`：（可选）指针，指向一组等待可读性检查的`socket`。如果该`socket`正处于`listen()`状态，则若有连接请求到达，该`socket`便被标识为可读，这样一个`accept()`调用保证可以无阻塞完成。对其它状态而言，可读性意味着有数据供读取，于是`recv()`或`recvfrom()`操作均能无阻塞完成.

`writeset`：（可选）指针，指向一组等待可写性检查的`socket`。如果一个`socket`正在`connect()`连接（非阻塞），可写性意味着连接顺利建立。如果套接口并未处于`connect()`调用中，可写性意味着`send()`和`sendto()`调用将无阻塞完成

`exceptset`：（可选）指针，指向一组等待错误检查的`socket`。

`timeout`：`select(`)最多等待时间，对阻塞操作则为`NULL`.

不需要查看的形参可以设为`NULL`

返回值：
* < 0：`select`执行错误
* \> 0：可读、可写或出错的`socket`数目
* == 0：等待超时，没有可读写或错误的文件 

讲道理，按上面UE的判断方式似乎可以判断连接状态，既然可读或可写,那自然连接应该是正常的，而且`select`的参数的描述也是说的`保证accept, recv, recvXXX, connect, send, sendXXX可以无阻塞地完成`,但是不要忘了一点, 当连接断开时，`recv, send`也是可以无阻塞地完成的（立即返回0 或 -1）, 因此这个时候`select`会判断这个断开的`socket`是可读与可写的！

参考`Winsock2 API`的[文档](https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select#remarks), 其中描述了对于面向连接的`socket`，可读性也可能表示来自`peer`的一个关闭请求,此时当所有之前的可读数据都收到之后，`recv`会立即返回0.

因此，用`select`判断`socket`已断开的充分条件是：

* `socket`可读 且 `recv`返回0.

下面这个例子中，先创建了一个`socket server`, 并监听3019端口， 然后开始循环，先阻塞和客户端建立连接，然后循环调用`select`判断是否有数据，没有就做其它事情，如果有，则有`真的有数据`和`断开`两种情况，有则读取，没有则表明客户端断开了连接，则`server`开始新一轮的`accept`.
```c++
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
            } // Can't read
			else
			{
				// do something eles.
			} 
        }
    } 
    closesocket(server);
	return 0;
}
```
![image](./222.png "上述代码运行结果")

在UE4 提供的`FSocket`接口中没有直接操作`select`的方法，可以将其转换为`FSocketBSD`,调用`GetNativeSocket`获取到原始的`SOCKET`句柄,就可以直接对其进行`select,send,recv`等操作，只需包含`"SocketSubsystemBSDPrivate.h"`,它处理了平台相关的`socket`头文件包含。

### ioctl(ioctlsocket in win API)

在上面判断`socket`可读之后，我们可以用`recv` 来判断是真的可读还是这是一个断开的信号。但这样做有个问题，即使你仅仅只想判断是否断开，你也会在没有断开的情况下真的就把数据读出来。用`ioctlsocket`可以解决这个问题，它可以判断`socket`接收缓冲区中存在的数据的字节数(但是它的功能远不止此):
```c++
int dataSize;
if(ioctlsocket(Socket, FIONREAD, (u_long*)(&dataSize)) == 0)
{
    if(dataSize > 0)
    {
        // 缓冲区有数据
    }
    else
    {
        // 缓冲区没数据, 如果之前用select判断过Socket可读，则此时可以判断连接断开
    }
}
``` 
`FIONREAD`表示调用`ioctlsocket`获取`Socket`的接收缓冲区中的字节数，并将其保存在变量`dataSize`中。这样就可以替换`recv`来判断是否连接断开。

# send()

向一个已经连接的socket发送数据。通过它的返回值也可以判断连接状态。[这应该是最可靠的判断方式](https://stackoverflow.com/questions/283375/detecting-tcp-client-disconnect)，无论客户端怎么掉线的。

```c++
int send(
  SOCKET     s,
  const char *buf,
  int        len,
  int        flags
);
```
向socket s发送buf中的数据, len表示buf中数据长度。

重点在返回值，如果没有错误出现,返回的则是已发送的`bytes`数,它有可能小于`len`参数指定的大小。如果有错误出现，则返回`SOCKET_ERROR`.
```c++
#define SOCKET_ERROR            (-1)
```
因此,使用这一方法可以判断一个连接是否断开。[引用一句话](https://stackoverflow.com/questions/283375/detecting-tcp-client-disconnect):
```
There is also only one reliable way to detect a broken connection: by writing to it. 
```

你可能会想到这样调用`send()`来判断连接状态
```c++
send(socket, nullptr, 0,0);
```
[这种调用是允许的](https://docs.microsoft.com/zh-cn/windows/win32/api/winsock2/nf-winsock2-send#remarks)，通常情况下会返回0.但这并不代表连接状态是OK的。即使连接断开，如果在断开之后没有用`len!=0`的参数调用过`send`使其返回`-1`，而是直接像上面那样调用,它会直接返回0. 也就是说，`len==0`的参数调用`send`无法通过其返回值判断连接是否断开。

仅当通过`len!=0`的参数调用`send`并返回`-1`后，用`send(socket, nullptr, 0,0)`调用才会返回`-1`.

### reference:
* BSD Socket: https://www.cnblogs.com/god-of-death/p/7152387.html

* Select 函数: https://www.cnblogs.com/gangzilife/p/9766292.html

* select : https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select#remarks

* 判断连接状态：https://stackoverflow.com/questions/283375/detecting-tcp-client-disconnect

* wiki: https://en.wikipedia.org/wiki/Berkeley_sockets

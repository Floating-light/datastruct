# Sockets(套接字)

套接字为通信的端点。通过网络通信的每`对`进程需要使用一`对`套接字。

套接字组成: `IP地址 + 端口号`

套接字采用`客户机`——`服务器`架构。

服务器通过监听指定端口，来等待客户请求。

实现特定服务的服务器监听众所周知的端口:
* telent服务器端口,23
* ftp服务器监听端口,21
* web或http服务器,80

所有低于1024的端口都是众所周知的。

### 客户端
发出请求Socket: 客户端IP + 大于1024且唯一的端口号。
例如: 146.86.5.20:1625
所有连接都必须是唯一的，当另一个进程与同一个服务器建立连接时，应该被分配大于1024且不等于1625的端口。

IP 127.0.0.1为特殊IP，回送(loopback),它引用自己，这一机制允许同一主机的客户机和服务器可以通过TCP/IP协议进行通信。

# BSD Sockts API
`伯克利套接字(Berkeley sockets)`定义了一组用于进程间Sockets通信的通用API,sockets的实现依赖于不同的操作系统各有所不同，在实现了BSD Sockets接口的操作系统上，就可以很方便地移植采用BSD Sockets API的应用程序。所有现代操作系统都实现了BSD Sockets。

#### 常用API:
* socket() 创建Socket,分配一些系统资源。
* bind() 通常用于服务端，将一个socket和一个socket地址结构关联，它指定了本地IP地址和端口号,可设定为绑定到本机的任何IP。
* listen() 用在服务端，使得一个被绑定的TCP socket进入监听状态。
* connect() 用在客户端，分配给Socket一个空闲的端口号，对于TCP Socket,还会尝试建立一个新的TCP连接。
* accept() 用在服务端，它接受来自远程客户端的创建TCP连接的请求`(connect)`，并创建一个新的Socket来表示这次连接两端的Sockets。
* send(), recv(), sendto(), and recvfrom() 用于发送和接收数据，标准IO函数`write()`和`read()`也可以使用。
* close() 使系统释放一个socket的资源，在TCP下，连接会终止。
* gethostbyname() and gethostbyaddr() 用于获取host名字和地址。仅在IPv4下可用。
* select() 用于挂起，等待列表中的一个或多个套接字准备好读取或写入或出现错误。
* poll() 检查在socket集合中的一个socket的状态。这个集合可以被测试是否有socket可以被写入或读出或出现错误。
* getsockopt() 获取一个socket的的指定socket 选项的当前值。
* setsockopt() 设置一个socket的指定socket选项的值。

# UE4 Sockets

UE4针对众多不同的操作系统的不同的Socket实现提供了同一的Socket编程接口。由于几乎所有操作系统都实现了BSD socket接口，利用这一点可以方便地统一一些基本的Socket接口。同时，不同的操作系统也提供了一些差异化的操作，这些操作由`ISocketSubsystem`定义。

## class FSocket

这个类定义了类似于BSD socket API 的抽象接口。包括:

    Close(),Bind(),Listen(), Connect(),Accept(),Send(),Recv(),等操作。

此外，还有一些便利操作:
```
WaitForPendingConnection(),HasPendingConnection(),可判断是否有连接请求。
HasPendingData(),查询是否有即将到来的数据可以接收。
RecvMulti(),一次读取多个数据包，
```

还有许多其它操作...

## class FSocketBSD

`FSocketBSD`继承自`FSocket`，它实现了所有`FSocket`的接口，跨平台的操作就是在这里实现的。

首先会通过`PLATFORM_HAS_BSD_SOCKETS`判断当前平台是否支持BSD socket接口，以决定是否定义`FSocketBSD`类。

在`BSDSockets/SocketSubsystemBSDPrivate.h`文件中，根据不同的平台包含不同平台的`BSD socket API`实现头文件。并统一原始Socket对象的类型为`SOCKET`，这一类型在`WinSock`中有定义:
```c++
    typedef UINT_PTR        SOCKET;
```
但在其它平台则没有，比如在`Unix`系统下就直接用的int类型表示socket对象指针。所以在其它情况下还会加上一些类型别名：
```c++
    typedef int32 SOCKET;
    // ... ...
```
它的几个成员变量:
```c++
	/** Holds the BSD socket object. */
	SOCKET Socket;

	/** Last activity time. */
	double LastActivityTime;

	/** Pointer to the subsystem that created it. */
	ISocketSubsystem* SocketSubsystem;
```
其中`SocketSubsystem`表示的是创建这个`FSocketBSD`的子系统，其中定义的是平台特异的操作。

## class ISocketSubsystem

这是抽象平台特异的sockets API的基本接口。其功能有:
* Get(const FName&) 获取指定平台的`ISocketSubsystem`单例。
* 不同平台的初始化操作
* 创建和销毁Sockets。
* Host name 到IP address 的域名解析。包括异步的域名解析。
* 创建恰当的`FInternetAddr`表示。
* CreateRecvMulti()
* TMap\<FString, TSharedPtr\<FInternetAddr\> \> HostNameCache; host Name的缓存

### class FSocketSubsystemBSD

此外，它还派生出`FSocketSubsystemBSD`对应于`FSocketBSD`。它实现了标准的BSD支持的一些socket 子系统的操作。

也是在这里，实现了CreateSocket(), 其中也是调用了平台特异的BSD socket API `socket()`实现的。先通过平台特异的原始`socket()`构造函数创建`SOCKET`，这一步其实将所有支持BSD socket API 的操作系统都考虑了进去，再进一步创建`FSocketBSD`，最后根据`ISocketSubsystem`定义的接口，返回`FSocket`.

## Observe

UE 定义的这一`Socket`模块，大体上分两个主要的继承线路，一个是`FSocket`是表示通信的一端的socket，定义了BSD 中的常规操作，以及一些额外的便利操作`(RecvMulti())`。另一条是`ISocketSubsystem`,这是抽象平台特异的功能的基本接口，同时也负责FSocket对象的创建，域名解析等其它功能。

通常情况下，下一步就可以是各个平台的FSocket 和ISocketSubsystem的派生实现了，但是，考虑到几乎所有操作系统都实现了BSD socket API，所以先加了一层`FSocketBSD`和`FSocketSubsystemBSD`，也是在这里,两者产生了直接引用，`FSocketBSD`保存了一个`ISocketSubsystem`指针。这一层其实已经实现了绝大部分的功能。

再往后就是每个平台的继承和实现，大多数只是`Subsystem`部分的`Create()`,`Init()`等操作，部分的`FSocketBSD`有继承下来改动Multi相关的操作。

`ISocketSubsystem::Get()`获取不同平台的单例`ISocketSubsystem`实例,在其上进一步调用`CreateSocket`创建`FSocket`.

UE4 采用`Module`来管理不同的平台`ISocketSubsystem`的创建和销毁。`FSocketSubsystemWindows`调用`CreateSocketSubsystem`和`DestroySocketSubsystem`来创建和销毁子系统。这两个函数extern 声明在`SocketSubsystem.cpp`中，而对应对每个平台的`Subsystem`类的`cpp`文件中都实现了这两个函数.

因此,Socket模块总的使用流程是：
1. ISocketSubsystem::Get(const FName& SubsystemName)获取一个指定平台Name的子系统。
2. ISocketSubsystem::Get中定义了一个静态结构体变量,它只有一个成员`FSocketSubsystemModule`，在这个静态变量初始化的时候，用`FModuleManager::LoadModuleChecked<FSocketSubsystemModule>("Sockets")`的返回值初始化`FSocketSubsystemModule`成员变量。最后返回GetSocketSubsystem(SubsystemName)的返回值。
3. `FSocketSubsystemModule`在加载时就会初始化一个默认的子系统，通过直接调用CreateSocketSubsystem( *this )构建一个默认子系统,至于调用的是什么平台的实现版本，是由什么决定的?
4. 在获取到了一个`ISocketSubsystem`后，就可以调用`ISocketSubsystem::CreateSocket`创建一个相关平台的`Socket`,这个`Socket`的动态类型可能是`FSocketBSD`，也可能是相关平台定义的子类比如`FSocketBSDIOS`,这又取决于这个子系统的动态类型，最终又取决于调用`ISocketSubsystem`的静态函数`Get`时的参数`SubsystemName`通常应该是此时运行的平台。

Reference:
* UE 4.23 Sockets模块源码.


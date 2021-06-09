# Title

* Introduction

## 1. UNetDrivers
负责管理一系列的UNetConnections, 和它们之间的一些共享数据.正常情况下, 仅有一个NetDriver用于标准的Game数据传输和连接.

Server NetDriver 会维护一个NetConnections列表, 每一个都代表了一个连接到游戏中的玩家, 用于负责Actor数据的复制.
Client NetDriver 仅有一个NetConnections, 表示到Server的连接.

在Server 和Client, NetDriver 负责从网络接收Packets并传给恰当的NetConnection(并在必要时建立新的Connections).

## 2. UNetConnections
表示一个连接到Game(UNetDirvers)的Clients. NetConnect不直接处理数据, 而是把数据转发给Channels, 一个NetConnection有一系列它自己的Channels.
一些 Channel 类型:
* Control Channel 用于处理Connection的状态信息(是否关闭).
* Voice Channel 在Client 和 Server之间发送语言数据
* Actor Channel 每一个从Server复制到Client的Actor 都有一个唯一的Channel.

## 3. Initiating Connections / Handshaking Flow.
几乎所有平台, 引擎都是默认使用UIpNetDriver 和 UIpConnection, 下面的描述也是基于这两个类的这两种实现. 因为不同的NetDriver实现具体的过程有所不同.

Server和Client都有它们自己的NetDriver, 所有Game内的复制传输都从IpNetDriver上面发送或接收, 也包括了建立连接的逻辑, 和重连的逻辑.

握手的过程分散在几个不同的地方: NetDriver, PendingNetGame, World, PacketHandlers 和一些可能的其它地方.这种分离是因为确实有这样的需求: 确定一个incoming 的连接是不是在用 "UE-Protocol" 发送数据, 确定是不是恶意攻击, 是否Client有正确版本的Game.

## 4. Startup and Handshaking
Server 在通过UEngine::LoadMap加载Map时, 都会调用UWorld::Listen, 这些代码负责创建主要的Game NetDriver, 解析设置, 调用UNetDriver::InitListen, 最终这些代码负责弄清楚如何监听Client Connections. 在UIpNetDriver::InitListen()中, 调用了配置的Socket子系统以确定要绑定到的IP / Port.(see ISocketSubsystem::GetLocalBindAddresses and ISocketSubsystem::BindNextPort).一旦Server 处于Listen状态, 就准备好了开始接收Client Connections.

当Client想加入到一个Server时, 首先在UEngine::Browse中建立一个带有Server的IP的UPendingNetGame,`UPendingNetGame::Initialize` 和 `UPendingNetGame::InitNetDriver` 负责初始化设置和NetDriver, Clients会针对这个Server立即创建一个UNetConnection 作为它初始化操作的一部分, 并在这个Connection上向Server发送数据,  发起握手的流程。

在Server和Client上, UNetDriver::TickDispatch 通常负责接收网络数据. 通常, 当收到一个Packet时, 先检查它的地址, 是不是来自于一个已知的Connection, 通过一个TMap<FInternetAddr,UNetConnection>记录.

如果是来自一个已经建立好连接的地址, 把这个Packet传给UNetConnection::ReceivedRawPacket.

如果还没有建立连接, 就当作一个还没有连接的地址, 并开始握手流程.`StatelessConnectionHandlerComponent.cpp`

## 5. UWorld / UPendingNetGame / AGameModeBase Startup and Handshaking

## 6. Resetablishing Lost Connections
在整个游戏过程中, 随时都有可能断线。 

如果是Server发起的Disconnection,  这会通过closing UNetConnection处理, 并通知游戏.此时可以由Game决定是否支持游戏中加入或重新加入.如果支持, 将会完全重启上面的握手流程.

如果Client的连接只是简单地中断了, 而且Server又不介意的话, Engine 通常会自动恢复(会有一些丢包和延迟).

如果Client的IP或端口由于某些原因改变了, 而且Server也不介意的话, 将通过再来一次底层握手, 以恢复连接. 游戏将不会受到影响.`StatlessConnectionHandlerComponent.cpp`

## 7. Data Transmission

Game NetConnections 和 NetDrivers 通常不知道底层使用的通信方法, 这是由子类来决定的(UIpConnection / UIpNetDriver or UWebSocketConnection / UWebSocketNetDriver).

在 NetConnections 和 NetDrivers 这一层面, 是和 Packets 和 Bunches 进行数据传输.
Packets是在Host和Client的NetConnections之间传输的数据块, 由meta data(头和ACK), 和 Bunches组成.

Bunches 是在Host和Client的Channels之间传输的数据块.

当一个Connection收到一个Packet时, 它会被拆解成独立的bunches, 这些bunches会被传给独立的Channels进一步处理. 

一个Packet, 可能没有Bunch, 有一个Bunch, 或者多个bunches.由于单个bunch的size有可能超过单个packet的size上限, UE4 会把它分成partial bunches,并打上标记( PartialInitial, Partial, PartialFinal), 使得单个bunch的大小满足要求.

Example: Client RPC to Server.
* Client makes a call to Server_RPC.
* That request is forwarded (via NetDriver and NetConnection) to the Actor Channel that owns the Actor on which the RPC was called.
* The Actor Channel will serialize the RPC Identifier and parameters into a Bunch. The Bunch will also contain the ID of its Actor Channel.
* The Actor Channel will then request the NetConnection send the Bunch.
* Later, the NetConnection will assemble this (and other) data into a Packet which it will send to the server.
* On the Server, the Packet will be received by the NetDriver.
* The NetDriver will inspect the Address that sent the Packet, and hand the Packet over to the appropriate NetConnection.
* The NetConnection will disassemble the Packet into its Bunches (one by one).
* The NetConnection will use the Channel ID on the bunch to Route the bunch to the corresponding Actor Channel.
* The ActorChannel will them disassemble the bunch, see it contains RPC data, and use the RPC ID and serialized parameters
		to call the appropriate function on the Actor.

## 8. Reliability and Retransmission

UE4 Networking假定了底层网络协议并不保证可靠, 它自己实现了packets 和 bunches的重传.

当NetConnection建立的时候, 它将会为它的packets和bunches生成Sequence Number, 这可以是固定顺序的, 也可以是随机的(此时, Server将会把它发给Client).

Packet number 是针对每个NetConnection的, 每次发送Packet时都会+1, 并发送它自己的Packet number.  相同Packet number 的packet将不会重复传输.

Bunch number 是针对每个Channel的, 针对每个reliable 的bunch 而增加, 并且每个reliable 的 bunch都会包含这个Number.和Packet不同的是, reliable的bunch可能会被重传, 这即意味着将会以相同的 bunch number 重新发送这个bunch.

### --- Detecting Incoming Dropped Packets ---

通过packet number, 可以很容易检测丢包, 用当前接收到的Packet 的 number, 与最近的成功接收的packet number比较.

在好的网络条件下, 所有packet将以它们发送的顺序接收, 即后到的packet 总是比之前的+1.

如果是比大, 表明有些packet missing了.此时将会假定missing的packet被dropped掉.并认为当前packet被成功接收, 并使用这个packet number 进一步处理.

如果差值是负值(或0), 这意味着收到了一些没有按顺序到达的packet, 或者外部Service 在尝试重新发数据(Engine不会重复使用Packet number).这两种情况, engine通常直接忽略这些missing 或invalid的packets.

的确也有办法修复在同一帧无序到达的packet, 在开启这个功能后, 当检测到missing packets (差值大于1)时, 并不立即处理这个packet, 而是把它加到一个队列中.下一次成功收到顺序正确的packet后(差值为1), 查看队头的顺序是否恰当, 是就处理, 不是就继续接收.

当收到当前帧的所有packets, 将flush 这个队列, 处理所有剩下的packets, 任何missing的packet都会被假定dropped.

每一个成功接收的packet都会把它对应的packet number 发回发送者作为acknowledgment.

### --- Detecting Outgoing Dropped Packets ---

和上面提到的一样, 当一个packet被成功接收后, 会发一个ACK, 其中包含了这个成功被接收的Packet number, (按顺序的).

sender会追踪最大的Acked的packet number.当ACKs被处理后, 任何收到的小于最近收到的Ack的将会被忽略, 并被认为是Not Acknowledge.

sender负责处理Acks 和 NAKs并重传任何丢失的数据. 新的数据将会被添加到新的将要发送的packets中, (不会发送已经发送过的packet, 也不会重复使用sequence number).

### --- Resending Missing Data ---

packets 并不包含有用的 game 信息.bunches才包含了对 game 有意义的数据. bunches 可以被标记为 Reliable 或 Unreliable.

Engine 不会尝试在unreliable 的bunches被drop后重发。因此, 如果被标记为unreliable, Game 应该能够在没有这些数据时也能正常work, 或者你自己实现外部重传机制,

对于reliable bunches, engine会在丢失这些数据时尝试重传. 当一个reliable bunch 被发送后, 它将会被添加到一个un-ACKed reliable bunches 列表中.如果收到一个包含了这个bunch 的packet的NAK, Engine将会重传一个这个bunch的Copy.如果这个bunch是一组partial bunch的一部分, 这组bunch都会被重传, 只有当所有partial bunch 都被ACK后, 才会把它从这个列表移除.

和Packets一样, 在每个Channal中, 将会比较reliable 的bunch的bunch number. 如果检测到差值为负, 将直接忽略这个bunch.  如果大于1, 则假定missing 了bunch, 与packet的处理不同(认为之前的都丢了, 接收当前的), 这里将会把这个bunch放到一个queue中, 并暂停这个channel中所有的bunch处理(reliabl or unreliable),直到检测到我们收到了所有missing的bunches, 开启处理过程.

在等待missing bunches 或 queue 不为空 的时候收到的所有bunch都会被添加到queue中, 而不会立即处理.
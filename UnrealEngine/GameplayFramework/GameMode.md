## GameMode and GameState： 

document ： https://docs.unrealengine.com/zh-CN/InteractiveExperiences/Framework/GameMode/index.html

简述即职责分析:  https://zhuanlan.zhihu.com/p/23707588

### 流程分析: 

https://zhuanlan.zhihu.com/p/70045930

https://zhuanlan.zhihu.com/p/70391404

## AGameModeBase

继承关系: AGameMode ---> AGameModelBase ---> AInfor ---> AActor
AGameMode : 基于比赛的多人游戏. 实现了 picking spawn points, match state.
AGameModeBase中有许多可重写的基础功能: 

InitGame() : InitGame 事件在其他脚本之前调用（包括 PreInitializeComponents），由 AGameModeBase 用于初始化参数和生成它的helper类。

它在任何 Actor 运行 PreInitializeComponents 前调用（包括 Game Mode 它自己）。

PreLogin() : 接受或拒绝尝试加入服务器的玩家。如果它将 ErrorMessage 设为一个非空字符串，会导致 Login 函数失败。PreLogin 在 Login 前调用，Login 调用前可能需要大量时间，加入的玩家需要下载游戏内容时尤其如此。

PostLogin() : 成功登录后调用。这是首个在 PlayerController 上安全调用复制函数之处。OnPostLogin 可在蓝图中实现，以添加额外的逻辑。

HandleStartingNewPlayer() : 在 PostLogin 后或seamless travel后调用，可在蓝图中覆盖，修改新玩家身上发生的事件。它将默认创建一个玩家 pawn。

RestartPlayer() : 用于开始生成一个玩家 pawn。如需要指定 Pawn 生成的地点，还可使用 RestartPlayerAtPlayerStart 和 RestartPlayerAtTransform 函数。OnRestartPlayer 可在蓝图中实现，在此函数完成后添加逻辑。~

SpawnDefaultPawnAtTransform() : 这实际生成玩家 Pawn，可在蓝图中覆盖。

Logout() : 玩家离开游戏或被摧毁时调用。可实现 OnLogout 执行蓝图逻辑。

可针对游戏提供的每个比赛格式、任务类型或特殊区域创建 AGameModeBase 的子类。一款游戏可拥有任意数量的 Game Mode，因此也可拥有任意数量的 AGameModeBase 类子类；然而，给定时间上只能使用一个 Game Mode。每次关卡进行游戏实例化时 Game Mode Actor 将通过 UGameEngine::LoadMap() 函数进行实例化。

### Init
GameMode Actor通过UGameEngine::LoadMap() 进行实例化

### Replicate NONONONONO
GameMode不会复制到加入多人游戏的Client. 它只存在于服务器上, Client本地只能看到GameMode类, 不能访问它实际的实例和访问其实例的变量.如果玩家确实要更新和当前GameMode相关的信息, 把这些信息放在GameState上可以很容易同步， 只要GameMode被创建出来就会创建出GameState, 然后会复制给所有Clients.

## AGameMode
AGameModeBase 的子类, 实现了一些额外的功能, 以支持多玩家比赛和遗留(legacy)行为.

AGameMode实现了一个状态机, 追踪比赛状态或通用gameplay流程, 可以使用GetMatchState查询当前状态, 或其它的wrappers : HasMatchStarted, IsMatchInProgress, and HasMatchended。可能的比赛状态: 

* EnteringMap : 初始状态, Actors还没有ticking, World 还没有充分初始化.当一些东西都加载好了之后, 就会过度到下一个状态.

* WaitingToStatrt : 就是下一个状态, 进入这个状态时将会调用HandleMatchIsWaitingToStart.Actors开始ticking, 但是Players还没有被Spawn到World. 如果ReadyToStartMatch返回true 或 StartMatch被调用, 就会过渡到下一个状态.

* InProgress : 这是game的主要部分出现的状态.进入时会调用HandleMatchHasStarted, 然后会调用所有Actor的BeginPlay. 此后正常的gameplay就在进行中了. 当ReadyToEndMatch返回true 或 EndMatch被调用时, 就会过度到下一个状态.

* WaitingPostMatch : 这是倒数第二个状态, 进入时调用HandleMatchHasEnded. Actors还在ticking, 但是新的players已经不能加进来了, 当map开始transfer时就会进入到下一个状态.

* LeavingMap : 正常流程中的最后一个状态, 进入时调用HadnleLeavingMap. 在转移到新的map时会一直维持这个状态, 完了之后又回到EnteringMap.

* Aborted : failure state. 通过调用AbortMatch进入这个状态. 出现错误时设置.

比赛状态将会大多数在InProgress, 因为这个时候BeginPlay已经调用, ticking也开始了. 不同的游戏可以重写这些行为, 构建规则更复杂的多人游戏. 比如在等待其它玩家加入一个多人游戏时允许他在level中自由飞翔.

GameMode设置的优先级(从低到高):
* Project Setting ---> Map & Modes ---> Default Modes
* World Setting
* 运行时的命令行参数 : UE4Editor.exe /Game/Maps/MyMap?game=MyGameMode -game

## Game State
Game State复制让Clients监控游戏状态.GameState通常管理需要所有玩家都知道的信息, 特定于GameMode, 而不是单个玩家. 它用于track整个游戏的属性: 已连接的玩家列表, 团队得分, 任务完成情况, etc..

这并不是track玩家特定的东西(单个玩家得分)的地方,  这些应该在PlayerState. 即GameState track 在gameplay期间与整个游戏相关并且对所有玩家都可见的东西.

### Replicate
存在于Server, 并保持复制和更新到所有链接Client .

* GetServerWorldTimeSeconds : Server版本的UWorld::GetTimeSeconds(), server 和Client都会同步, 是用于replication的可靠的时间. 

* PlayerArray : 是APlayerState对象Array, 需要在游戏中对所有玩家do something时很有用.

* HasBegunPlay : 如果游戏中的Actor已经调用了BeginPlay则返回true.

AGameStateBase 通常在 C++ 或蓝图中扩展，实现用于使游戏中玩家知到当前游戏情况的额外变量和函数。进行的修改通常特定于配对的 Game State 和 Game Mode。Game Mode自身也可将其默认 Game State 类重置为派生自 AGameStateBase 的任意 C++ 类或蓝图。
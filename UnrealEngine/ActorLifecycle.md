# 销毁
由于UObject本身有GC机制, 我们只需要去掉所有对它的引用, 就会自动GC. 

Actor中:
1. Actor::Destory()方法可以进入摧毁流程.里面第一行就调用了EndPlay()；
2. 蓝图中有Actor::K2_DestroyActor(), 且为虚函数, 可以重写.
最终会调用:
UWorld::DestroyActor(ActorToDestory);
里面根据EditorWOrld, GameWorld, 网络, Actor类型(ROLE_None, ROLE_Authority), 特殊Actor(WorldSetting)不摧毁做不同的处理.

Detach所有Children Actor, Detach from anything we were attached to(RootComponent->DetachFromComponent(DetachmentRules)).Actor中是RootComponent的tach关系.
```c++
ThisActor->Destroyed();
// Clean up the actor's components.
ThisActor->UnregisterAllComponents();
// Mark the actor and its direct components as pending kill.
ThisActor->MarkPendingKill();
ThisActor->MarkPackageDirty();
ThisActor->MarkComponentsAsPendingKill();
ThisActor->RegisterAllActorTickFunctions(bRegisterTickFunctions, bIncludeComponents);
```
即将这个Actor标记为了RF_PendingKill.

同时也会触发EndPlay()

EndPlay - 在数个地方调用，保证 Actor 的生命走向终点。在游戏过程中，如包含流关卡的 Actor 被卸载，Destroy 将发射此项和关卡过渡。调用 EndPlay 的全部情形：
对 Destroy 显式调用。
Play in Editor 终结。
关卡过渡（无缝行程或加载地图）。 
包含 Actor 的流关卡被卸载。
Actor 的生命期已过。
应用程序关闭（全部 Actor 被销毁）。

随后在适当的情况下, 会调用UObject的通用销毁流程:

BeginDestroy - 对象可利用此机会释放内存并处理其他多线程资源（即为图像线程代理对象）。与销毁相关的大多数游戏性功能理应在 EndPlay 中更早地被处理。
IsReadyForFinishDestroy - 垃圾回收过程将调用此函数，以确定对象是否可被永久解除分配。返回 false，此函数即可延迟对象的实际销毁，直到下一个垃圾回收过程。
FinishDestroy - 最后对象将被销毁，这是释放内部数据结构的另一个机会。这是内存释放前的最后一次调用。

## UE Doc Example
Link: https://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/QuickStart/index.html

### Health 和 Damage
通常是可复制的， 为了同步, 会设为ReplicatedUsing. Health为Character的成员变量.

Damage, AActor有一个TakeDamage虚函数, 在Pawn有进一步的处理.

* 在其他什么地方对我们的Character调用内置的CauseDamage函数.(Server)
* 调用TakeDamage(), --> SetCurrentHealth --> OnHealthUpdate.(Server)
* CurrentHealth同步到Client时, 所有Client---->Rep_CurrentHealth---> OnHealthUpdate.

其中, OnHealthUpdate() 对Health的改变做出任何反应, 这保证了server 和 client的反应一致, 且只同步一个CurrentHealth变量, 就能把由Health改变造成的所有东西都同步上, 减少了许多不必要的同步.
两个关键函数, SetCurrentHealth(), 和OnHealthUpdate(), 使得随后的维护和扩展更容易.

```
ReplicatedUsing = OnRep_VariableName

包含了Replicated 的功能, 在任何时候变量发生更改的时候, 将其复制到所有Client.ReplicatedUsing同时可以设置一个RepNotify函数, 当一个Client成功收到被复制的数据时，就会执行这个函数.
```

### Shooting the Projectile
为了将Projectile复制到每一个Cilent, 它必须在Server上生成, 但fire的操作的按键输入肯定是在client上, 所以要用RPC.
```c++
/** Server function for spawning projectiles.*/
// 仅有的RPC函数, 在Server上Spawn projectiles, 且为Reliable, 保证调用到, 但保存这样的函数的队列长度有限,会溢出, 将会强制Client断开链接.
// Client ---> call function on server
// 必须十分小心在Client上调用这个函数的频率
UFUNCTION(Server, Reliable)
void HandleFire();
```
在按键触发的fire操作中, 通过定时器对HandleFire的调用频率做限制, 然后调用HandleFire, 在其中做Projectile的生成.
# Overview of Gameplay Ability Prediction

## High Level Goals:
在实现一个GA的层面, Prediction 是明确的. 一个Ability是说"Do X->Y->Z", 并且我们将会自动预测这些能够被预测的部分. 我们希望避免像"If Authority: Do X. Else: Do predictive version of X"这样的逻辑出现在Ability自己中.

从这一个出发点, 并不是所有的cases都被解决了.但是这对于解决Client端的预测已经是一个很扎实的框架了.

当我们说Client端的预测时, 即是在说Client预测Game的模拟状态. 完全是Client端的东西不必去预测. 例如, footsteps(脚步)就完全是Client端的东西, 就绝不会和Prediction系统交互. 但是Clients 施展法术使得法力值从100 变为90时就需要Client端的预测.

当前可以预测的东西
* Ability的激活
* Triggerd Events
* GameplayEffect application
    * Attribute modification (例外: Execution不能被预测, 仅attribute modifiers)
    * GameplayTag modification
* Gameplay Cue events(从一个可预测的GE或他们自己本身?)

* Montages
* Movement (UCharacterMovement实现了)

没有预测的(潜在的可以预测, 但当前没有)
* GE的移除
* GE的周期性效果(dots ticking)

尝试解决的问题:
1. "Can I do this?" Prediction的基础协议
2. "Undo" 当预测失败时, 如何UNDO?
3. "Redo" 如何避免在本地预测了的效果又从Server上复制了一遍下来.
4. "Completeness" 如何确定我们预测了Client端的所有Effect.
5. "Dependencies" 如何管理预测依赖和预测的事件链.
6. "Override" 如何在Server复制下来后覆盖Client端预测的状态?

## Implementation Details

### PredictionKey
在这个系统中最基本的概念就是Prediction key(FPredictionKey). 一个PredictionKey本身只是个Unique ID, 在Client的一个Central place生成. Client 把它的Prediction Key和相关联的Action和side effects 发给Server. Server 可能接受或拒绝(Accept/reject)这个Prediction Key, 也会关联Server端的effect和这个Prediction Key.

PredictionKey 总是从Clicent -> Server, 当从Server -> Clients 仅仅会复制给发给Server这个Prdection的CLinet. 这发生在FPredictionKey::NetSerialize. 当一个从Client发过来的Prediction Key又通过属性复制从Server发回去时, 所有其它的Clinets将会收到一个Invalid(0) Prediction key.

### Ability Activation
Ability 的激活是首先会被预测的行为.无论什么时候Client预测地激活一个Ability, 它都会显式地请求Server并且Server也会显式回应. 一旦一个ABility被预测地激活, Client就会有一个有效的'Prediction Window', 在其中发生预测的Effects, 并且不用显式'请求(asked about)'.(不会显式地问'我能扣法力值吗, 我能把这个Ability置于能却中吗, 这些行为在逻辑上会伴随着激活一个Ability 自动考虑').
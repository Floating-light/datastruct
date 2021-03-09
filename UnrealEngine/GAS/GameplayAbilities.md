# 2. Sample Project 
* 多人第三人称射击
* C++, blueprints, UMG, Replication, Intermediate topics in UE4.
* AbilitySystemComponent on PlayerState for heroes(Player or AI controlled). 
* AbilitySystemComponent on the character for AI controlled minions(小兵).
* No predicting projectiles.

# 4. GAS Concepts
## 4.1 Ability System Component
* UAbilitySystemComponent --> UActorComponent
* 一个Actor 想要use GameplayAbilities, have Attributes, receive GameplayEffects. 必须要有一个ASC.
* OwnerActor 逻辑上拥有ASC的actor(PlayerState), AvatarActor ASC 的物理表示的Actor(Character).也可以是一样的.
* 如果OwnerActor和AvatarActor不同, 两者都需要实现IAbilitySystemInterface.均指向同一个ASC.
### 4.1.1 Replication Mode
Mixed : GE仅会被复制给owning client. GameplayTags 和 GameplayCues 复制给所有client.

### 4.1.2 Setup and Initialization
ASC通常在Owner Actor 的构造函数中创建.

然后，还需要用OwnerActor和AvatarActor初始化ASC, 在Server和Client上都需要. 

如果player controlled characters 为OwnerActor和AvatarActor. 则在Character的contructor中创建ASC.
对于server上的初始化,可在ACharacter::PossessedBy中初始化.
对于Client上的初始化, 可在APlayerController::AcknowledgePossession上初始化.

如果ASC在PlayerState上, 
在Server, 在Character的PossessedBy()中初始化. 
在Client, Pawn's OnRep_PlayerState()函数中, 这确保了PlayerState存在于Client上.
# 4.2 Gameplay Tags

FGameplayTags 是一种以层次结构管理的names, 形式类似于Parrent.Child.Grandchild...,由GameplayTagManager注册.在分类和描述一个Objact的状态时很有用. 例如, 如果一个Character被眩晕, 则可以在眩晕期间给这个Character一个State.Debuff.Stun GameplayTag.

可以通过是否有特定标签, 来代替对booleans 或者enums的判断.

当给一个Tags给object, 通常就是把tag添加到它的ASC上, 这样GAS就可以和这些Tags交互. UAbilitySystemComponent可以实现IgameplayTagAssetInterface, 提供访问自己拥有的Tag的方法.

多个GameplayTags可以存储在ags列表.这通常不是问题. GameplayTagContainers 也可以返回一个TArray&lt;FGameplayTag&gt;用于iteration.FGameplayTagContainer中, 而不是用TArray&lt;FGameplayTag&gt;, 因为GameplayTagContainers对GameplayTags的管理更高效.如果tags是标准的FNames, 它们则可以在GameplayTagContainers中高效地打包在一起, 同时如果在project setting中开启了Fast Replication, 就可以高效地复制这些Tags.Fast Replication 要求server和clients有同样的GameplayT

存储在FGameplayTagCountContainer中的GameplayTags 会有一个TagMap存储了GameplayTag的实例数目.一个FGameplayTagCountContainer可能有GameplayTag在其中, 但它的TagMapCount是0.

GameplayTags必须被定义在DefaultGameplayTag.ini中, UE4 Editor在Project setting 提供了UI 以管理GameplayTags, 可以创建, 重命名, 搜索引用, 删除 GameplayTags.

搜索GameplayTag引用将会在熟悉的Rederence Viewer中显示所有引用了这个GameplayTag的assets. 但是不会显示c++ class.

重命名GameplayTag会创建redirect, 即assets仍然引用原来的GameplayTag, 但它会被重定向到新的GameplayTag. 如果可以, 建议先创建新的GameplayTag, 然后手动更新引用到新的GameplayTag, 再删除原来的GameplayTag, 以避免创建Redirect.

除了Fast Replication, GameplayTag editor 中还有其它选项去进一步优化GameplayTags的复制.

如果GameplayTags是从GameplayEffect添加的, 它就会被复制.在ASC中, 可以添加LooseGameplayTags, 它们不能被复制, 必须手动管理. Sample Project 将State.Dead 作为LooseGameplayTag, owning clients 可以在health为0时立即做出反应. Respawning 手动将TagMapCount设置回0. 用LooseGameplayTags时, 仅需要手动调整TagMapCount的值.通常使用UAbilitySystemComponent::AddLooseGameplayTag() 和 UAbilitySystemComponent::RemoveLooseGameplayTag()调整.

获取对一个GameplayTag的引用:
```c++
FGameplayTag::RequestGameplayTag(FName("Your.GameplayTag.Name"));
```
对GameplayTag更多的操作在GameplayTagManager.h的方法, 通过UGameplayTagManager::Get().访问.GameplayTagManager实际将GameplayTags保存为一些相关的节点, 以快速处理 constant string 操作和比较.

GameplayTags and GameplayTagContainers可以有Meta = （Categories = "GameplayCue"）, 这和通常的意义不同, 这会使得在蓝图中仅显示有"GameplayCue"父节点的GameplayTag. 这在已知这些变量只应该用于GameplayCues时很有用.
此外, FGameplayCueTag封装了一个FGameplayTag, 实现和上述相同的功能.

过滤函数的GameplayTag参数, 可以在UFUNCTION中加 `Meta = (GameplayTagFilter = "GameplayCue")`。GameplayTagContainer未实现此功能, 但可以自己实现. 可以看看 `Engine\Plugins\Editor\GameplayTagsEditor\Source\GameplayTagsEditor\Private\SGameplayTagGraphPin.cpp`中的`SGameplayTagGraphPin::ParseDefaultValueData()`函数是如何调用
`FilterString = UGameplayTagsManager::Get().GetCategoriesMetaFromField(PinStructType);`并且传递`FilterString`给`SGameplayTagGraphPin::GetListContent()`中的`SGameplayTagWidget`. GameplayTagContainer版本的这些函数在`Engine\Plugins\Editor\GameplayTagsEditor\Source\GameplayTagsEditor\Private\SGameplayTagContainerGraphPin.cpp`, 没有检查meta 属性字段。

### 4.2.1 Responding to Changes in Gameplay Tags
当GameplayTags添加或删除时, 可以触发ASC中的委托, EGameplayTagEventType 触发的改变方式(added, removed or any change)
```c++
AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventype::NewOrRemoved).AddUObject(this, &AGDPlayerState::StunTagChanged);

// call back function
virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
```
## 4.3 Attributes 
### 4.3.1 Attribute Definition
通常由FGameplayAttributeData来表示, 就是一个浮点数, 有BaseValue 和 CurrentValue, 方便一些逻辑上的处理.如果一个数值是和某个Acctor相关的, 就可以考虑以一个Attribute表示它. 

一个Attribute被定义成AttributeSet子类的一个成员.通常就是为FGameplayAttributeData类型. AttributeSet负责复制那些被标记为replication的属性.
Tip: 不想要一个属性显示在Editor's Attributes list, 可以用Meta = (HideInDetailsView).

### 4.3.2 BaseValue vs CurrentValue 
FGameplayAttributeData通常将一个属性表示为BaseValue 和 CurrentValue, BaseValue 表示永久的值, 而CurrentValue 表示在 BaseValue 的基础上加上一些临时的修改量.比如一个临时的冲刺.

* BaseValue 和 Maximum value
Attribute的Maximum value 可以改变或者被abilities或Ui 引用, 所以应该作为一个单独的属性对待.对于硬编码的最大最小值, 可以用FAttributeMetaData定义一个数据表设置maximum 和minimum values, (work in progress).通常建议, 可以被abilities 或UI 引用的Maximum值作为单独的属性. 硬编码的最大最小值仅用于clamping Attributes，可以在AttributeSet中被定义成floats.

Instant GE 对BaseValue造成永久性的改变.Duration和Infinite GE 改变CurrentValue. Periodic GE 被当作instant GE对待, 改变BaselValue.

### 4.3.3 Meta Attributes
其实还是FGameplayAttributeData, 仅意义上称为Meta Attributes.需要和其它Attributes交互的属性, 比如Damage, GameplayEffect不直接更改health Attribute, 而是用Meta Attribute作为占位, 进一步这个临时属性可以被buffs 或debuffs更改(护盾), 最终从Helth减去它.通常不可复制.

Meta Attributes 在逻辑上将Damage和healing这样的东西分开, 这也意味着GE和Execution Calculations 不需要知道Target如何处理这些Damage.即 GE 决定有多少Damage, AttributeSet决定对这些Damage做什么.

并不是所有的Characters都有同样的Attributes. Base AttributeSet class 可能仅仅有一个health Attribute, 但是子类可能会多一个shield Attribute， 而它对damage 的处理将和base AttributeSet的处理完全不同.

虽然Meta Attributes 很好用, 但也不是强制的. 如果仅有一个Execution Calculation 用于所有damage 实例,  并且所有Characters 共享一个AttributeSet 类, 这样的话直接在Exeuction Calculation应用Damage 到health , shields并直接修改这些Attributes. 

### 4.3.4 Responding to Attribute Changes
当属性值发生改变时, 会调用指定的委托,这可以用来更新UI面板,处理死亡. 通常在拥有ASC的Actor中完成.
```c++
AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGDPlayerState::HealthChanged);
```
实质上是注册到了FActiveGameplayEffectsContainer中的一个TMap:
```c++
TMap<FGameplayAttribute, FOnGameplayAttributeValueChange> AttributeValueChangeDelegates;
```
激活的GE应该在ASC中:
```c++
/** Contains all of the gameplay effects that are currently active on this component */
UPROPERTY(Replicated)
FActiveGameplayEffectsContainer ActiveGameplayEffects;
```
即当有GE改变了一些属性时, 可以通过它所在的Container回调到对应的委托.

也可以自定义蓝图节点, 以在蓝图中实现这个回调. 
### 4.3.5 Derived Attributes





## 4.4 Attribute Set
### 4.4.1 Attribute Set definition
定义、保存、管理Attributes的变化.我们应该从他派生, 并在OwnerActor的构造函数中注册到ASC.
### 4.4.2 Attribute Set Design
一个ASC可以有一个或多个AttributeSets, 其内存消耗很小.

可以定义一个包含所有Attribute的Set, 在使用时忽略掉不用的就是.

也可以定义多个Set, 表示不同类型Attribute的组合, 在ASC中加入需要的即可.

AttributeSets也可子类化, 作为选择使用哪一个Set的另一种方式.

在一个ASC中最多只能有同一个AttributeSets 的一个实例, 有多个也只会用一个.

#### 4.4.2.1 Subcomponents with Individual Attributes

在一个有着多个可伤害组件的Pawn(载具)中,在知道这些组件的(最大)数量时, 就创建多少个Health属性在AttributeSet中,——DamageableCompHealth0,DamageableCompHealth1, etc.代表逻辑slot。在对应的Damageable component实例中,分配slot number Attribute, 以被GameplayAbilities或Executions读取. 

如果每个子组件都有许多Attributes, 而且可能有没有边界的数量的子组件，而且子组件可以被分离给其它玩家.直接用普通的float保存在Components中.

#### 4.4.2.2 Adding and Removing AttributeSets at Runtime
运行时添加或移除AttributeSet有崩溃的风险, 因为服务器客户端同步的原因，需要强制复制。
#### 4.4.2.3 Item Attributes (Weapon Ammo)
有三种方法:
对于可被多个玩家装备的物品的属性, 通常直接用float存在这些物品上。
##### 4.4.2.3.1 Plain Floats on the Item
Fortnite枪的子弹数量就是这样处理的.最大弹夹数,当前在弹夹中的子弹数, 储备子弹等
属性直接以可复制的float存在枪的实例中.

如果武器的储备子弹可以共享,可以把储备子弹放到Character的共享的弹药属性集中.

为避免射击期间把gun复制回客户端,应该在此期间disable复制.

##### 4.4.2.3.2 AttributeSet on the Item
##### 4.4.2.3.3 ASC on the Item
https://epicgames.ent.box.com/s/m1egifkxv3he3u3xezb9hzbgroxyhx89

### Defining Attributes
在派生自UAttributeSet的类的属性中定义, 包括其Getter,Setter 网络复制等都在这里定义.

### 4.4.4 Initializing Attributes
有多种方法初始化. 直接设置BaseValue.

Epic 建议使用一个立即触发的GameplayEffect.

在用了ATTRIBUTE_ACCESSORS定义属性时,也会生成一个InitXXX函数.

### 4.4.5 PreAttributeChange()
通常用于Clamp新的值, 但并不是永久的用于所有Modifier, 后续的更改需要重新Clamp.
```c++
void UMyAttributeSets::ProAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    if(Attribute == GetMoveSpeedAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
    }
}
```
这也不用于触发任何Gameplay事件, 应该用: UAbilitySyatemComponent::GetGameplayAttributeValueChangeDelegate(FGameplayAttribute Attribute)
### 4.4.6 PostGameplayEffectExecute()
在GameplayEffect修改了Attribute 的BaseValue之后调用.用于操纵来自GameplayEffect的属性更改.

在这里减去最终的Health Meta Attribute.如果有盾则减去盾.击中动画.显示伤害数据,分配经验 和 黄金给killer.

### 4.4.7 OnAttributeAggregatorCreated()

## 4.5 Gameplay Effects Move to Separate file

## 4.6 Gameplay Abilities    Move to Separate file

## 4.8 Gamplay Cues
### 4.8.1 Gameplay Cue Definition
* GameplayCues(GC) 执行非gameplay 相关的事, 音效, 粒子, 相机震动等.
* Replicated (unless explicitly Executed, Added, or Removed locally) and predicted
* 触发一个GameplayCues: 发送一个以对应GameplayCue的Name为父Name的GameplayTag 和 通过ASC发送一个event type 给GameplayCueManager.
* GameplayCueNotify objects和其它Actors可以实现IGameplayCueInterface , 以订阅这些事件.基于这些GameplayCue's GameplayCueTag(GameplayCueTag) 订阅.
* Reiterate: GameplayCue 的GameplayTags必须以GameplayCue的父GameplayTag开头.

有两类GameplayCueNotifies--> Static and Actor . 对不同的Events 和不同的GameplayEffects做出反应.

| `GameplayCue` Class                                                                                                                  | Event             | `GameplayEffect` Type    | Description                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| ------------------------------------------------------------------------------------------------------------------------------------ | ----------------- | ------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`GameplayCueNotify_Static`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UGameplayCueNotify_Static/index.html) | `Execute`         | `Instant` or `Periodic`  | Static `GameplayCueNotifies` operate on the `ClassDefaultObject` (meaning no instances) and are perfect for one-off effects like hit impacts.                                                                                                                                                                                                                                                                                                                                                                        |
| [`GameplayCueNotify_Actor`](https://docs.unrealengine.com/en-US/BlueprintAPI/GameplayCueNotify/index.html)                           | `Add` or `Remove` | `Duration` or `Infinite` | Actor `GameplayCueNotifies` spawn a new instance when `Added`. Because these are instanced, they can do actions over time until they are `Removed`. These are good for looping sounds and particle effects that will be removed when the backing `Duration` or `Infinite` `GameplayEffect` is removed or by manually calling remove. These also come with options to manage how many are allowed to be `Added` at the same so that multiple applications of the same effect only start the sounds or particles once. |

从技术上讲GameplayCueNotfies可以对任何Events 做出反应.

Note: 使用GameplayCueNotify_Actor时, 需要检查`Auto Destroy on Remove`, 否则接下来Add这一GameplayCueTag将不会起作用.

 
## 4.10 Prediction
GAS 支持对Client-side的prediction. 但并不能预测所有东西. Client-side的预测是指client不必等到Server允许它，就去激活一个GameplayAbility或应用GE, client 预测server将会允许它做这些, 和预测GE将会应用到一个Target上.在Client激活GA并运行了 网络延迟的时间 后, Server将会告诉client它的prediction是否正确.如果Client错了, 将要回滚它的改变, 以匹配Server端的状态.

GAS Prediction 相关代码在 GameplayPrediction.h。

Epic'的对Predict的想法是只预测 what you "can get away with"。For example : Paragon and Fortnite 不预测Damage. 很有可能他们用的是`ExecutionCalculations`，这样怎么也没办法预测Damage.这并不是说你不能尝试预测像Damage这样确定的东西.只要做出来， 而且能work, 就很好.
```
we are also not all in on a "predict everything: seamlessly and automatically" solution. We still feel player prediction is best kept to a minimum (meaning: predict the minimum amount of stuff you can get away with).
仅预测最少数量的东西.
```
Dave Ratti from Epic's comment from the new Network Prediction Plugin

What is predicted:
* Ability activation
* Triggered Events
* GameplayEffect application:
* Attribute modification (EXCEPTIONS: Executions do not currently predict, only attribute modifiers)
* GameplayTag modification
* Gameplay Cue events (both from within predictive gameplay effect and on their own)
* Montages
* Movement (built into UE4 UCharacterMovement)

What is not predicted:
* GameplayEffect removal
* GameplayEffect periodic effects (dots ticking)

From GameplayPrediction.h


# 6. Debugging GAS

即可视化显示Tags. 两种方式:
* showdebug abilitysystem
* hooks in the GameplayDebugger
编译时, 会优化一些代码, 导致难以Dubg:
* VS Solution 配置为DebugGame Editor
* 在函数的实现上下添加:
```c++
PRAGMA_DISABLE_OPTIMIZATION_ACTUAL
void MyClass::MyFunction(int32 MyIntParameter)
{
	// My code
}
PRAGMA_ENABLE_OPTIMIZATION_ACTUAL
```
### 6.1 文字显示GAS状态信息
```
console command： showdebug abilitysystem
```

翻页: AbilitySystem.Debug.NextCategory

共有三页:
* 显示所有属性的CurrentValue
* 显示所有Duration 和infinite GameplayEffects. number of stacks, GameplayTags, Modifiers.
* grant过的所有GameplayAbilities, 当前running的AbilityTasks状态.
此外， 还可以用PageUp和pageDown.
默认显示本地控制的Character的信息, 用AbilitySystem.Debug.NextTarget和AbilitySystem.Debug.PrevTarget可以显示其他ASC的数据，但并不会更新上半部分的信息.
```
Note: For showdebug abilitysystem to work an actual HUD class must be selected in the GameMode. Otherwise the command is not found and "Unknown Command" is returned.
```
### 6.2 Gameplay Debugger
可以查看一个Actor的GameplayTags, GameplayEffects, 和GameplayAbilities。但不显示Attributes的CurrentValue.

屏幕中心对中一个Actor, 按下‘(单引号).再按数字键盘3, 如果没有，则要 在project setting 中更改keybingdings.
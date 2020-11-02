### 4.3.3 Meta Attributes
需要和其它Attributes交互的属性, 比如Damage, GameplayEffect不直接更改health Attribute, 而是用Meta Attribute作为占位, 进一步这个临时属性可以被buffs 或debuffs更改.

### 4.3.4 Responding to Attribute Changes
当属性值发生改变时, 会调用指定的委托, 

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


## 4.5 Gameplay Effects

### 4.5.1 Gameplay Effect Definition
GameplayEffects(GE)是abilities的容器, abilities通过它来改变Attributes和
GameplayTags(在GE上或其它地方).

通常是一个Data-only class, 定义了一个gameplay effect,没有额外的逻辑在里面.
通常会在蓝图中创建多个UGameplayEffect子类.

GameplayEffects 通过Modifiers 和 Executions(GameplayEffectExecuteion)改变
Attributes.

GameplayEffects 有三种类型: 
Instant 对Attribute立即执行的永久改变, 永远不会加GameplayTags, 一帧也不会.
Duration 临时改变, GE没有时Tags也消失.
Infinite 临时改变, GE不会主动消失, 必须被另一个Ability或ASC主动移除.

后两者可以设置为周期性地应用Periodic Effects(instant)执行Modifiers或Executeions.

GameplayEffects 可以添加/执行 GameplayCues. 

Instant GE 会调用GameplayCues或GameplayTags上的Execute(). Duration 或 
Infinite GE会调用Add 和Remove.

Duration 和 Infinite GE 可以被开启和关闭.

4.5.2 Applying Gameplay Effects
有许多方式, GameplayAbilities 或ASC都有函数可以, 类似于 ApplyGameplayEffectTo.
它们最终都是调用目标Actor的UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf().

在其它任何地方应用一个GE都要先获取到目标Actor的ASC,并使用上述函数.

可以用下面的委托监听Duration或Infinite GE应用到ASC上:
```c++
AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &MyObject::OnActiveGameplayEffectAddedCallback);
virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
```

### 4.5.3 Removing Gameplay Effects
同样GameplayAbilities和ASC也有许多函数可以移除GE, 通常叫 RemoveActiveGameplayEffect.
最终也是调用了目标上的FActiveGameplayEffectsContainer::RemoveActiveEffects().

同样在Target之外移除也需要先获取到Target's 的ASC.

也可以监听Duration 或 Infinite GE的移除:
```c++
AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &APACharacterBase::OnRemoveGameplayEffectCallback);
virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);
```

### 4.5.4 Gameplay Effect Modifiers

Modifiers改变一个Attribute, 也是唯一的以可预测地方式改变属性的方法。
一个GE可以有一个或多个Modifiers, 每个Modifier只通过指定的操作改变一个属性.
包括加减乘除和直接覆盖.

一个Attribute的CurrentValue就是应用了所有Modifiers之后的BaseVlaue的值.
在GameplayEffectAggregator.cpp中的FAggregatorModChannel::EvaluateWithBase:
```
((InlineBaseValue + Additive) * Multiplicitive) / Division
```

有四种类型的Modifiers, 它们表示的操作数的来源:
Scalable Float: 有一个固定系数和数据表配置的等级系数
Attribute Based: 基于属性本身作更改.
Custom Calculation Class: 用一个ModifierMagnitudeCalculation操纵系数.
Set By Caller: 在GE之外运行时设定的值

#### 4.5.4.1 Multiply and Divide Modifiers

在`FAggregatorModChannel`中有一个`TArray<FAggregatorMod> Mods[EGameplayModOp::Max]`数组,
对应着Modifiers的四种操作, 其`EvaluateWithBase`方法根据传入的BaseValue来应用这些
操作, 并返回结果.其中对多个相同的操作先用`SumMods`处理,得到对应的单个操作数,最后用以下公式计算:
```
((InlineBaseValue + Additive) * Multiplicitive) / Division
```
其中值得注意的是对多个相同类型的比如乘和除的`SumMods`的调用类似于:
```
1 + (Mod1.Magnitude - 1) + (Mod2.Magnitude - 1) + ...
```
FAggregatorMod::EvaluatedMagnitude即是这个Modifier的操作数.这样的作法通常和我们想的不一样,
比如两个乘的Modifter的操作数为1.5, 它并不是BaseValue*1.5*1.5, 而是
BaseValue*(1 + (1.5 - 1) + (1.5 - 1)) = BaseValue * 2, 即表示的意义是两次增加0.5倍,
即BaseValue的2倍. 这看上去好像也没有问题，但是:
* 不要超过一个小于一的值, 和任何在1 和2之间的.
* 一个大于2的值.

两个0.5:
1 + (0.5 - 1) + (0.5 - 1) = 0 

1.1 和0.5 ： 
1 + (0.5 - 1) + (1.1 - 1) = 0.6; √

5,5 :
1 + (5 - 1) + (5 - 1) = 9

如果要改变这种规则, 则要修改`EvaluateWithBase`和`SumMods`函数.

#### 4.5.4.2 Gmaeplay Tags on Modifiers
在所属的Effect应用之后才会考虑, 如果是periodic 或infinite effect, 仅仅会在第一次
应用的时候考虑.

通过`UpdateQualifies`, 会通过GE捕获的target ASC 和source ASC获得他们的tags,并与
`FAggregatorEvaluateParameters`参数中的TagFilter比较, 以决定是否起作用.

### 4.5.5 Stacking Gameplay Effects

在GE中有一组属性定义它.

当一个新的GE加入到ASC时, 一般会创建一个新的GameplayEffectSpec, 也可以设置成stack
模式.仅对Duration 和 Infinite有效.

有两种类型, Aggregate by Source and Aggregate by Target.

可以在蓝图写一个自定义节点监听StackChange.

### 4.5.6 Granted Abilities

Duration 和 Infinite GE 可以授予ASCs新的GameplayAbilities.

当你想要另一个玩家被击退, 可以应用一个GE给另一个玩家,并自动激活执行击退动作的ability.
设计者可以选择GE授予什么ability, 及它的一些属性.还可以改输入绑定.

### 4.5.7 Gameplay Effect Tags


GE 中有许多Tags配置, .

GameplayEffectAssetTag: 这个GE本身的Tag,仅仅是为了描述这个GE.

Granted Tags: 会被给这个GE应用的ASC.当Ge移除时, 这些Tags也会从ACS移除.仅对
Duration和InfiniteGE有效.

Ongoing Tag Requirements: 一旦GE被应用,这些Tags决定它在应用时的初始状态是On还是off.

ApplicationTagRequirements: 决定Targets是否能够应用这个GE,如果Target不匹配这组GE,
将不会Apply.

RemoveGameplayEffectswithTags: 在Target的AssetTags或GrantedTags有任何一个这些Tags
都会把这个GE移除。

### 4.5.8 Immunity

GE可以授予免疫,有效地阻止其它GE的应用, 基于GameplayTags.还可以用他达到
ApplicationTagRequirements的效果, 还有一个由于Immunity导致的GE blocked的委托
UAbilitySystemComponent::OnImmunityBlockGameplayEffectDelegate.

GrantedApplicationImmunityTags检查Source ASC有任何指定的Tags,可以实现免疫来自
特定characters的GE.

GrantedApplicationImmunityQuery会检查即将来的GESpec,是否满足这个查询.

### 4.5.9 GameplayEffectSpec

GameplayEffectSpecs可以用UAbilitySystemComponent::MakeOutgoingSpec().直接
用传进来的GE的TSubclassOf的GetDefaultObject()作为GE, 这也暗示了GE是
不能在Runtime下改变的,否则一个地方改了, 其它地方都会受到影响.

通常可以把从一个Ability创建的GE传给Projectile, Projectile在稍后将其应用到击中
的对象上.当GE成功应用时, 会返回一个新的结构体: FActiveGameplayEffect.

#### 4.5.9.1 SetByCallers





## 4.8 Gamplay Cues
### 4.8.1 Gameplay Cue Definition
GameplayCues(GC) 执行非gameplay 相关的事, 音效, 粒子, 相机震动等.
 
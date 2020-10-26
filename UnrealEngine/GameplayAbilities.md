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
对于可被多个玩家装备的物品的属性, 通常直接用float存在这些物品上。
##### 4.4.2.3.1 Plain Floats on the Item
Fortnite枪的子弹数量就是这样处理的.最大弹夹数,当前在弹夹中的子弹数, 储备子弹等
属性直接以可复制的float存在枪的实例中.
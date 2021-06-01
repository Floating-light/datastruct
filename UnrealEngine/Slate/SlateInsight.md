## What SNew
```c++
#define SNew( WidgetType, ... ) \
	MakeTDecl<WidgetType>( #WidgetType, __FILE__, __LINE__, RequiredArgs::MakeRequiredArgs(__VA_ARGS__) ) <<= TYPENAME_OUTSIDE_TEMPLATE WidgetType::FArguments()

// usually
SNew(SVerticalBox::Slot())
+SVerticalBox::Slot()
.bala
.bala
[
    SNew(bala)
] // [] 还是返回的SVerticalBox::FSlot, 但它没有operator+, 所以停, +给Argumnet, 再+下一个
+SVerticalBox::Slot().asdf
```
其中 把SNew展开， 发现， `+` 是针对`WidgetType::FArguments()`, 而这是在:

```c++
SLATE_BEGIN_ARGS( SVerticalBox )
{
	_Visibility = EVisibility::SelfHitTestInvisible;
}
SLATE_SUPPORTS_SLOT(SVerticalBox::FSlot) // 这里定义了operator+

SLATE_END_ARGS()

// -------------SLATE_SUPPORTS_SLOT(SVerticalBox::FSlot)---------------------
TArray< SVerticalBox::FSlot* > Slots; \
WidgetArgsType& operator + (SVerticalBox::FSlot& SlotToAdd) \
{ \
	Slots.Add( &SlotToAdd ); \
	return *this; \
}
```
但是从继承结构上知道，所有`slots`应该是保存在`SBoxPanel`的`TPanelChildren<FSlot> Children`成员中的啊。所以SVerticalBox还有个函数:
```c++
void SVerticalBox::Construct( const SVerticalBox::FArguments& InArgs )
{
	const int32 NumSlots = InArgs.Slots.Num();
	for ( int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex )
	{
		Children.Add( InArgs.Slots[SlotIndex] );
	}
}
```
把`SVerticalBox::FArguments`中的自定义参数写回我们在S类本身定义的属性中。
注意`operator+`返回的是`SVerticalBox::FArguments`本身, 所以我们可以一直`+`下去.
`SLATE_SUPPORTS_SLOT`其作用就是, 定义一个传入类型的`TArray<SlotType*> Slots`, 就可以在创建的时候一直用+添加新的SlotType, 最后需要实现
```c++
void Construct( const FArguments& InArgs );
```
在其中把写在FArguments中的属性写回我们的类中， 这会在SNew 中的<<=操作中, 用`RequiredArgs::MakeRequiredArgs(__VA_ARGS__)`的返回值
```c++
auto _RequiredArgs = RequiredArgs::MakeRequiredArgs(__VA_ARGS__); // 会根据传参情况特化成不同版本
// 最终都会调用它的
_RequiredArgs.CallConstruct(_Widget, InArgs);

// 而这会调用不同可变参数数量的版本, 但Construct的第一个参数都是FArguments
template<class WidgetType>
void CallConstruct(const TSharedRef<WidgetType>& OnWidget, const typename WidgetType::FArguments& WithNamedArgs) const
{
	// YOUR WIDGET MUST IMPLEMENT Construct(const FArguments& InArgs)
	OnWidget->Construct(WithNamedArgs, Forward<Arg0Type>(Arg0), Forward<Arg1Type>(Arg1));
	OnWidget->CacheVolatility();
}
```
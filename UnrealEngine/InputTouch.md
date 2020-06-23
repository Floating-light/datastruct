# Input Touch Move
在蓝图中,有一个`Touch`节点, 它实现了对`Touch Pressed`, `Touch Released`, `Touch Moved`三个事件的绑定.
在C++中, 事件绑定是通过`UInputComponent`, 触摸事件绑定有两种`BindTouch`和`BindGesture`.

其中`BindTouch`接受的函数参数和蓝图中`InputTouch`节点的输出一致.

```c++
/** 
 * Delegate signature for touch handlers. 
 * @FingerIndex: Which finger touched
 * @Location: The 2D screen location that was touched
 */
DECLARE_DELEGATE_TwoParams( FInputTouchHandlerSignature, ETouchIndex::Type, FVector );
DECLARE_DYNAMIC_DELEGATE_TwoParams( FInputTouchHandlerDynamicSignature, ETouchIndex::Type, FingerIndex, FVector, Location );
```

但是`BindTouch`的第一个参数`EInputEvent`中并没有`Move`类型:

```c++
enum EInputEvent
{
	IE_Pressed              =0,
	IE_Released             =1,
	IE_Repeat               =2,
	IE_DoubleClick          =3,
	IE_Axis                 =4,
	IE_MAX                  =5,
};
```



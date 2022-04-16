当我们需要将一些对象搜集起来集中管理时, 通常需要将这些对象放到一个容器中.

1. 最直观的,我们可以include所有这些类,在程序的某个地方主动一个个注册.
```c++
#include "ClassA.h"
#include "ClassB.h"
#include "ClassC.h"

int main()
{
    ClassManager::Get().Register<ClassA>();
    ClassManager::Get().Register<ClassB>();
    [...]
}
```
这种方式体验很不好，需要include一个个类, 并在代码中加上对应的注册的语句.

2. Static Auto Register
```c++
//StaticAutoRegister.h
template<typename TClass>
struct StaticAutoRegister
{
	StaticAutoRegister()
	{
        // 注册到统一的一个地方
		Register(TClass::StaticClass());
	}
};
//MyClass.h
class MyClass
{
    //[...]
};
//MyClass.cpp
#include "MyClass.h"
#include "StaticAutoRegister.h"
const static StaticAutoRegister<MyClass> AutoRegister;
```
这样一来,在`main`函数之前,就会在全局区给全局静态变量分配空间并初始化。初始化调用`StaticAutoRegister`的构造函数,注册`MyClass`.

这种模式只能在独立的地址空间才能有效，如果该文件被静态链接且没有被引用到的话则很可能会绕过static的初始化。不过UE因为都是dll动态链接(Editor下)，且没有出现静态lib再引用Lib(非Editor, 为静态链接, 链接参数上强制引用Lib中的一个符号)，然后又不引用文件的情况出现，所以避免了该问题。或者你也可以找个地方强制的去include一下来触发static初始化。
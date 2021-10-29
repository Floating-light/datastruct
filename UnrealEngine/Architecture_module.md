# Module 

用```IMPLEMENT_MODULE( ModuleImplClass, ModuleName )```,实现模块,`ModuleImplClass`是`IModuleInterface`接口的实现, 其中定义了模块的生命周期函数, 会在相应的时机被`FModuleManager`自动调用.

`IMPLEMENT_MODULE`根据当前是静态链接还是动态链接有不同的实现方式.

## 链接
如果是`IMPLEMENT_MODULE(FDefaultModuleImpl, RHI)`则, 在静态链接下展开为:
```c++
/** Global registrant object for this module when linked statically */ 
static FStaticallyLinkedModuleRegistrant< FDefaultModuleImpl > ModuleRegistrantRHI( TEXT("RHI") ); 
/* Forced reference to this function is added by the linker to check that each module uses IMPLEMENT_MODULE */ 
extern "C" void IMPLEMENT_MODULE_RHI() { } 
PER_MODULE_BOILERPLATE_ANYLINK(FDefaultModuleImpl, ModuleName) // new delete 相关重载.
```

动态链接下: 
```c++
extern "C" DLLEXPORT IModuleInterface* InitializeModule() 
{ 
	return new ModuleImplClass(); 
} 
/* Forced reference to this function is added by the linker to check that each module uses IMPLEMENT_MODULE */ 
extern "C" void IMPLEMENT_MODULE_##ModuleName() { } 
PER_MODULE_BOILERPLATE 
PER_MODULE_BOILERPLATE_ANYLINK(ModuleImplClass, ModuleName)
```

其中, 静态变量`ModuleRegistrantRHI`在初始化的时候会将创建`FDefaultModuleImpl`实例的委托注册到`FModuleManager`单例的一个`Map<Name, Delegate>`中.这个注册将会发生在静态初始化时期. 当我们调用`FModuleManager::LoadModule()`:

1. 首先尝试从已加载(实例化)的的`Modules`中找,找不到则尝试加载.
2. 加载过程同时处理了静态链接还是动态链接的问题. 
3. 对所有`Module`, 先尝试静态链接, 即从注册的`Map`中找到对应的委托并执行, 得到返回的实例, 然后进行初始化.
4. 如果上一步失败(没有找到该模块的初始化委托), 在编译配置为`IS_MONOLITHIC`时直接失败,找不到模块.
5. 如果是动态链接, 则尝试加载动态库DLL, 从相应目录下加载, 调用`Windows API -> LoadLibrary()`. 然后调用DLL导出函数`IModuleInterface* InitializeModule()`创建模块.

<!-- https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok/11336506#11336506 -->

## 静态初始化的问题
如果照着这个思路自己实现一个类似的模块系统, 大概率在静态链接下会出现模块注册器静态变量没有初始化(`ModuleRegistrantRHI`). 因为在链接时, 如果这个静态变量所在的编译单元没有任何符号被外部模块引用, 导致编译器认为这个编译单元是不可达的, 进而将其从结果中消除, 这样最终的结果中就不会含有任何这个编译单元中的符号, 也就不会存在这个静态变量. 一些具体业务层的模块很容易出现这个问题, 它们往往只需要主动向上层模块注册, 而不会有其它模块显式引用自己.

要解决这个问题, 外部必须对这个编译单元的符号有引用.

1. 最直观的方法就是人为创造一个外部模块对这个静态变量所在编译单元的符号的引用,竟然都用静态变量的初始化来注册了, 自然不希望外部对这个静态变量本身有引用, 所以一般直接创建一个空的函数:

```c++
// SomeModule.h
void ForceReferenceFunc();

// SomeModule.cpp
static AutoRegisteredVariable<ModuleClass> RegesterantVar;

void ForceReferenceFunc(){}

```
然后必须在主要的一定会Link到的模块的CPP中Include这个头文件, 并显式调用它：
```c++ 
// main.cpp 
#include "SomeModule.h"

int main()
{
    ForceReferenceFunc();
    return 0;
}
```
但实际上肯定不希望这样解决.

2. 再通用一点, 可以定义一些宏, 自动完成这个过程, 参见[zoolib_xx](https://github.com/zoolib/zoolib_cxx/blob/6e52b8ac85c6636d70f14b4c8123ff67e5dc5837/Core/zoolib/Compat_MSVCStaticLib.h).不用再显式定义强制引用的函数, 用两个宏在对应模块中定义即可, 但还是需要别的地方显式Include这个头文件, 自动利用静态构造引用一个函数, 而不用侵入代码调用这个空的函数.

3. 更干净的解决办法是在链接时加上指定链接参数,强行引用链接的模块中的一个符号.就像UE4的模块定义中： 
```c++
//  extern "C", 表明把它当作c函数编译, 不添加额外的修饰符, 使我们可以直接通过他的字符串名字引用
extern "C" void IMPLEMENT_MODULE_##ModuleName() { } 
```
用在链接到这个模块, 时强行引用这个符号, 这样和它在一个编译单元的静态变量就会得到初始化. 在MSVC下是
```
/include:IMPLEMENT_MODULE_##ModuleName
```
这只要在编译工具上实现即可, 在链接到一个模块时, 添加上这一链接参数.
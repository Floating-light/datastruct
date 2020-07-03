## 目录
* [Display](#标题名,点去掉,空格改为-)
    * [15.7.1 Virtual Destructors](#1571-virtual-destructors)

* QT https://blog.csdn.net/xpg4321/article/details/101278293

# Keyword

### const

1. 修饰变量, 说明其值不可变.
2. 修饰指针, 指向常量的指针`(pointer to const)`和自身是常量的指针`(const pointer)`
3. 修饰引用, reference to const, 通常用于形参类型, 同时避免了拷贝和修改。
4. 修饰成员函数, 该函数不能修改成员变量.
```c++
int a = 0x2468;
int aa = 10;

const int& cc = a;
int& cccc = a; // 引用一旦被初始化就无法修改它指向的对象, 永远也不可能让它指向a之外的变量.
cccc = aa;

const int* p1 = &a;
p1=&aa; 
// *p1 = 3; // 表达式必须是可修改的左值

int *const p2 = &aa;
// p2 = &a; // 表达式必须是可修改的左值
*p2 = aa;

int const*p3 = &a; // <==> const int* 
p3 = &aa;
// *p3 = aa; // 表达式必须是可修改的左值
```
### static
1. 修饰普通变量, 使变量存储在静态区(和全局变量一起),在mian函数之前就分配了空间并初始化。
2. 修饰普通函数, 表明函数作用范围,仅在定义该函数的文件内才能使用。(防止与别的命名空间中的函数重名)
3. 修饰成员变量, 使得所有对象只保存同一个该变量,类不需要初始化就能访问。
4. 修饰成员函数, 同样无需生成对象就能访问,但不能访问非静态成员(变量和函数)。
如果想将函数中此变量的值保存至下一次调用时，如何实现？

控制变量的存储方式和可见性.

https://www.runoob.com/w3cnote/cpp-static-usage.html
https://blog.csdn.net/xueyong4712816/article/details/41120595

因为函数在程序运行中被调用，所以静态数据成员不能在任何函数内分配空间和初始化。

```c++
// file StaticVariable.h
#pragma once
// 全局静态变量,被多次include并不会报重复定义的错
// 因为static限定变量的作用域在当前文件,即包含了这个头文件的地方(编译单元)
static int GlobalItem = 111;
// 如果去掉static则会报重复定义,因为此时它的作用域都是同一个(全局作用域)
// 真正实现全局变量应该用extern
// 类的static成员变量也不能在头文件中定义, 多次include后会重复定义,因为它在类名的作用域下.

// file ClassB.h
#pragma once
class ClassB
{
public:
	static void SetGlobalItem(int i);
	static int GetGlobalItem();
};

// file ClassB.cpp
#include "ClassB.h"
#include "StaticVariable.h"
void ClassB::SetGlobalItem(int i)
{
	GlobalItem = i;
}

int ClassB::GetGlobalItem()
{
	return GlobalItem;
}

// file main.cpp
#include <iostream>
#include "ClassB.h"
#include "StaticVariable.h"

int main()
{
    std::cout << "Global item init value : " << GlobalItem << std::endl;
    std::cout << "Global item init value in classB.cpp : " << ClassB::GetGlobalItem() << std::endl;
    ClassB::SetGlobalItem(-1);
    GlobalItem = -13;
    std::cout << "Global item after set -13 : " << GlobalItem << std::endl;
    std::cout << "Global item in classB.cpp after set -1: " << ClassB::GetGlobalItem() << std::endl;

    ClassB::Address();
    std::cout << "The adress of GlobalItem in main.cpp: " << &GlobalItem << std::endl;
}
```
输出结果
```
Global item init value : 101
Global item init value in classB.cpp : 101
Global item after set -13 : -13
Global item in classB.cpp after set -1: -1
The adress of GlobalItem in ClassB.cpp: 00007FF6A14ED00C
The adress of GlobalItem in main.cpp: 00007FF6A14ED008
```
这说明在不同的include "StaticVariable.h"的.cpp文件中,有着不同的`GlobalItem`,在main函数中直接访问的`GlobalItem`和通过ClassB访问的`ClassB.cpp`中的`GlobalItem`就像是两个作用域中的同名同类型的变量一样,虽然名字和类型都相同,但并不是同一个变量(不同的作用域中可以有相同类型和名称的变量).

### this
1. `this`隐含于非静态成员函数中的特殊指针,指向调用这个成员的对象。
2. `this`的类型隐式为ClassName*const this.所有无法给this赋值。在const成员函数中,为const ClassName*const this.
3. `this`为右值，所以无法取得它的地址。

#### delete this

### pointer and reference
pointer 
1. 是一个变量，一个地址(数字)，本身就占４个byte，这个数字表示的是另一个变量所在的地址。

reference
1. 和原来的变量是同一个东西，占用同一内存单元(对它们取地址得到的结果都一样)。
2. 它必须初始化,且初始化了之后不能改变它指向的变量(也就没有const reference这一说法).行为类似于* const(const pointer). 

### inline

#### inline for virtual
### enum
https://www.runoob.com/w3cnote/cpp-enum-intro.html
1. 枚举变量可以直接输出，但不能直接输入。如：cout >> color3;   //非法
2. 不能直接将常量赋给枚举变量。如：  color1=1; //非法
3. 不同类型的枚举变量之间不能相互赋值。如： color1=color3;  //非法
4. 枚举变量的输入输出一般都采用switch语句将其转换为字符或字符串；枚举类型数据的其他处理也往往应用switch语句，以保证程序的合法性和可读性。

### rvalue reference

### volatile
和多线程相关

### assert()

### sizeof()

### #pragma pack(n)

### bit-field(位域)

### extern "C"

extern 可以置于变量或函数之前。提示编译器到其它编译单元寻找定义。进行链接指定。

* 如果在两个.cpp文件中定义了相同名称和类型的变量,编译不会有问题,但链接的时候会报重复定义,此时该用static限定两者的作用域为本文件。
* 由此我们可以联想到,如果在A.cpp中定义了一个全局变量应该可以直接在Ｂ.cpp中使用？但是别忘了,是在链接之后才发现的重复定义,链接之前的编译阶段,Ｂ.cpp中会报错找不到定义,就到不了链接阶段,发现另一文件中的定义。此时可以用extern声明该变量。
* 所以ｅｘｔｅｒｎ解决的是编译阶段告诉编译器该变量在别的文件定义,请在链接的时候再找它的定义。

－　修饰函数时,加不加它没有特别的意义。

#### extern "C"
用于混用Ｃ和Ｃ＋＋的代码。通常在ｃ＋＋项目中调用ｃ语言实现的库时,包含ｃ语言的库的头文件时应该用extern "C"　｛　#include "clibheader.h"｝包起来。

在C++环境下用Ｃ的函数,常常会出现编译器无法找到ＯＢＪ中的ｃ函数定义,从而导致链接失败。

这是因为Ｃ＋＋在编译的时候为了解决函数的多态问题(函数重载，Ｃ语言不支持)，会将函数名和参数联合起来生成一个中间的函数名称，而Ｃ语言则不会，由此会造成链接时找不到对应函数的情况，此时Ｃ函数就需要用ｅｘｔｅｒｎ　“Ｃ”进行链接指定，这告诉ｃ＋＋编译器，请保持我的名称，不要给我生成用于链接的中间函数名, 因为要链接的符号是ｃ语言编译器按ｃ语言的规则编译出来的。

extern "C"的含义:
1. 首先被它修饰的东西是ｅｘｔｅｒｎ的.
2. 被它修饰的目标是Ｃ的。
```c++
/* c language header: cExample.h */
#ifndef C_EXAMPLE_H
#define C_EXAMPLE_H
extern int add(int x, int y);
#endif
/* c language source file: cExample.c */
#include "cExample.h"
int add(int x, int y)
{
    return x+y;
}
// c++ , call add : cppFile.cpp
extern "C"
{
    #include "cExample.h"
}
int main()
{
    add(2,3);
    return 0;
}
```
### 四种智能指针
https://www.cnblogs.com/WindSun/p/11444429.html
#### auto_ptr
本意是维持对一个指针变量的唯一所有权,在auto_ptr超出作用域之后自动在其析构函数中delete这个指针变量。
（C++98的方案，C++11已经抛弃）采用所有权模式。
```c++
auto_ptr<string> p1 (new string ("I reigned lonely as a cloud.")); 
auto_ptr<string> p2; 
p2 = p1; //auto_ptr不会报错.p1失去了所有权.再尝试通过p1访问指针会直接崩溃.
```
此时不会报错，p2剥夺了p1的所有权，但是当程序运行时访问p1将会报错。所以auto_ptr的缺点是：存在潜在的内存崩溃问题！
#### unique_ptr
严格独占或拥有指针，保证同一时间仅有一个智能指针指向该对象。

相较于`auto_ptr`,它的赋值操作,拷贝构造，都是delete的。但是允许`move`操作。
### shared_ptr
和上面的独占机制不同, `shared_ptr`提供了多个智能指针指向同一对象的管理机制。
### vector

vector<T>本身相当于有四个成员变量: begin, end, capacity, allocator的类.用于表示连续内存空间的起始和容量,以及内存分配器.

https://stackoverflow.com/questions/17299951/c-vector-what-happens-whenever-it-expands-reallocate-on-stack

### struct 和 typedef struct

### struct 和 class 

### union

### explicit

### implement c++ class in c languge

### friend

### using 

### ::

### enum 

### decltype

### reference

### 宏

### initializer_list

### 多态

### vfptr and  vtptr

### 虚继承

### 模板

### 内存分配和管理

### 智能指针

### 类型转换

### 运行时类型信息(RTTI)



1. 对命名空间进行权限管理
2. 类型重命名, 等同于typedef.
3. 继承体系中改变部分接口的继承权限.
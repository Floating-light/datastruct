## 目录
* [Display](#标题名,点去掉,空格改为-)
    * [15.7.1 Virtual Destructors](#1571-virtual-destructors)

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
如果想将函数中此变量的值保存至下一次调用时，如何实现？

控制变量的存储方式和可见性.

https://www.runoob.com/w3cnote/cpp-static-usage.html

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
#### delete this

### inline

#### inline for virtual

### volatile
和多线程相关

### assert()

### sizeof()

### #pragma pack(n)

### bit-field(位域)

### extern "C"

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
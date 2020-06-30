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
1. 修饰普通变量, 使变量存储在静态区(和全局变量一起),在mian函数之前就分配了空间并初始化。
2. 修饰普通函数, 表明函数作用范围,仅在定义该函数的文件内才能使用。(防止与别的命名空间中的函数重名)
3. 修饰成员变量, 使得所有对象只保存同一个该变量,类不需要初始化就能访问。
4. 修饰成员函数, 同样无需生成对象就能访问,但不能访问非静态成员(变量和函数)。
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
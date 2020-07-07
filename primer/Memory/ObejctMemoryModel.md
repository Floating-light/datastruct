# 内存模型
* c++中的类的内存布局是怎么样的?
* 类的虚方法的动态绑定是如何实现的?

Key word: 

### 虚表和虚函数指针

```c++
class Base1
{
public:
    virtual void vfunc1() 
    {
        std::cout << "Base1::vfunc1" << std::endl;
    }
    virtual void vfunc2()
    {
        std::cout << "Base1::vfunc2" << std::endl;
    }
};

class Derived: public Base1
{
public:
    virtual void vfunc1() override
    {
        std::cout << "Derived::vfunc1" << std::endl;
    }
    virtual void vfunc2() override
    {
        std::cout << "Derived::vfunc2" << std::endl;
    }
};

int main()
{
    typedef void (*func)(void);
    Derived der; // &der ---> 虚表函数指针数组的数组指针(第一个数组元素的地址)
    Base1* b1 = &der;

    int* p_vftable = (int*)(&der); // 指向虚表数组的指针

    int* p_first_vf_addr = (int*)*p_vftable; // 虚表数组第一个元素的地址
    int* p_second_vf_addr = p_first_vf_addr + 1; // 虚表数组第二个元素的地址

    int* p_first_func = (int*)*p_first_vf_addr; // 函数指针,虚表数组中的元素
    int* p_second_func = (int*)*p_second_vf_addr;

    std::cout << "Virtual table address: " << p_vftable << std::endl; 
    std::cout << "The pointer to first function's address" << p_first_vf_addr << std::endl; 
    std::cout << "The pointer to second function's address" << p_second_vf_addr << std::endl; 
    std::cout << "The first function pointer: " << p_first_func << std::endl;
    std::cout << "The second function pointer: " << p_second_func << std::endl;
    func f = nullptr;
    f = (func)p_first_func;
    f();
    f = (func)p_second_func;
    f();
}
```
对象`der`的内存空间中最前面的位置保存了指向虚表数组的指针。`&der`即表示了这一指针。
```
virtual table address: 0087FC08
The pointer to first function's address: 0091AC74
The pointer to second function's address: 0091AC78
The first function pointer: 009112D0
The second function pointer: 009112FD
Derived::vfunc1
Derived::vfunc2
```

### 继承-无虚函数重写
1. 虚函数按其声明顺序放于表中.
2. 父类的虚函数在子类的虚函数之前
3. 变量放在虚表后。

### 继承-有虚函数重新
1. 子类重写的虚函数放到了虚表中原来父类实现的虚函数的位置。
2. 没有被覆盖的照旧。

在有多层级继承结构下,每一层级都按照这一规则,后面的覆盖前面的。

### 多重继承-无虚函数重写
1. 每一个父类都有自己的虚表(按声明顺序)
2. 每一个虚表后面是这个父类的成员变量
2. 子类的虚函数成员被放到第一个父类的虚表中(按上述规则)

### 多重继承-有虚函数重写
在继承的多个父类中若有相同的虚函数,则子类在重写这一函数时会同时覆盖所有父类。
1. 子类重写的虚函数会覆盖所有父类虚函数表中的那一虚函数。


对于菱形继承, 
#### Warning
* 可以通过父类型的指针访问子类的虚函数(通过虚表)
* 访问non-public虚函数
    * 非public的虚函数同样存在于虚表中。

reference:

* https://coolshell.cn/articles/12165.html
* https://coolshell.cn/articles/12176.html
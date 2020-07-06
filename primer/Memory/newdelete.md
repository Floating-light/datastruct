# 关于new, delete
* malloc, free, c函数, 不可重载
* new, delete c++表达式,不可重载
* operator new, c++函数,可以重载
* allocator, c++标准库

```c++
class Complex
{
    double i;
    double j;
    public:
    Complex(int r , int v): i(r),j(v) { }
};

int main()
{
    // experssion new, 不能重载
    // new delete 之后的分解动作不能改变
    Complex * pc = new Complex(1,2);
    // 1. void* mem = operator new(sizeof(Complex)); ---> malloc(n)
    // 2. pc = static_cast<Complex*>(mem);
    // 3. pc->Complex::Complext(1,2); // ----> Complex::Complex(pc, 1,2)

    delete pc;
    // 1. Complex::~Complex(pc); // do nothing, complier defalut version，释放user动态开辟的内存
    // 2. operator delete(pc); // free 释放内存
}
```
# 全局operator new， delete重载
```c++
void* myAlloc(size_t size)
{
    return malloc(size);
}
void myFree(void* ptr)
{
    return free(ptr);
}
// 不能用cout, 在程序初始化的时候就会调用new， delete,这时ostream还没初始化好。
inline void* operator new(size_t size)
{
    printf("my global override operator new function\n");
    return myAlloc(size);
}
inline void* operator new[](size_t size)
{
    printf( "my global override operator new[] function\n");
    return myAlloc(size);
}
inline void operator delete(void* ptr)
{
    printf( "my global override operator delete function\n" );
    return myFree(ptr);
}

inline void operator delete[](void* ptr)
{
    printf( "my global override operator delete[] function\n");
    return myFree(ptr);
}


class Complex
{
    double i;
    double j;
public:
    Complex(int r , int v): i(r),j(v) 
    {
        cout << "Complex constructor" << std::endl;
    }

    ~Complex()
    {
        cout << "Complex deconstructor" << std::endl;
    }
};

int main()
{
    // experssion new, 不能重载
    // new delete 之后的分解动作不能改变
    Complex * pc = new Complex(1,2);
    // 1. void* mem = operator new(sizeof(Complex)); ---> malloc(n)
    // 2. pc = static_cast<Complex*>(mem);
    // 3. pc->Complex::Complext(1,2); // ----> Complex::Complex(pc, 1,2)
                                      // new [N] 构造函数会调用N次

    delete pc;
    // 1. Complex::~Complex(pc); // do nothing, complier defalut version，释放user动态开辟的内存，delete[N] 析构函数调用N次
    // 2. operator delete(pc); // free 释放内存

    Complex* p2 = new Complex(2,3);
    delete p2;
}
```

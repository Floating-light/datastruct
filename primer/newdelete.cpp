#include <string>
#include <vector>
#include <iostream>

using namespace std;

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
    // 优先调用(相对于global d )
    void* operator new(size_t size)
    {
        printf("my Complex member override operator new function\n");
        return myAlloc(size);
    }
    void* operator new[](size_t size)
    {
        printf("my Complex member override operator array new function\n");
        return myAlloc(size);
    }
    void operator delete(void* ptr, size_t)//后一参数可选
    {
        printf( "my Complex member override operator delete function\n" );
        return myFree(ptr);
    }
    void operator delete[](void* ptr, size_t)//后一参数可选
    {
        printf( "my Complex member override operator array delete function\n" );
        return myFree(ptr);
    }
};

class Foo
{
public:
    int _id; // 4
    long _data; // 4
    string _str; // 4， pointer。
public:
    Foo(): _id(0) 
    { 
        std::cout << "constructor from Foo" << std::endl;
    }
    Foo(int i ):_id(1) 
    { 
        std::cout << "constructor from Foo with one parameter" << std::endl;
        throw ;
    }
    ~Foo()
    {
        std::cout << "deconstructor from Foo" << std::endl;
    }
        // 优先调用(相对于global d )
    void* operator new(size_t size)
    {
        printf("my Complex member override operator new function\n");
        return myAlloc(size);
    }


    void* operator new[](size_t size)
    {
        printf("my Complex member override operator array new function\n");
        return myAlloc(size);
    }
    void operator delete(void* ptr, size_t)//后一参数可选
    {
        printf( "my Foo member override operator delete function\n" );
        return myFree(ptr);
    }
    void operator delete[](void* ptr, size_t)//后一参数可选
    {
        printf( "my Complex member override operator array delete function\n" );
        return myFree(ptr);
    }

    // placement operator new
    // param one 必须是size_t
    //                 括号内即placement arguments
    // Foo* pf = new(300, 'c')Foo;

    void* operator new(size_t size, size_t extra, char c)
    {
        printf("placement new(size_t extra) : %d", c);
        return myAlloc(size + extra);
    }
    // 也可以重载class member operator delete(), 但他们永远也不会被delete调用
    // 当且仅当new所调用的constructor抛出exception,才会调用这些重载版本的delete(), 用于归还还没完全创建成功的object所占用的内存,胎死腹中
    void operator delete(void* ptr, size_t extra, char c)
    {
        printf( "my Foo placement delete: %d <---> %d \n", extra, c );
        return myFree(ptr);
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

    delete pc;
    // 1. Complex::~Complex(pc); // do nothing, complier defalut version，释放user动态开辟的内存
    // 2. operator delete(pc); // free 释放内存

    // force global
    Complex* p2 = ::new Complex(2,3);
    ::delete p2;

    Foo* f = new(100, 'a')Foo;
    delete f;

    Foo* f2 = new(200, 'b')Foo(12);
    delete f;
}
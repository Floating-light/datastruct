#include <iostream>
#include <string>

using namespace std;

// 普通成员函数指针
class MyTest 
{
    public:
    int TestFunc1(std::string str)
    {
        int ret = 1;
        std::cout << ret << "--------->" << str << std::endl; 
        return ret;
    }

    float TestFunc2(std::string str)
    {
        int ret = 2.0;
        std::cout << ret << "--------->" << str << std::endl;
        return ret;
    }
    static int TestStaticFunc(std::string str)
    {
        int ret = 2.0;
        std::cout << ret << "static--------->" << str << std::endl;
        return ret;
    }
    private:
    int i;
};

int MyFunc(std::string str)
{
    return 1110;
}

int main()
{
    MyTest* MyObj = new MyTest();
    MyTest StackObj;
    using Func1PtrType = int (MyTest::* )(std::string );
    using StaticFunc1Type = int (* )(std::string );
    Func1PtrType Func1Ptr = &MyTest::TestFunc1;
    StaticFunc1Type Func1PtrStatic = &MyTest::TestStaticFunc;
    printf("===>>>%x\n", Func1Ptr);
    printf("===>>>%x\n", &MyTest::TestFunc1);

    printf("===>>>%x\n", &MyTest::TestFunc2);


    std::cout << "Func1 address: " << Func1Ptr << std::endl;
    std::cout << "Func2 address: " << &MyTest::TestFunc2 << std::endl;
    
    (MyObj->*Func1Ptr)("Call func via member function pointer ");
    (StackObj.*Func1Ptr)("Call from stack Object");
    (*Func1PtrStatic)("Call static member");

    Func1PtrStatic = &MyFunc;
    std::cout << "=======>>> "<< (*Func1PtrStatic)("hahah") << std::endl;
}

class Base
{
public:
    virtual ~Base() = default;
    virtual int BaseDerivedFunc(std::string str)
    {
        std::cout << "------------>>> Call BaseClass Version : " << str << std::endl;
        return 3;
    }
};

class Derived : public Base
{
public:
    // 当Derived类没有实现这个函数时Derived::BaseDerivedFunc是BaseType,而不是Derived的成员 ---------->>> 有的编译器会认为两个都可
    // 一旦它实现了， 这就是两个不同的函数指针类型
    // virtual int BaseDerivedFunc(std::string str) override
    // {
    //     return 0;
    // }
};


// int main()
// {
//     using BaseType = int (Base::* ) (std::string);
//     using DerivedType = int (Derived::* ) ( std::string);

//     // Derived 类没有重写
//     BaseType BaseFunc1 = &Base::BaseDerivedFunc;

//     DerivedType DerivedFunc1 = &Base::BaseDerivedFunc;

//     BaseType BaseFunc = &Derived::BaseDerivedFunc;

//     DerivedType DerivedFunc = &Derived::BaseDerivedFunc;

//     Derived d;
//     Base base;
//     (d.*BaseFunc)("Test string");
//     (d.*DerivedFunc)("From DerivedFunc type");

//     (base.*BaseFunc)("Test string");

//     // cannot convert from 'Base *' to 'Derived *'
//     // cannot dereference a 'DerivedType' on a 'Base'
//     // (base.*DerivedFunc)("From DerivedFunc type");

//     // ****************************************************** //
    
// }

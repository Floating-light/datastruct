#include <iostream>
#include <vector>
using namespace std;

// 虚函数表解析: https://coolshell.cn/articles/12165.html
// 对象内存布局: https://coolshell.cn/articles/12176.html
// pointers to members: https://isocpp.org/wiki/faq/pointers-to-members#macro-for-ptr-to-memfn
class Base1
{
public:
    virtual void func1() 
    {
        std::cout << "Base1::func1" << std::endl;
    }
    void func2()
    {
        std::cout << "Base1::func2" << std::endl;
    }
};

class Derived: public Base1
{
public:
    virtual void func1() override
    {
        std::cout << "Derived::func1" << std::endl;
    }
    void gun1() 
    {
        std::cout << "Derived::gun1" << std::endl;
    }
};

int main()
{
    typedef void (*func)(void);
    Derived der; // &der ---> 虚表函数指针数组的数组指针(第一个数组元素的地址)
    Base1* b1 = new Derived;
    func f = nullptr;
    std::cout << "virtual table address: " << (int*)(&der) << std::endl; // 指向虚表数组(数组的第一个元素的地址)的指针
    std::cout << "The first function address of virtual table: " << (int*)*(int*)(&der) << std::endl; // 指向虚表数组的第一个元素的指针(也即是vfptr)
    //f = (func)*((int*)*(int*)(&der));
    f = (func) *((int*)*(int*)(&der)); // 虚表数组的第一个元素(函数指针)
    //f = (func)*((int*)*(int*)(&der));
    f();

    int* pp[10];
    
    
    for(int i = 0; i < 10 ; ++i)
    {
        pp[i] = new int;
        *(pp[i]) = i*10 + 1;
        std::cout << "The value of No: " << i << " ---> "<< pp[i] << " ,. Value: " << *(pp[i])<<  std::endl;
    }
    std::cout << "The address of pp: " << pp << std::endl;
    std::cout << "The value of *pp: " << *pp << std::endl;
    std::cout << "The value of **pp: " << **pp << std::endl;

    std::cout << "The address of pp + 1: " << pp+1 << std::endl;
    std::cout << "The value of *(pp+1): " << *(pp+1) << std::endl;
    std::cout << "The value of **(pp+1): " << **(pp+1) << std::endl;
    std::cin.get();
    
}
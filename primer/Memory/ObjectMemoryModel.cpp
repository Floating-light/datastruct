#include <iostream>
#include <vector>
using namespace std;

// 虚函数表解析: https://coolshell.cn/articles/12165.html
// 对象内存布局: https://coolshell.cn/articles/12176.html
// pointers to members: https://isocpp.org/wiki/faq/pointers-to-members#macro-for-ptr-to-memfn
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
    int base11;
};

class Base2
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
    int base22;

};

class Derived: public Base1, Base2
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
    int base33;
};

int main()
{
    typedef void (*func)(void);
    Derived der; // &der ---> 虚表函数指针数组的数组指针(第一个数组元素的地址)
    Base1* b1 = &der;

    int* p_vftable = (int*)(&der); // 指向虚表pointer 的指针

    int* p_first_vf_addr = (int*)*p_vftable; // 虚表第一个元素的地址
    int* p_second_vf_addr = p_first_vf_addr + 1; // 虚表第二个元素的地址

    std::cout << "last element : " << *(p_first_vf_addr + 3) << std::endl;

    int* p_first_func = (int*)*p_first_vf_addr; // 函数指针,虚表数组中的元素
    int* p_second_func = (int*)*p_second_vf_addr;

    std::cout << "virtual table address: " << p_vftable << std::endl; // 指向虚表数组(数组的第一个元素的地址)的指针
    std::cout << "The pointer to first function's address" << p_first_vf_addr << std::endl; // 指向虚表数组的第一个元素的指针(也即是vfptr)
    std::cout << "The pointer to second function's address" << p_second_vf_addr << std::endl; // 指向虚表数组的第一个元素的指针(也即是vfptr)
    std::cout << "The first function pointer: " << p_first_func << std::endl;
    std::cout << "The second function pointer: " << p_second_func << std::endl;
    func f = nullptr;
    f = (func)p_first_func;
    f();
    f = (func)p_second_func;
    f();
    
    //f = (func)*((int*)*(int*)(&der));
    //f = (func) *((int*)*(int*)(&der)); // 虚表数组的第一个元素(函数指针)
    //f = (func)*((int*)*(int*)(&der));
    //f();

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
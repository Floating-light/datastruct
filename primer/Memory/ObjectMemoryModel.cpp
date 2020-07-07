#include <iostream>
#include <vector>
using namespace std;

// �麯�������: https://coolshell.cn/articles/12165.html
// �����ڴ沼��: https://coolshell.cn/articles/12176.html
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
    Derived der; // &der ---> �����ָ�����������ָ��(��һ������Ԫ�صĵ�ַ)
    Base1* b1 = &der;

    int* p_vftable = (int*)(&der); // ָ�����pointer ��ָ��

    int* p_first_vf_addr = (int*)*p_vftable; // ����һ��Ԫ�صĵ�ַ
    int* p_second_vf_addr = p_first_vf_addr + 1; // ���ڶ���Ԫ�صĵ�ַ

    std::cout << "last element : " << *(p_first_vf_addr + 3) << std::endl;

    int* p_first_func = (int*)*p_first_vf_addr; // ����ָ��,��������е�Ԫ��
    int* p_second_func = (int*)*p_second_vf_addr;

    std::cout << "virtual table address: " << p_vftable << std::endl; // ָ���������(����ĵ�һ��Ԫ�صĵ�ַ)��ָ��
    std::cout << "The pointer to first function's address" << p_first_vf_addr << std::endl; // ָ���������ĵ�һ��Ԫ�ص�ָ��(Ҳ����vfptr)
    std::cout << "The pointer to second function's address" << p_second_vf_addr << std::endl; // ָ���������ĵ�һ��Ԫ�ص�ָ��(Ҳ����vfptr)
    std::cout << "The first function pointer: " << p_first_func << std::endl;
    std::cout << "The second function pointer: " << p_second_func << std::endl;
    func f = nullptr;
    f = (func)p_first_func;
    f();
    f = (func)p_second_func;
    f();
    
    //f = (func)*((int*)*(int*)(&der));
    //f = (func) *((int*)*(int*)(&der)); // �������ĵ�һ��Ԫ��(����ָ��)
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
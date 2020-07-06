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
    char c = 'a';
    int iBase = 10;
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
    int iD = 111;
};

int main()
{
    typedef void (*func)(void);
    Derived der; // &der ---> �����ָ�����������ָ��(��һ������Ԫ�صĵ�ַ)
    Base1* b1 = new Derived;
    func f = nullptr;
    std::cout << "virtual table address: " << (int*)(&der) << std::endl; // ָ���������(����ĵ�һ��Ԫ�صĵ�ַ)��ָ��
    std::cout << "The first function address of virtual table: " << (int*)*(int*)(&der) << std::endl; // ָ���������ĵ�һ��Ԫ�ص�ָ��(Ҳ����vfptr)
    //f = (func)*((int*)*(int*)(&der));
    f = (func) *((int*)*(int*)(&der)); // �������ĵ�һ��Ԫ��(����ָ��)
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
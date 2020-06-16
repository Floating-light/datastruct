#include <iostream>

typedef struct Stag
{
    int a, b, c,d ;
}S;

void func(const int i){}
void func(const int& i){}

int main()
{
    double x = 0;
    double* p = &x;
    double& r = x;

    std::cout << sizeof(x) << std::endl; // 8
    std::cout << sizeof(p) << std::endl; // 4
    std::cout << sizeof(r) << std::endl; // 8 ，假象,
    std::cout << p << std::endl; // 003EFD28
    std::cout << *p << std::endl; // 0
    std::cout << x << std::endl; // 0
    std::cout << r << std::endl; // 0
    std::cout << &x << std::endl; // 003EFD28
    std::cout << &r << std::endl; // 003EFD28 , 假象

    S s;
    S& rs = s;
    std::cout << sizeof(s) << std::endl; // 16
    std::cout << sizeof(rs) << std::endl; // 16
    std::cout << &s << std::endl; // 003EFD38
    std::cout << &rs << std::endl; // 003EFD38

    func(12);//C2668: 'func': ambiguous call to overloaded function
}
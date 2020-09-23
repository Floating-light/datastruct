#include <vector>
#include <iostream>
#include <string>

using namespace std;

template <typename T>
class TypeDisplayer; // 用于利用编译器错误显示推导出的类型

std::vector<std::string> GetStrsFactory() 
{
    return { "Teat1", "test2", "test3"};
}

// c++ 14, allow decltype(auto)
template <typename Container, typename Index>
decltype(auto) authAndAccess(Container& c, Index i)
{
    // balabalah

    return std::forward<Container>(c)[i];
}

// for c++ 11
template < typename Container, typename Index>
auto cpp11AuthAndAccess(Container&& c, Index i) -> decltype(std::forward<Container>(c)[i])
{
    TypeDisplayer<Container> t;
    return std::forward<Container>(c)[i];
}


template < typename T>
void TestLeftValueReference(T& t)
{
    
}
// template < typename T>
// void TestLeftValueReference(T&& t)
// {
    
// }

void Func1(int& i)
{
    std::cout << "Input Integer is : " << i << std::endl;
}

int ReturnIntegerRValue()
{
    return 5;
}

int main()
{
    vector<int> vInt{1,5,8,9,};
    vector<string> vStr{"word","good","best"};

    authAndAccess(vInt, 2);
    authAndAccess(vStr, 1);
    authAndAccess(GetStrsFactory(), 0);
    authAndAccess(std::vector<std::string>(), 0);
    authAndAccess(std::string("dasfg"), 0);

    cpp11AuthAndAccess(std::vector<std::string>(), 0);

    int ii = 0;
    const int& iConst = ii;
    // TestLeftValueReference(std::move(3));
    TestLeftValueReference(iConst);
    
    // Func1(8);
}
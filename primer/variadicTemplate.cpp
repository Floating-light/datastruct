#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;






template <typename T, typename... Args>
void foo(const T& t, const Args&... rest)
{
    std::cout << sizeof...(Args) << std::endl;
    std::cout << sizeof...(rest) << std::endl;
}

// 16.4.1 编写可变参数函数模板
class MyClass
{
    public:
    MyClass(const string& str):data("MyClass: " + str){}
private:
    string data;
 friend ostream& operator<<(ostream& os, const MyClass& my);
};

 ostream& operator<<(ostream& os, const MyClass& my)
 {
     os << my.data << std::endl;
 }

// 用于终止递归, 并打印最后一个元素
// 必须在可变参数版本之前声明
// 若在之后声明, 则无法通过编译, 
// note:   candidate expects at least 2 arguments, 1 provided
template<typename T>
ostream &print(ostream& os, const T &t)
{
    return os << t << std::endl;
}

// 可变参数函数通常是递归的,每一步处理最前面的一个实参, 然后用剩余参数调用自身.
// 除了最后一个元素之外的其他元素都会调用这个版本的print
template <typename T, typename ... Args>
ostream &print(ostream& os, const T& t,const Args&... rest)
// const Args&.. rest
// 表示将const Args& 应用到包中的每一个元素, 其扩展结果为一个逗号分隔的零个或多个类型的列表，
// 每个类型都形如const type&
// print(std::cout, 12, 13.3, "asfdae");
// 实例化为 print(ostream&, const int&, const double&, const string&)
{
    os << t << ", ";
    return print(os, rest...);
    // 模式是函数参数包的名字 rest.
    // 扩展为一个由由包中元素组成的、逗号分隔的列表.
    // print(os, 13.3, "asfdae");
}

// 16.4.2 包扩展
// 对于一个参数包, 除了获取其大小, 我们唯一能做的就是扩展(expand)它
// 即对于包中的每一个元素都应用同样的"模式"，即参数的修饰形式

template <typename T>
string debug_rep(const T& t)
{
    ostringstream ss;
    ss << t;
    return ss.str();
}
template <typename... Args>
ostream &errorMsg(ostream&os, const Args&... rest)
{
    return print(os, debug_rep(rest)...);
// 扩展结果为一逗号分隔的debug_rep的调用列表
// errorMsg(cerr, fcnName, code.num(), otherData, "balabala", item);
// print(cerr, debug_rep(fcnName), debug_rep(code.num(),
//             debug_rep(otherData), debug_rep("balabala"),
//             debug_rep(item)));
}
// 这样的调用则会失败:
// print(os, debug_rep(rest...));
// 展开为将包传递给了debug_rep:print(os, debug_rep(a1,a2,a3...an))

// 16.4.3 转发参数包
// 使用可变参数模板和forward机制实现将实参不变地传递给其他函数。

// 标准库容器的emplace_back(), 使用其实参在容器管理的内存空间中直接构造一个元素
// 由于大多数数据类型都有不止一个构造函数,参数类型也不尽相同，emplace_back()也应该是可变参数的 
// 保持类型信息有两阶段过程:
// 1. 为保持实参中类型信息--->将emplace_back()的函数参数定义为模板类型参数的右值引用
// 2. 将这些实参传给construct时,使用forward保持实参原始类型

int main()
{
    // int i = 0;
    // double d = 3.14;
    // string s = "haha";
    // foo(i, s, 42, d); // 3 ,3 
    // foo(s, 42, "hoi"); // 2, 2
    // foo(d,s); // 1, 1
    
    // foo("hihihi"); // 0, 0
    // foo(d,"asdf", "asdfasdf"); // 2, 2

    // 编写可变参数函数模板
    MyClass my("no << operator");
    print(std::cout, 12, 13.3, "asfdae"); // 可变参版本
    print(std::cout, 13.3, "asfdae"); // 可变参版本
    print(std::cout, "asfdae", my); // 两个print版本的参数都是匹配的，但是非可变参数魔板比可变参数
                                // 模板更特例化, 因此编译器选择非可变参数版本
        
    vector<int> res;
    res.emplace_back();
}

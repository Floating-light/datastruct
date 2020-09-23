#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Widget
{
public:
    Widget()
    {
        std::cout << "Default constructor" << std::endl;
    }

    Widget(const Widget& other)
    {
        std::cout << "Copy constructor" << std::endl;
    }

    Widget(char i , char d)
    {
        std::cout << "char and char constructor" << std::endl;
    }

    Widget(const std::string& str)
    {
        std::cout << "Paramter String constructor" << std::endl;
    }

    Widget(const std::initializer_list<int>& values)
    {
        std::cout << "Braced Initialization" << std::endl;
    }

    Widget& operator=(const Widget& rh)
    {
        std::cout << "Assignment operator" << std::endl;
        return *this;
    }

};

int main()
{
    std::cout << "int a{1};"<< std::endl
              << "int b = {1}---------->" << std::endl;
    Widget a{1};
    Widget b = {1}; // 视为int a{1};

    Widget ccc = "12";

    // {} 初始化会强烈地优先选择有std::initializer_list<T>形参的构造函数
    // 哪怕有一丝可能将{}内的东西转为T类型, 也会选择它.

    // 大括号内不允许向下转换
    // Widget c{2.1f, 1.0f};

    std::cout << "Widget d{static_cast<char>(1), static_cast<char>(2)};---->" << std::endl;
    Widget d{static_cast<char>(1), static_cast<char>(2)};
    
    std::cout << "Widget(static_cast<char>(2), static_cast<char>(1));------>" << std::endl;
    Widget(static_cast<char>(2), static_cast<char>(1));
    
    // 'Widget MyError(void)': prototyped function not called (was a variable definition intended?)
    Widget MyError();

    std::cout << "Widget Test{};------>" << std::endl;
    Widget Test{};

    std::cout << "Widget RealBarced({});------>" << std::endl;
    Widget RealBarced({});

    std::cout << "Widget str{\"asdfas\"};------>" << std::endl;
    Widget str{"asdfas"};

    return 0;
}
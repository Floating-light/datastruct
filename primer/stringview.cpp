#include <iostream>
#include <string>
#include <vector>
#include <xstring>
using namespace std;

void func1(const std::string_view& view){
     // / / /
     std::cout << "func1 output: " << view << std::endl;
}

int main() 
{
    std::string MyString("123456789123456789");

    std::string_view v(MyString.c_str() + 9, 8);

    func1(v);
}

// reference: 
// https://stackoverflow.com/questions/20803826/what-is-string-view
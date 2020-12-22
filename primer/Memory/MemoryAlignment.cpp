#include <iostream>

using namespace std;

class Test
{
public:
    char c;
    int i;
};

struct A
{
     A(int) {} 
     A(std::string) {} 
    explicit operator bool() {return true;} 
}a;

void func1(A a) {} 

int main()
{
    func1(1);
    func1(std::string("asdf"));
    std::cout << "Test " << std::endl;
    bool b1(a);
    if(a);

    bool bb = a;
}
/*
reference:
* https://www.jianshu.com/p/1df235f58ee0
* https://blog.csdn.net/haoel/article/details/1948051
* https://coolshell.cn/articles/12176.html
* https://stackoverflow.com/questions/38875369/what-is-data-alignment-why-and-when-should-i-be-worried-when-typecasting-pointe
*/
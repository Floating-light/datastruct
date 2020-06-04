#include <iostream>
#include <thread>
#include <string>

using namespace std;

void function_1()
{
    std::cout << "out put from function 1. " << std::endl;
}

class Fctor
{
    public:
    void operator()(std::string& in)
    {
        std::cout << "from : " << in << std::endl;        
        in = "in fctor";
    }
};

int main()
{
    // how many thread can be running concurrently
    unsigned int thraednum = std::thread::hardware_concurrency();

    Fctor fac;
    // must join or detach t1 before t1 goes out of scope 
    // std::thread t1(Fctor()); // become a function declare.

    std::string s = "message";
    // always pass by value
    std::thread t1(fac, std::move(s)); // become a function declare.
    t1.join();

    fac(s);        
    
    return 0;
}
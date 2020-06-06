#include <iostream>
#include <thread>
#include <string>

using namespace std;

void function_1()
{
    std::cout << "out put from function 1. " << std::endl;
}

void function_2()
{
    for(int i = 0; i < 100; ++i)
    {
        std::cout << "from t1: " << i << std::endl;
    }
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
    std::cout << "hardware number: " << thraednum << std::endl;
    Fctor fac;
    // must join or detach t1 before t1 goes out of scope 
    // std::thread t1(Fctor()); // become a function declare.

    std::string s = "message";
    // always pass by value
    // use std::ref() to pass reference
    std::thread t1(fac, std::ref(s)); 
    t1.join();
     
    // become a function declare.
    
    //std::thread t1(function_2); 

    std::cout << "from main thread : " << s << std::endl;

    // for(int i = 0; i < 1000; ++i)
    // {
    //     std::cout << "from main: " << i << std::endl;
    // }

    
    return 0;
}
#include <iostream>

#include "StrVec.hpp"

int main() 
{
    StrVec v = {"test", "test2", "test3"};
    std::string* b = v.begin();
    std::cout << v.size() << "<<----\n";
    while(b != v.end())
    {
        std::cout << *b << ", " ;
        ++b;
    }
    std::cout << std::endl;
}
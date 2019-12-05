#include <iostream>

#include "StrVec.hpp"

int main() 
{
    StrVec v;
    v.push_back("test");
    v.push_back("elemet2");
    v.push_back("elemet3");
    v.push_back("elemet4");
    v.push_back("elemet5");
    v.push_back("elemet6");
    
    std::cout << "test" << std::endl;
    std::cout << *(v.end()-1) << std::endl;
}
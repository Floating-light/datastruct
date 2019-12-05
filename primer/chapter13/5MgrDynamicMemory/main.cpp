#include <iostream>

#include "StrVec.hpp"

int main() 
{
    StrVec v;
    v.push_back("elemet6");
    
    std::cout << "test" << std::endl;
    std::cout << *(v.end()-1) << std::endl;
}
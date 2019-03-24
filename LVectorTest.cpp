#include "LVector.hpp"
#include <iostream>
int main(int argc, char **argv){
    LVector<int> lv(20, 20, 11);
    std::cout << "Find 24: return " << lv.find(24, 0, 20) << std::endl;
    lv.expand();
    std::cout <<"*************";
    lv.insert(8,6);
    std::cout << "Find 8: return " << lv.find(8, 0, 20) << std::endl;
    lv.print();
    return 0;
}
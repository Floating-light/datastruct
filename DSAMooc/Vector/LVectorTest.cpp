#include "LVector.hpp"
#include <iostream>

struct test{
    //function object ,overloaded () operator
    void operator() (int i){
        std::cout << "Increase one : " << ++i << std::endl;
    }
};

void visit(int& temp){
    std::cout << "===========>" << temp << std::endl;
}

/**
 * function pointer can not directly definition
 * must declare first ,then funcp = function name 
int (*funcp)(int i,char c){
    return c;
}*/

int main(int argc, char **argv){
    LVector<int> lv;
    for(int i = 0; i < 10; ++i)
        lv.insert((rand()%100), i);
    lv.print(); 
    lv.sort(0,10);
    lv.print();
    lv.unsort(0, 10);
    lv.print();
    lv.sort(0, 10);
    lv.print();
    //std::cout << "The position of the element 3: " << lv.search(3) << std::endl;
    //std::cout << "The position of the element 2: " << lv.search(2) << std::endl;
    //std::cout << "The position of the element 0: " << lv.search(0) << std::endl;

    return 0;
}
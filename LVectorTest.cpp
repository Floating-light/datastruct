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
    test currTest;
    LVector<int> lv;
    for(int i = 0; i < 10; ++i)
        lv.insert(i, i);
    lv.unsort(0,10);
    lv.print();

    lv.insert(2,2);
    lv.insert(3,5);
    lv.print();

    lv.deduplicate();
    lv.print();

    lv.traverse(currTest);
    lv.print();

    
    std::cout << "Is ordered ?====>>: " << lv.disordered()<< std::endl;

    std::cout << "Deleted element number : " << lv.uniquify() << std::endl;
    lv.print();
    return 0;
}
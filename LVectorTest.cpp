#include "LVector.hpp"
#include <iostream>

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
        lv.insert(i, i);
    lv.unsort(0,10);
    lv.print();

    lv.insert(2,2);
    lv.insert(3,5);
    lv.print();

    lv.deduplicate();
    lv.print();

    lv.traverse(visit);
    
    return 0;
}
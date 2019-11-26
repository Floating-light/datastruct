#include <iostream>
#include "LList.hpp"

using namespace std;

int main()
{
    LList<int> l;
    for(int i= 0; i < 10; ++i)
    {
        l.insertA(l.last(), i*3+5);
        if(i == 1)
            l.insertA(l.last(), 8);
    }

    l.insertA(l.last(), 23);
    int num = l.uniquify();
    std::cout << "Duplicate : " << num << std::endl;

    int i = 0;
    LListNode<int>* p = l.first();
    while(i < l.size())
    {
        std::cout << p->data << ", " ;
        p = p->succ;
        ++i;
    }
    return 0;
}
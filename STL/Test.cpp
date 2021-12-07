#include <iostream>
#include <thread>
#include <atomic>
struct RMatrix
{
public:
    float M[4][4] ;
};
int main()
{
    std::atomic<int*> node;
    node.compare_exchange_weak();
    std::cout << sizeof(float) << "------------" <<sizeof(double) <<"=============" <<  sizeof(RMatrix)<< std::endl;
    return 0;
}
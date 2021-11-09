#include <iostream>

struct RMatrix
{
public:
    float M[4][4] ;
};
int main()
{
    std::cout << sizeof(float) << "------------" <<sizeof(double) <<"=============" <<  sizeof(RMatrix)<< std::endl;
    return 0;
}
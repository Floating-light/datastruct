#include <iostream>
#include <numeric>

using namespace std;




unsigned int MyGCD(unsigned int a, unsigned int b)
{
    while(b != 0)
    {
        unsigned int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int main()
{
    std::cout << MyGCD(100, 10);
}
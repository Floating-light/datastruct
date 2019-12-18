#include <string>
#include <iostream>

using namespace std;

class MType
{
public:
    int data;
    string str;

    MType(): data(0), str(""){}
    MType(int i, string s ): data(i), str(s){}
};
// error
//int operator+( int , int){}

int operator+(MType m, int i)
{
    return m.data + i*2;
}


int main() 
{
    MType m(111, "name");
    int res = m + 1;
    int res2 = operator+(m, 2);
    
}
#include <iostream>

// default paramater of virtual function

using namespace std;

struct B
{
    virtual void print(int a = 10)
    {
        cout << "print from base B : ";
        cout << a << endl;
    }
};

struct C : B
{
    void print(int a = 10000) override
    {
        cout << "print from derived C : ";

        cout << a << endl;
    }
};

int main()
{
    B b1;
    b1.print();
    
    C c1;
    c1.print();

    B& b2 = c1;

    b2.B::print();
    
    b2.print();
}
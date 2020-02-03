#include <iostream>

using namespace std;

class Base
{
    void funcPrivate()
    {
        cout << "Call private function in Base\n";
    }
    Base(char c)
    {
        cout << "Call private constructor of Base\n";
    }
public:
    Base()
    {
        cout << "Call defautl constructor of Base \n";
    }

    Base(int i)
    {
        cout << "Call public constructor of Base\n";
    }

    void funcPublic()
    {
        cout << "Call public function of Base\n";
    }
protected:
    void funcProtected()
    {
        cout << "Call protected function  of Base\n";
    }
};

class Derived : public Base
{
    using Base::funcPublic;
public: 
    using Base::funcProtected;

    using Base::Base;

    // function "Base::funcPrivate" (declared at line 7) is inaccessible
    //using Base::funcPrivate;
};

int main()
{
    Derived d;
    // function "Base::funcPublic" (declared at line 26) is inaccessible
    //d.funcPublic();

    // that is ok
    d.funcProtected(); 

    Derived d1(1);

    // "Derived::Derived(char c)" (declared implicitly) is inaccessible
    //Derived d2('c');
}
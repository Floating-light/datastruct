#include <iostream>
using namespace std;

class Base 
{
public:
    Base() 
    {
        cout << "Call default constructor of class Base\n";
    }

    Base(const Base& b) 
    {
        cout << "Call copy constructor of class Base\n";
    }

    Base(const Base&& b)
    {
        cout << "Call move constructor of class Base\n";
    }

    Base& operator=(const Base& ) = default;

    virtual ~Base()
    {
        cout << "Call deconstructor of class Base\n";
    }
};

class Derived : public Base
{
public:
    Derived()
    {
        cout << "Call default constructo of Derived\n";
    }

    Derived(const Derived& d): Base(d)
    {
        cout << "Call copy constructor of Derived\n";
    }

    // oddly
    Derived(const Derived&& d) /* :Base(std::move(d)) */
    {
        cout << "Call move constructor of Derived\n";
    }

    //Base::operator=(cosnt Base&) is not invoked automatically
    Derived& operator=(const Derived& r)
    {
        // as usuall,
        // handing self-assignment and freeing existing resources as appropriate.
        // copy the base part of r to the base part of this object 
        Base::operator=(r);

        // TODO: to copy the Derived part of r to this object.
    }
    
    ~Derived()
    {
        cout << "Call deconstructor of Derived\n";
    }
};

int main()
{
    // Call default constructo of Derived
    // Call copy constructor of class Base
    Derived d;  

    // Call default constructo of Derived
    // Call copy constructor of class Base
    Derived d1(d);

    // Call default constructor of class Base
    // Call move constructor of Derived
    Derived d2(std::move(d));

    // All of above  
    // Call deconstructor of Derived
    // Call deconstructor of class Base
}
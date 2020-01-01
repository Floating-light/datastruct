#include <iostream>

using namespace std;

struct B;

struct A
{
    A() = default;
    A(const B& ) { cout << "converting constructor B ---> A\n"; }
};

struct B
{
    B() = default;
    operator A() const { cout << "conversion operator B ---> A" << std::endl; }
};

A f(const A&){ }

int main()
{
    
    B b;
    A a = f(b); // call converting constructor under g++ 7.4
    a = b;      // call converting constructor
                // say ambiguous in c++ primer 5th, p584, c++11
    cout << "----------------------------\n";
    // must explicitly call conversion operator or conversion constructor
    A a1 = f(b.operator A());
    A a2 = f(A(b));

    //can't resolve the ambiguity by cast.
}
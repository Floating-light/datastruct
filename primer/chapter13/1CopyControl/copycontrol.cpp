#include <iostream>

using namespace std;

class Foo
{
    public:
    Foo(){ std::cout << "default constructor" << std::endl;}
    Foo(const string& n);          //default constructor

    //copy constructor
    //also can be nonconst, but must be a reference type, why? 
    //In case of indefinitely call copy constructor.
    /*happen when:
     1. define variables using an =.
     2. Pass an object as an argument to a parameter of nonreference type.
     3. Return an object from a function that has a nonreference return type.
     4. Brace initialize the elements in an array or the members of an aggregate.
     5> allocte? For example, the library containers.
    */
    Foo(const Foo&);
    
    //Copy-Assignment operator ordinarily should return a reference to their 
    //left-hand operand.
    Foo& operator=(const Foo&);

    string name;
};

Foo::Foo(const string& n):
    name(n)
{

}
Foo::Foo(const Foo& origin):
name(origin.name)
{
    std::cout << "call copy constructor" << std::endl;
}

Foo& Foo::operator=(const Foo&)
{
    std::cout << "call copy-assignment operator" << std::endl;
}

int main()
{
    string s;//direct initialization?(default constructor, no parameters)
    string s2(10,'s');//direct initialization
    string s3(s2);//direct initialization

    string s4 = s2;//copy initialization
    string s5 = "string";//copy initialization
    string s6 = string("string");//copy initialization
    //The compiler is permitted to rewrite:
    //string s = "stringstring";
    //into
    //string s("stringstring");

    Foo f0;
    Foo f1 = Foo("riven");
    Foo f2 = f1;//copy constructor
    f2 = f1;//copy-assignment operator
    std::cout << "the nam of f2:" << f2.name << s <<std::endl;
}

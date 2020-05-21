#include <string>

using namespace std;

class Foo
{
public:
    Foo& operator=(const Foo&) & ;
    Foo sorted() &&;       // may run on modifiable rvalues
    Foo sorted() const &;  // may run on any kond of Foo
};

// We indicate the lvalue/rvalue property of this in the sameway thatwe define
// const member functions (§ 7.1.2, p. 258);
Foo& Foo::operator=(const Foo& )&//just like const , make this const or &
{

}

Foo &retFoo(); // returns a reference; a call to retFoo is an lvalue
Foo retVal(); // returns by value; a call to retVal is an rvalue
Foo i, j; // i and j are lvalues
i = j; // ok: i is an lvalue
retFoo() = j; // ok: retFoo() returns an lvalue
retVal() = j; // error: retVal() returns an rvalue
i = retVal(); // ok: we can pass an rvalue as the right-hand operand to assignment

// A function can be both const and reference qualified. In such cases, the reference
// qualifier must follow the const qualifier:
class Foo1 {
public:
Foo1 someMem() & const; // error: const qualifier must come first
Foo1 anotherMem() const &; // ok: const qualifier comes first
};


// When we define const memeber functions, we can define two versions that
// differ only in that one is const qualified and the other is not. There is no similar
// default for reference qualified functions. When we define two or more members
// that have the same name and the same parameter list, wemust provide a reference
// qualifier on all or none of those functions:
class Foo2
{
public:
    Foo sorted() &&;
    Foo sorted() const; // error: must have reference qualifier
    // Comp is type alias for the function type (see § 6.7 (p. 249))
    // that can be used to compare int values
    using Comp = bool(const int&, const int&);
    Foo sorted(Comp*); // ok: different parameter list
    Foo sorted(Comp*) const; // ok: neither version is reference qualified
    Foo sorted(Comp* ) &; 
}
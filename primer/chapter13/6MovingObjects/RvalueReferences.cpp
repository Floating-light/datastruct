#include <string>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

// lvalue references : ===> persist(持久的) state 
// assignment, subscript, dereference, perfix increment/decrement
// operators ---> return lvalue 
// Type& = lvalue;

// rvalue references : ===> ephemeral 短暂的 
//                     ===> used to be literals or temporary objects
// functions that return a nonreference type, along with the 
// arithmetic, relational, bitwise, postfix increment/decrement
// operators, all yield rvalues.
// cannot bind an lvalue reference to these expressions, but can 
// bind either an lvalue reference to const or an rvalue reference
// to such expressions.
// cosnt Type& = rvalue; Type&& = rvalue;
/* Because rvalue references can only be bound to temporaries:
   The referred-to object is about to be destroyed. 
   There can be no other users of that object.
   So that is free to take over resources from an rvalue reference refers.
 */
// warning: Variable expressions are lvalues. Just like a experssion
//          with one operand and no operator.

int &&rr1 = 42;
//int &&rr2 = rr1;
int &&rr3 = std::move(rr1); // explicitly cast an lvalue to its 
                            // corresponding rvalue reference type.

// We can destroy a moved-from object and cann assign a new value to
// it, but we cannot use the value of a moved-from object.
// we cannot make any assumptions about the value of the moved-from object.
int main() 
{
    std::vector< string> first{"asdf", "asdf", "asdfwe"};
    std::vector< string>second(std::move(first));
    // the moved-from object remains valid, but we don't know what result
    //  we'll get, that may be empty, but that is not  guaranteed.
    std::cout << "The size of first: " << first.size() << std::endl;
    std::cout << "The size of second: " << second.size() << std::endl;
}

// if a clas doesn't have a move operation, the corresponding copy operation
// is used inplace of move through noraml function matching.

// synthesize move constructor or move-assignment operator 
// only if the class doesn't define any of its own copy-control members(copy constructor..) and
// if every nonstatic data member can be moved(built-in, class type have move operation)

// the compiler will synthesize the move operations for X and hasX
struct X{ // no define any of its own copy-control members
    int i; // build-in type
    std::string  s; // string define its own move operations
};

struct hasX
{ 
    X mem; // X has synthesized move operations
};

// assume Y is a class that defines its own copy constructor but not a move constructor
struct Y{
    int i;
    Y(const Y& in ){i = in.i;};
     // no move constructor
};
struct hasY {
    hasY() = default;
    hasY(hasY&&) = default;
    Y mem; // hasY will have a deleted move constructor
};
hasY hy, hy2 = std::move(hy); // error: move constructor is deleted
// Classes that define a move constructor or move-assignment operator
// must also define their own copy operations. Otherwise, those members
// are deleted by default.

// Rvalues are moved, lvalues are copied ... 
// But Rvalues are copied if there is no move constructor

class Foo
{
public:
    Foo() = default;
    Foo(const Foo&); // copy constructor
    // other memebers, but Foo does not define a move constrcutor
};
Foo x;
Foo y();            // copy cosntructor; x is a lvalue;
Foo z(std::move(x));// copy constructor , because there is no move constructor
                    // we can convert a Foo&& to a const Foo&

// All five copy-control members:
// copy constructor, copy-assignment operator, destructor, move-constructor, move-assignment operator
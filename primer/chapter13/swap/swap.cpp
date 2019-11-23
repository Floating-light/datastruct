#include <string>

class HasPtr
{
friend void swap(HasPtr&, HasPtr&);

public:
    HasPtr(const int& in, const std::string& sin = std::string()):
    a(in), pstr(new std::string(sin))
    {

    }
    HasPtr(const HasPtr& right):
    a(right.a), pstr(new std::string(*right.pstr))
    {

    }
    HasPtr& operator=(HasPtr right)
    {
        /* std::string* temp = new std::string(*right.pstr);
        delete pstr;
        pstr = temp;
        a = right.a; */
        // using swap version copy-assignment, the parameter is not reference.
        swap(*this, right);
        return *this;
        // self assignment and exception salf?
        // only code might throw is the new expression indise the copy constructor,
        // if occurs, it will happen before we have changed the left-hand operand.
    }

    ~HasPtr()
    {
        delete pstr;
    }
private:
    int a;
    std::string* pstr;
};

//swap two object may look like:
//HasPtr temp = v1; // copy constrcutor
//v1 = v2;
//v2 = temp; //copy assignment in origin v1
//it also allocates memory.
//that is unnecessary.But we can define ourself version swap function for this class.
inline void swap(HasPtr& l, HasPtr& r)
{
    using std::swap; //if memeber type has not itself swap function, will call std::swap.
    swap(l.a, r.a);
    swap(l.pstr, r.pstr);
}

class Foo
{
    friend void swap(Foo&, Foo&);
    HasPtr h;
};

inline void swap(Foo& l, Foo& r)
{
    //this function use the library version of swap, not the HasPtr function version.
    //this code would complie and execute.
    //this is a error useage.
    std::swap(l.h, r.h);
}

//correct version
inline void swap(Foo& l, Foo& r)
{
    using std::swap;
    swap(l.h, r.h);//there is a type-specific version of swap, will call this version preferentially.(16.3.p697)(18.2.3.p798)
}

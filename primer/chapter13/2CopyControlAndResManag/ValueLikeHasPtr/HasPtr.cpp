#include <string>

class HasPtr
{
public:
    HasPtr(const int& in, const std::string& sin = std::string());
    HasPtr(const HasPtr& );
    HasPtr& operator=(const HasPtr&);

    ~HasPtr();
private:
    int a;
    std::string* pstr;
};

HasPtr::HasPtr(const int& in, const std::string& sin = std::string()):
    a(in), pstr(new std::string(sin))
{

}

HasPtr::HasPtr(const HasPtr& in):
a(in.a), pstr(new std::string(*in.pstr))
{

}

HasPtr::~HasPtr()
{
    delete pstr;
}

//typically combine the actions of the destructor and the copy constructor.
//these actions be done in a sequence that is correct even if an object is assigned to itself.
//by first copying the right-hand side.
HasPtr& HasPtr::operator=(const HasPtr& right)
{
    auto newp = new std::string(*right.pstr);//in case of assigned to itself
    delete pstr;
    pstr = newp;
    a = right.a;
    return *this;
}

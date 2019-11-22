#include <string>

class HasPtr
{
public:
    HasPtr(const int& in, const std::string& sin = std::string()):
    a(in), pstr(new std::string(sin))
    {

    }
    HasPtr(const HasPtr& right):
    a(right.a), pstr(new std::string(*right.pstr))
    {

    }
    HasPtr& operator=(const HasPtr& right)
    {
        std::string* temp = new std::string(*right.pstr);
        delete pstr;
        pstr = temp;
        a = right.a;
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
//that is unnecessary.
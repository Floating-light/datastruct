#include <string>

class HasPtr
{
public:
    HasPtr(const std::string& s = std::string()):
        i(0), pstr(new std::string(s)), use(new std::size_t(1))
        {

        }
    HasPtr(const HasPtr& p):
        i(p.i), pstr(p.pstr), use(p.use)
        {
            ++*use;
        }

    ~HasPtr();

    HasPtr& operator=(const HasPtr&);
private:
    std::string* pstr;
    int i;
    std::size_t* use;
};

HasPtr::~HasPtr()
{
    if(--*use == 0)
    {
        delete pstr;
        delete use;
    }
}

HasPtr& HasPtr::operator=(const HasPtr& right)
{
    ++*right.use;
    if(--*use == 0)
    {
        delete pstr;
        delete use;
    }
    pstr = right.pstr;
    i = right.i;
    use = right.use;
    return *this;
}


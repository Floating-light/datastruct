#include <stdexcept>
#include <iostream>

class SmallInt
{
    friend SmallInt operator+(const SmallInt& , const SmallInt&);
    public:
    SmallInt(int i = 0): val(i) 
    {
        if(i < 0 || i > 255)
            throw std::out_of_range("Bad SmallInt value");
    } 
    //operator int() const { return val; }
    explicit operator bool() const { return val > 0; }
    operator int() const { return val; }
    private:
    std::size_t val;
};

SmallInt operator+(const SmallInt& l , const SmallInt& r)
{
    return SmallInt(l.val + r.val);
}

int main()
{
    SmallInt si;
    SmallInt s2;
    
    si = 4;
    std::cout << static_cast<bool>(si); // not auto convert to bool
    //std::cout << si;  // error if not + operator

    if(si) // OK auto convert to bool
    {

    }

    SmallInt s3 = si + 3; // ambiguities , can convert si to int or convert 3 to SmallInt and use the SmallInt version of +.
}



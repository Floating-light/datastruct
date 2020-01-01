#include <stdexcept>
#include <iostream>

class SmallInt
{
    public:
    SmallInt(int i = 0): val(i) 
    {
        if(i < 0 || i > 255)
            throw std::out_of_range("Bad SmallInt value");
    } 
    //operator int() const { return val; }
    explicit operator bool() const { return val > 0; }
    private:
    std::size_t val;
};

int main()
{
    SmallInt si;
    si = 4;
    std::cout << static_cast<bool>(si);
    si + 1;
    if(si) // OK
    {

    }
}
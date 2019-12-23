#include <iostream>
#include <vector>

using namespace std;

// Function object class
class PrintString
{
public:
    PrintString(ostream&o = cout, char c = ' '): 
        os(o), sep(c){}
    void operator()(const string& s) const 
    {
        os << s << sep;
    }
private:
    ostream& os;
    char sep;
};

int main()
{
    PrintString p;
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    std::cout << std::endl;
}
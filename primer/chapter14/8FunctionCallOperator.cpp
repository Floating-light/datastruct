#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>

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
    int fff(int i, int j);
private:
    ostream& os;
    char sep;
};

namespace func
{
    int add(int i, int j) { return i + j; }
    auto mod = [](int i, int j ) -> int { return i % j; };
    struct divide
    {
        int operator()(int denominator, int divisor)
        {
            return denominator / divisor;
        }
    };
    PrintString p;
    map<char, function<int(int, int)>> binops =
    {
        {'+', add},
        {'-', std::minus<int>()},
        {'/', divide()},
        {'*', [](int i, int j)->int{return i * j;} },
        {'%', mod},
        {']', p.fff}

    };
}
int main()
{
    PrintString p;
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    p("asdgfertgoifwedg");
    std::cout << std::endl;
    plus<int> intAdd;
    intAdd(2,3);


    vector<string*> names;
    sort(names.begin(), names.end(), [](string* a, string* b)
    {
        return a < b; // thie pointers in names are unrealated, so < is undefined behavior
    });
    sort(names.begin(), names.end(), less<string*>()); // that's good;
    int count = 0;
    auto f =  [&count](int i)
    {
        if(i > 1024) ++count;
    };

}
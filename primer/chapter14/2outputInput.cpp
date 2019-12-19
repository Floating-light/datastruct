#include <iostream>
#include <string>

using namespace std;

class Test 
{
friend ostream& operator<<(ostream&, const Test&);
private:
    int data;
    string name;
public:
    Test(int d = 10, string s = "test string"): data(d), name(s){ }
};

ostream &operator<<(ostream &os, const Test& item)
{
    os << item.data << " " << item.name << std::endl;
}

int main()
{
    Test t;
    cout << t;
}
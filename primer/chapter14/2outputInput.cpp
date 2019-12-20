#include <iostream>
#include <string>

using namespace std;

class Test 
{
friend ostream& operator<<(ostream&, const Test&);
friend istream& operator>>(istream&, Test&);
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

istream &operator>>(istream &is, Test& item)
{
    is >> item.data >> item.name;
    if(is)
    {
        item = Test();
    }
}

int main()
{
    Test t;
    cin >> t;
    cout << t;
}
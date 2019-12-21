#include <iostream>
#include <string>

using namespace std;

class Test 
{
friend ostream& operator<<(ostream&, const Test&);
friend istream& operator>>(istream&, Test&);
friend Test operator+(const Test&, const Test&); // - * / > < >= <= etc.
friend bool operator==(const Test&, const Test&);
friend bool operator!=(const Test&, const Test&);
private:
    int data;
    string name;
public:
    Test(int d = 10, const string s = "test string"): data(d), name(s){ }
    Test(initializer_list<int>& ini)
    {
        this->data = *ini.begin();
        this->name = string("from initializer list constructor");
        std::cout << "from initializer list constructor" << this->name << std::endl;
    }
    Test(const Test& l)
    {
        this->data = l.data * 2;
        this->name = "copy constructor " + l.name;
    }
    // must be defined as members
    Test& operator=(const Test& l)
    {
        // free the existing dynamic memory

        this->data = l.data;
        this->name = l.name;
        std::cout << "operator= parameter const Test&" << std::endl;
        return *this;
    }
    Test& operator=(const initializer_list<int>& l)
    {
        // free the existing dynamic memory

        this->data = *l.begin();
        this->name = "sdafgghsfghj";
        std::cout << "operator= parameter const Test&" << std::endl;
        return *this;
    }
    // ordinarily be defined as members
    Test& operator+=(const Test& l)
    {
        this->data += l.data;
        this->name += l.name;
        return *this;
    }

    //must be defined as members
    int& operator[](std::size_t n)
    {
        return this->data;
    }
    const int& operator[](std::size_t n) const 
    {
        return this->data;
    }

};




ostream &operator<<(ostream &os, const Test& item)
{
    os << item.data << " " << item.name << std::endl;
}

istream &operator>>(istream &is, Test& item)
{
    is >> item.data >> item.name; 
    if(!is || item.data <100) // 完成输入后检查stream 的状态，而不是每次输入都检查，这样对不同的错误处理过于复杂
            // 只要有错误出现，就把item重置为初始状态， 且仍然是有效的，可用的，防止因错误出现部分更改，导致不可预期的结果。
    {
        item = Test();
        is.setstate(istream::failbit); // usually should only set the failbit.
    }
}

Test operator+(const Test& l, const Test& r)
{

    Test n = l;
    n+= r;

    // Test n;
    // n.data = l.data + r.data; // 不用这个， 减少对成员不必要的构造 + 创建了新的对象，并返回copy
    // n.name = l.name + r.name; // += 没有额外的构造和copy

    return n;
}
bool operator==(const Test& l, const Test&r)
{
    return l.name == r.name && l.data == r.data;
}
bool operator!=(const Test&l, const Test&r)
{
    return !(l == r);
}

int main()
{
    Test t1{4367658};
    
    cout <<t1;
}
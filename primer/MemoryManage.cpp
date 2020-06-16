#include <memory>
#include <vector>
#include <string>


using namespace std;
class MyClass
{

    std::string s;
};

int main()
{
    int* p = allocator<int>().allocate(5);
    allocator<int>().deallocate(p, 5);
    MyClass* my = (MyClass*)malloc(sizeof(MyClass));
    my->MyClass::MyClass();
    operator new(10); // 直接分配10个单位的内存的函数=----> malloc
    new(MyClass);
    vector<int> v;
    operator delete(ptr); // ---->> free
}
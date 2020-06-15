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
    
}
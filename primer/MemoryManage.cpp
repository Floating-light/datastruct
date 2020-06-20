#include <memory>
#include <vector>
#include <string>


using namespace std;
class Screen
{
public:
    Screen(int x) : i(x){}
    int get() { return i;}
    void* operator new(size_t);
    void operator delete(void*, size_t);
private:
    Screen* next;
    static Screen* freeStore;
    static const int screenChunk;
private:
    int i;
};
Screen* Screen::freeStore = 0;
const int Screen::screenChunk = 24;
void* Screen::operator new(size_t size)
{
    Screen *p;
    if(!freeStore)
    {
        size_t chunk = screenChunk * size;
        // char = 8 bit = 1 byte
        freeStore = p = reinterpret_cast<Screen*>(new char[chunk]);
        for(;p != &freeStore[screenChunk - 1]; ++p)
        {
            p->next = p+1;
        }
        p->next = 0;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}
void Screen::operator delete(void * p, size_t)
{
    Screen* deleted = static_cast<Screen*>(p);
    deleted->next = freeStore;
    freeStore = deleted;
}


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
    if(true)
    {
        while(true)
        {

        }
        wait()
        try
        {
            /* code */
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
}
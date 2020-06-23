# Memory manage 
目的:
1. 提升程序运行速度
2. 减少内存浪费

```c++
// memory manage
// 1. 减少malloc调用次数, 提高运行速度(虽然malloc 并不会很慢, 但减少它的调用总是好的)
// 2. 减少内存分配时的cookie使用(调一次malloc, 8 byte)
// 提高速度, 降低内存浪费

class Screen
{
public:
    Screen(int x) : i(x){}
    int get() { return i;}
    void* operator new(size_t);
    void operator delete(void*, size_t);
private:
    // 下一段可用的内存空间, 在operator new 和delete中使用
    // 需要的额外空间(4 byte)
    Screen* next;
    // 指向开辟好的第一个可用空间区域
    static Screen* freeStore;
    // 每次分配的内存空间数目
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
        // char = 8 bit = 1 byte = 1 B
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
```

reference :
1. https://blog.csdn.net/caogenwangbaoqiang/article/details/79788368
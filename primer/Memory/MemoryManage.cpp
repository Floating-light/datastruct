#include <memory>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
// memory manage
// 1. 减少malloc调用次数, 提高运行速度(虽然malloc 并不会很慢)
// 2. 减少内存分配时的cookie使用(8 byte)

// version 1
class Screen
{
public:
    Screen(int x) : i(x){}
    int get() { return i;}
    void* operator new(size_t);
    void operator delete(void*, size_t);
private:
    // 下一段可用的内存空间
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

// version 2
// Effective C++ 2e, item 10.
// 考虑到记录下一个可用位置的指针,与这个类的数据成员不可能同时有用
// 可以用 union 使类的数据成员和next指针共用一块内存区域, 
// 在next有意义时(开辟内存但未使用),数据成员并未初始化]
// 当要使用这块内存时, 直接在这块区域上初始化数据成员,next 也完成了它的使命不再需要
class Airplane
{
private:
    struct AirplaneRep // represstion
    {
        unsigned long miles;
        char type;
    };
private:
    union
    {
        AirplaneRep rep; // 8byte, 对齐3byte
        Airplane* next;
    }; // anonymous union
public:
    Airplane(unsigned long m, char t)
    {
        rep.miles = m;
        rep.type = t;
    }
    unsigned long getMiles()
    {
        return rep.miles;
    }
    char getType() 
    {
        return rep.type;
    }
    void set(unsigned long m, char t)
    {
        rep.miles = m;
        rep.type = t;
    }
public:
    static void* operator new(size_t size);
    static void operator delete(void* deadObject, size_t size);
private:
    static const int BLOCK_SIZE;
    static Airplane* headOfFreeList;
};
Airplane* Airplane::headOfFreeList;
const int Airplane::BLOCK_SIZE = 512; // 内存池每次申请的内存大小

void* Airplane::operator new(size_t size)
{
    // 当继承发生时, 大小有误
    if(size != sizeof(Airplane))
        return  ::operator new(size); // 直接用全局的new开辟内存
    
    Airplane* p = headOfFreeList;
    if(p)
        headOfFreeList = p->next;
    else
    {
        // 没有可用的已申请的内存, 开辟内存
        Airplane* newBlock = static_cast<Airplane*>(::operator new(BLOCK_SIZE*sizeof(Airplane)));

        for(int i = 1; i < BLOCK_SIZE - 1; ++i)
        {
            newBlock[i].next = &newBlock[i+1];
        }
        newBlock[BLOCK_SIZE-1].next = 0;
        p = newBlock;
        headOfFreeList = &newBlock[1];
    }
    return p;
}
// 我们并未真正将内存归还操作系统(调用free), 当前
void Airplane::operator delete(void* deadObject, size_t size)
{
    if(deadObject == 0) return ;
    if(size != sizeof(Airplane))
    {
        ::operator delete(deadObject);
        return ;
    }
    Airplane* carcass = static_cast<Airplane* >(deadObject);
    carcass->next = headOfFreeList;
    headOfFreeList = carcass;
}

// version 3
class MyAllocator
{
public:
    struct obj
    {
        struct obj* next;
    };
public:
    void* allocate(size_t);
    void deallocate(void*, size_t);
private:
    obj* freeStore = nullptr;
    const int CHUNK = 5;
};

void* MyAllocator::allocate(size_t size)
{
    obj* p;
    if(!freeStore)
    {
        freeStore = p = (obj*)malloc(CHUNK*size);
        for(int i = 0; i < (CHUNK-1); ++i)
        {
            p->next = (obj*)((char*)p + size);// char, '+'每次移动1个byte
            p = p->next;
        }
        p->next = nullptr;
    }
    p = freeStore;
    freeStore = freeStore->next;
    return p;
}

void MyAllocator::deallocate(void*p, size_t)
{
    ((obj*)p)->next = freeStore;
    freeStore = (obj*)p;
}

class Foo
{
public:
    long L;
    string str;
    static MyAllocator myAlloc;
public:
    Foo(long l): L(l){}
    static void* operator new(size_t size)
    {
        return myAlloc.allocate(size);
    }
    static void operator delete(void* pdead, size_t size)
    {
        return myAlloc.deallocate(pdead, size);
    }
};
MyAllocator Foo::myAlloc;

// version 4
// 观察 Foo 和内存管理相关写法, 十分固定
// 我们可以用宏简化这一写法
// MFC 是这样做的
#define DECLARE_POOL_ALLOC() \
public:\
    void* operator new(size_t size) { return myAlloc.allocate(size);} \
    void operator delete(void* pdead, size_t size) { myAlloc.deallocate(pdead, size); } \
protected: \
    static MyAllocator myAlloc; \
private:

#define IMPLEMENT_POOL_ALLOC(class_name) \
MyAllocator class_name::myAlloc;

class Goo
{
    DECLARE_POOL_ALLOC()
public:
    int x;
    int y;
    Goo(int _x, int _y) : x(_x),y(_y) { }
    friend std::ostream& operator<<(std::ostream& os, const Goo& g);
};
IMPLEMENT_POOL_ALLOC(Goo)

std::ostream& operator<<(ostream& os, const Goo& g)
{
    std::cout << "(" << g.x << ", " << g.y << ")";
    return os;
}
class Test
{
public:
    void func1()
    {

    }
    string func2(int i, string s)
    {

    }
};

// 按以上方法, 每个需要做内存管理的类都需要写内存管理相关的操作,
// 这样一来重复的操作太多
// 同样的东西应该集中在一起
int main()
{
    Foo* foo[8];
    for(int i = 0; i < 8; ++i)
    {
        foo[i] = new Foo(i*10);
        std::cout << "Number: " << i << "--->"<<foo[i] << ", data: " << foo[i]->L << std::endl;
    }
    std::cout << "size of Test(not member data): "<< sizeof(Test) << std::endl;
    std::cout << "size of Foo: "<< sizeof(Foo) << std::endl;
    std::cout << "--------------------------------" << std::endl;
    Goo* goo[11];
    for(int j = 0; j < 11; ++j)
    {
        goo[j] = new Goo(1*j, 2*j);
        std::cout << "Number: " << j << "--->" << goo[j] << ", data: "<< *(goo[j]) << std::endl;
    }

}
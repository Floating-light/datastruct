#include <iostream>
#include <memory>
#include <string>

using namespace std;
// operator new 无法分配出申请的内存时, 会抛出异常std::bad_alloc exception.
// 旧编译器会返回 0. 也可以显式指定编译器这样做 new(nothrow) Foo;
// 在抛出异常之前可以调用一个可由 client 指定的 handler 
// typedef void(*new_handler)();

// new handler 的职责
// 1. 让更多的memory可用, 释放可释放的内存
// 2. 调用 abort() 或 exit()

void noMoreMemory()
{
    static int count = 0;

    std::cout << "out of memory: " << ++count << std::endl;
    // abort();
    // 如果不调用这个, operator new会不断调用 new handler， 直到获得足够的 memory 成功分配内存.
}


class Foo
{
public:
// '= default' can only appear on default constructors, 
// copy/move constructors, copy/move assignment operators, and destructors
static void* operator new(size_t size) = delete;
static void operator delete(void*,size_t) = delete;
};
int main()
{
    // set_new_handler(noMoreMemory);
    // int* p = new int[999999999999999999];
    // // static_assert(p);

    // p = new int[999999999999999999];

    // =defalut, =delete
    Foo* f = ::new Foo;
}
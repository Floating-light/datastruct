#include <iostream>
#include <memory>
#include <alloca.h>
#include <vector>
#include <stl_alloc.h>
using namespace std;
// class allocator : xmemory---> line 738

// 内存对齐
// 一般内存池中开辟的内存大小都是8的倍数, 当对象大小不是8的倍数时需要对齐

// 嵌入式指针
// 对象本身的大小必须大于 4 byte
int main()
{
    std::shared_ptr<int> s;
    allocator<int> alloc;alloc.allocate()
    alloc.construct()
    std::vector<int> v;
    v.push_back();
    std::unique_ptr<int> uq = std::make_unique<int>(new int(10));
    int * ii = new int(19);
    uq = &ii;
    std::shared_ptr<int> sp = std::make_shared<int>(11);

    std::weak_ptr<int> wp;
    std::vector<int> vv;
    vv.begin();
    vv.pop_back()
}
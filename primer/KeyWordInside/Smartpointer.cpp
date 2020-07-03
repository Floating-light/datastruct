#include <memory>
#include <vector>
#include <iostream>

using namespace std;

struct test
{
    std::shared_ptr<int> next;
}

int main()
{
    std::auto_ptr<int> ap;
    std::unique_ptr<int> uq(new int(10));
    std::unique_ptr<int> qq = uq;
    qq = std::move(uq);//delete assignment operator

    std::shared_ptr<int> sp = std::make_shared<int>(10);

    std::weak_ptr<int> wp;
    wp.
}
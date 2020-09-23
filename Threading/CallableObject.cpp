#include <iostream>
#include <thread>
#include <future>
#include <functional>
#include <unordered_map>
#include <vector>

using namespace std;

class A
{
public:
void f(int x, char c){}
long g(double x) { return 0;}
int operator()(int N){return 0;}
    std::once_flag flag;

};

void foo(int x)
{

}

int main()
{
    A a;
    std::thread(a, 6); // copy of a() in a different thread
    std::thread(std::ref(a), 6); // a() in a different thread
    std::thread(std::move(a), 6); // a is no longer ysable in main thread

    std::thread([](int x){return x*x;}, 6); // 
    std::thread(foo, 6); // 
    std::thread(&A::f,a, 6, 'c'); // copy of a .f(8,'c') in a different thread
    std::thread(&A::f,std::ref(a), 6, 'c'); // a.f(8,'c') in a different thread
    
    std::packaged_task<int(int)> ta;
    int x = ta.get_future().get();

    //std::async(std::launch::async, a, 6);
    //std::bind(a, 6);
    std::call_once(a.flag, a, 6);
    std::function<void (int, float)> sign; 

    std::unordered_map<std::string, std::vector<int>> map;
    int i = 0;
    new(&i) int(11);
    printf(i);

    
}
#include <thread>
#include <future>
#include <iostream>
#include <chrono>

using namespace std;

int factorial(std::future<int>& f)
{
    int res = 1;
    int N = f.get();
    for(int i = N; i > 1; i--)
    {
        res *= i;
    }
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    cout << "Result is : " << res << endl;
    return res;
}

int SomeWork(int InParam)
{
    int res = 1;
    int N = InParam;
    for(int i = N; i > 1; i--)
    {
        res *= i;
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    cout << "Result is : " << res << endl;
    return res;
}
// Get return value form sub thread work

void TestGetValueForWork()
{
    using sys_per_ratio = std::chrono::system_clock::period ;
    std::chrono::system_clock::time_point tp_begin = std::chrono::system_clock::now();
    std::future<int> fu = std::async(std::launch::async, SomeWork, 5);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "after main thread sleep : " << (std::chrono::system_clock::now() - tp_begin).count()  * sys_per_ratio::num / sys_per_ratio::den << std::endl;

    int res = fu.get();

    std::cout << "GetValue : " << res << ",    duration : " << (std::chrono::system_clock::now() - tp_begin).count()  * sys_per_ratio::num / sys_per_ratio::den << std::endl;

}
int main()
{
    int x = 0;
    // 想传给子线程一个int参数,但调用的时候不知道这个参数的具体值,
    // 所以我们承诺会在未来给它一个值.
    std::promise<int> p;
    std::future<int> f = p.get_future();

    // can be copy 
    // std::shared_future<int> sf = f.share();

    std::future<int> fu = std::async(std::launch::async, factorial, std::ref(f));

    // do something else
    std::this_thread::sleep_for(std::chrono::seconds(1));

    p.set_value(4);
    // p.set_exception(std::make_exception_ptr(std::runtime_error("To err is human")));

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    x = fu.get();

    std::cout << "Get from child: " << x << std::endl;
    std::cout << "-----------------------------------------------------" << x << std::endl;

    TestGetValueForWork();
    return 0;
}
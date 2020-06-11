#include <thread>
#include <future>
#include <iostream>

using namespace std;

int factorial(std::future<int>& f)
{
    int res = 1;
    int N = f.get();
    for(int i = N; i > 1; i--)
    {
        res *= i;
    }
    cout << "Result is : " << res << endl;
    return res;
}

int main()
{
    int x = 0;
    // 想传给子线程一个int参数,但调用的时候不知道这个参数的具体值,
    // 所以我们承诺会在未来给它一个值.
    std::promise<int> p;
    std::future<int> f = p.get_future();

    // can be copy 
    std::shared_future<int> sf = f.share();

    std::future<int> fu = std::async(std::launch::async, factorial, std::ref(f));

    // do something else
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //p.set_value(4);
    //p.set_exception(std::make_exception_ptr(std::runtime_error("To err is human")));

    //int x = fu.get();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "Get from child: " << x << std::endl;

    return 0;
}
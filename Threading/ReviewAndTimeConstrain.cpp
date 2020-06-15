#include <iostream>

#include <thread>
#include <mutex>
#include <future>

using namespace std;

int factorial(int N)
{
    int res = 1;
    for(int i = N; i > 1; i--)
    {
        res *= i;
    }
    cout << "Result is : " << res << endl;
    return res;
}

int main()
{
    /* thread */
    std::thread t1(factorial, 6);
    std::this_thread::sleep_for(chrono::milliseconds(3));
    std::chrono::steady_clock::time_point tp = chrono::steady_clock::now() + chrono::microseconds(4);
    std::this_thread::sleep_until(tp);

    /* Mutex */
    std::mutex mu;
    std::lock_guard<std::mutex> locker(mu);
    std::unique_lock<std::mutex> locker2(mu);
    locker2.try_lock();
    locker2.try_lock_for(chrono::nanoseconds(500));
    locker2.try_lock_until(tp);

    /* Condition variable*/
    std::condition_variable cond;
    cond.wait_for(locker2, chrono::microseconds(2));
    cond.wait_until(locker2, tp);
    
    /* Future and Promise */
    std::promise<int> p;
    std::future<int> f = p.get_future();    
    f.get();
    f.wait();
    f.wait_for(std::chrono::microseconds(2));
    f.wait_until(tp);

    /* async() */
    std::future<int> fu = async(factorial, 6);

    /* Packaged Task*/
    std::packaged_task<int(int)> t(factorial);
    std::future<int> fu2 = t.get_future();
    t(6);
}
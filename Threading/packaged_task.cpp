#include <thread>
#include <future>
#include <iostream>
#include <functional>
#include <queue>

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

std::deque<std::packaged_task<int()>> task_q;
std::mutex mu;
std::condition_variable cond;
void thread_1()
{
    std::packaged_task<int()> t;
    {
        // std::lock_guard<std::mutex> locker(mu);
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, [](){ return !task_q.empty();});
        t = std::move(task_q.front());
        task_q.pop_front();
    }
    t();
}

int main()
{
    std::thread t1(thread_1);

    auto f = std::bind(factorial, 6);
    std::packaged_task<int()> t(f);
    std::future<int> res = t.get_future();
    {
        std::lock_guard<std::mutex> locker(mu);
        task_q.push_back(std::move(t));
    }
    cond.notify_one();

    t1.join();
    std::cout << "result int main thread: " << res.get() << std::endl;
    return 0;
}

/* Summary
 * 3 ways to get a future:
 * - primise::get_future()
 * - packaged_task::get_futrue()
 * - async() returns
 */ 
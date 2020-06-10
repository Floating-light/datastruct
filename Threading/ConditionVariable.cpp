#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <deque>

using namespace std;
std::deque<int> q;
std::mutex mu;
std::condition_variable cond;
// producer
void function_1()
{
    int count = 10;
    while(count > 0)
    {
        std::unique_lock<std::mutex> locker(mu);
        q.push_front(count);
        locker.unlock();
        // cond.notify_all(); 
        cond.notify_one(); // Notify one waiting thread, if there is one.
        std::this_thread::sleep_for(chrono::seconds(1));
        count--;
    }
}

// consumer
void function_2()
{
    int data = 0;
    while(data != 1)
    {
        std::unique_lock<std::mutex> locker(mu);
        //cond.wait(locker); // first unlock and go to sleep, lock it again after wake up .                
        cond.wait(locker, [](){return !q.empty();}); // 通常只想它在队列中有数据的时候被唤醒, 但也可能被自己唤醒, 称为spurious wake.
                                                     // 因此加一个lambda限制唤醒条件.
        data = q.back();
        q.pop_back();
        locker.unlock();
        cout << "t2 got a value from t1: " << data << endl;
    }
}

int main()
{
    std::thread t1(function_1);
    std::thread t2(function_2);

    t1.join();
    t2.join();
    return 0;
}
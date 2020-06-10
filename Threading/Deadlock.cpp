#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <fstream>

using namespace std;

class LogFile
{
private:
    // security reason
    std::mutex _mu;
    // filesystem reason or whatever
    std::mutex _mu2;
    ofstream _f;
public:
    LogFile()
    {
        _f.open("log.txt");
    }
    void shared_print(string id, int value)
    {
        std::lock(_mu, _mu2);
        // adopt_lock tell the locker _mu is already locked, remember to unlock the _mu
        std::lock_guard<std::mutex> locker2(_mu, std::adopt_lock); 
        std::lock_guard<std::mutex> locker(_mu2, std::adopt_lock);

        cout << "From " << id << ": " << value << std::endl;
    }
    void shared_print2(string id, int value)
    {
        // to avoid deadlock
        // make sure every body is locking the mutexes in the same order
        // or use std::lock, that can be use to lock arbitrary number of lockable objects,使用一些避免死锁的算法。
        std::lock(_mu, _mu2);
        std::lock_guard<std::mutex> locker2(_mu, std::adopt_lock);
        std::lock_guard<std::mutex> locker(_mu2, std::adopt_lock);

        cout << "From " << id << ": " << value << std::endl;
    }
    {
        
        std::lock(_mu, _mu2);
        std::lock_guard<std::mutex> locker2(_mu, std::adopt_lock);
        std::lock_guard<std::mutex> locker(_mu2, std::adopt_lock);

        cout << "From " << id << ": " << value << std::endl;
    }
    ~LogFile()
    {
        _f.close();
    }
};

void function_1(LogFile& log)
{
    for(int i = 0; i > -1000; i--)
    log.shared_print2("t1", i);
}

int main()
{
    LogFile log;
    std::thread t1(function_1, std::ref(log));
    for(int i = 0; i < 1000; i++)
        log.shared_print(("From main: "), i);
    t1.join();
    return 0;
}
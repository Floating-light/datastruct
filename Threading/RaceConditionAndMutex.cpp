#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
using namespace std;

std::mutex mu;
class LogFile // thread safe
{
private:
    std::mutex m_mutex;
    ofstream f;
public:
    LogFile(/* args */)
    {
        f.open("log.txt");
    }
    ~LogFile(){}

    void shared_print(string msg, int id)
    {
        // whenever the guard goes out of scope the mu sill always be unlocked
        std::lock_guard<std::mutex> guard(mu); // RAII
        //mu.lock();
        // if there throw a exceptsion，then our mutex will end up being locked forever.
        // so not to use mutex lock and unlock directly, instead use a std::lock_guard<std::mutex>
        std::cout << msg << " : " << id << std::endl;
        //mu.unlock();
    }

    // Never return f to the outside world
    ofstream& getStream(){ return f;}

    // Never pass f as an augument to user provided function
    void processf(void fun(ofstream&))
    {
        fun(f); // fun can print to f witout lock, or close f etc..
    }
};

class stack{
private:
    int *data;
    std::mutex _mu;
public:
    // not thread safe
    void pop(); // pops off the item on top od the stack
    int& top(); // return the item on top
    // ...
};

void fun2(stack& stk)
{
    int v = stk.top();
    stk.pop();
    process(v);
}

void func_1(LogFile& log)
{
    for (size_t i = 0; i < 100; i++)
    {
        log.shared_print("from t1", i);
    }
}

int main()
{
    LogFile log;
    std::thread t1(func_1, std::ref(log));

    for (size_t i = 0; i < 3000; i++)
    {
        //std::cout << "from main : " << i << std::endl;
        log.shared_print("from main", i);
    }
    t1.join();
    return 0;
}
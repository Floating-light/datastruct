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
        std::lock_guard<std::mutex> locker(_mu);
        cout << "From " << id << ": " << value << std::endl;
    }
    void shared_print2(string id, int value)
    {
        std::lock_guard<std::mutex> locker(_mu);
        cout << "From " << id << ": " << value << std::endl;
    }
    ~LogFile()
    {
        _f.close();
    }
};

void function_1(LogFile& log)
{
    for(int i = 0; i > -100; i--)
    log.shared_print("t1", i);
}

int main()
{
    LogFile log;
    std::thread t1(function_1, std::ref(log));
    for(int i = 0; i < 100; i++)
        log.shared_print(("From main: "), i);
    t1.join();
    return 0;
}
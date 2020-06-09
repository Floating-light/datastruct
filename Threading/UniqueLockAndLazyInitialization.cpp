#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <fstream>

using namespace std;

// unique lock
// how to handle lazy initialization in the threading environment

class LogFile
{
private:
    std::mutex _mu;
    std::mutex _mu_open;
    std::once_flag _flag;
    ofstream _f;
public:
    LogFile()
    {
        // 没必要在constructor中打开, 如果从没调用过"shared_print"?
        _f.open("log.txt");

    }
    void shared_print(string id, int value)
    {
        // Lazy Initialization
        // or initialization upon first use Idiom
        // {
        //     std::unique_lock<std::mutex> locker2(_mu_open);
        //     if(!_f.is_open())
        //     {
        //         // std::unique_lock<std::mutex> locker2(_mu_open); // still not thread safe
        //         _f.open("log.txt"); // thread safe
        //     }
        // }
        std::call_once(_flag, [&](){ _f.open("log.txt");}); // file will be opened only once by one thread

        std::lock_guard<std::mutex> locker(_mu);
        cout << "From " << id << ": " << value << std::endl;
    }
    ~LogFile()
    {
        _f.close();
    }
};
#include <iostream>
#include <chrono>

// https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
int main()
{
    auto TimePoint = std::chrono::high_resolution_clock::now();

    auto TimeEnd = std::chrono::high_resolution_clock::now();

    std::chrono::seconds dur = std::chrono::duration_cast<std::chrono::seconds>(TimeEnd - TimePoint);

    std::cout << "Time cost : " << dur.count() << "seconds" << std::endl;
    
}
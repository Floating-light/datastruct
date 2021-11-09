#include <chrono>
#include <iostream>
#include <vector>
#include <string>


// greatest common divisor : 最大公约数,如果数a能被数b整除(a / b)，a就叫做b的倍数，b就叫做a的约数。
// a, b 的最大公约数为, 
int main()
{
    // specific number of ticks over a time unit.
    // first template arg : the type of the ticks .
    // second template argument: defines the unit type of seconds.
    std::chrono::duration<int> twentySeconds(20);
    std::chrono::nanoseconds oneNanosecond(1);
    std::chrono::microseconds oneMicroseconds(1);
    std::chrono::seconds oneSecond(1);



    return 0;
}
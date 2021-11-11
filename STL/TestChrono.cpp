#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

template<typename V, typename R>
std::ostream& operator << (std::ostream& os, const std::chrono::duration<V,R>& dur)
{
    // num : numerator, den : denominator
    os << "[" << dur.count() << " of " << R::num << "/" << R::den << "]";
    return os;
}
// greatest common divisor : 最大公约数,如果数a能被数b整除(a / b)，a就叫做b的倍数，b就叫做a的约数。
// a, b 的最大公约数为, 
int main()
{
    // specific number of ticks over a time unit.
    // first template arg : the type of the ticks .
    // second template argument: defines the unit type of seconds.
    std::chrono::duration<int> twentySeconds(20);
    std::chrono::nanoseconds oneNanosecond(1);
    std::chrono::seconds oneSecond(1);
    
    std::chrono::milliseconds mili(4250);

    // down cast, always ok
    std::chrono::microseconds oneMicroseconds = mili;
    
    // may lost precision, so must explicitly cast .
    // std::chrono::minutes minu = mili;
    std::chrono::seconds minu = std::chrono::duration_cast<std::chrono::seconds>(mili);

    std::cout << mili << std::endl;
    std::cout << oneMicroseconds << std::endl;
    std::cout << minu << std::endl;

    // ----------------------------------------------------------------------------
    std::chrono::duration<double, std::ratio<60, 1>> mu2(0.5);

    // float to integral alway require explicity cast
    // std::chrono::seconds oneMicroseconds = mu2;
    
    std::chrono::seconds minu2 = std::chrono::duration_cast<std::chrono::seconds>(mu2);

    std::cout << mu2 << std::endl;
    std::cout << minu2 << std::endl;

    // ----------------------------------------------------------------------------
    std::cout << std::endl << "------------------------------------------------------------" << std::endl;
    std::chrono::milliseconds ms3(7255042);
    std::chrono::hours hh3 = std::chrono::duration_cast<std::chrono::hours>(ms3);
    std::chrono::minutes mm3 = std::chrono::duration_cast<std::chrono::minutes>(ms3 % std::chrono::hours(1));
    std::chrono::seconds ss3 = std::chrono::duration_cast<std::chrono::seconds>(ms3 % std::chrono::minutes(1));
    std::chrono::milliseconds mss3 = ms3 % std::chrono::seconds(1);

    std::cout << "Raw : " << hh3 << "::" << mm3 << "::" << ss3 << "::" << mss3 << std::endl;
    std::cout << "      " << std::setfill('0') << std::setw(2) << hh3.count() << "::"
                                               << std::setw(2) << mm3.count() << "::"
                                               << std::setw(2) << ss3.count() << "::"
                                               << std::setw(3) << mss3.count() << std::endl;

    return 0;
}
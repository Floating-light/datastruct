#include <chrono>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <format>
#include <type_traits>

template<typename V, typename R>
std::ostream& operator << (std::ostream& os, const std::chrono::duration<V,R>& dur)
{
    // num : numerator, den : denominator
    os << "[" << dur.count() << " of " << R::num << "/" << R::den << "]";
    return os;
}

template <typename C>
void printClockData()
{
    std::cout << "- precision: ";
    if(std::ratio_less_equal<C::period, std::milli>::value)
    {
        typedef typename std::ratio_multiply<C::period, std::kilo>::type TT;
        std::cout << std::fixed << double(TT::num) / TT::den << " milliseconds" << std::endl;
    }
    else{
        std::cout << std::fixed << double(C::period::num) / C::period::den << " seconds" << std::endl;
    }
    std::cout << "- is_steady: " << std::boolalpha << C::is_steady << std::endl;
}

std::string asString(const std::chrono::system_clock::time_point& tp)
{
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::string ts = std::ctime(&t);
    return ts;
}

// std::chrono::clock epoch and tick period, 就是一个Duration, 表示从固定起始点(程序开始时, jan 1, 1970)开始的duration(100 ns为单位)
// std::chrono::time_point : 关联一个Duration 到给定的Clock(本身是一个Duration, 表示一个具体的有名字(意义)起始点),
//  这样, time_point的duration就表示到这个具体的时间点的新的时间点
// 
void TestClockTimePoint()
{
    std::chrono::high_resolution_clock::time_point p = std::chrono::high_resolution_clock::now();
    std::chrono::system_clock::time_point clock = std::chrono::system_clock::now();
    std::chrono::steady_clock::time_point steadyTime = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> tp;
    std::chrono::time_point<std::chrono::steady_clock> steadyTime2 = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock>::min();

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
    // std::chrono::seconds oneMicroseconds = mu2;P
    
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


    printClockData<std::chrono::system_clock>();
    printClockData<std::chrono::steady_clock>();
    printClockData<std::chrono::high_resolution_clock>();
    // std::chrono::sys_days
    std::chrono::time_point<std::chrono::steady_clock> mytp = std::chrono::steady_clock::now();
    // std::cout << mytp << std:endl;
    
    std::chrono::system_clock::time_point ttt;
    // std::cout << asString(ttt) << std::endl;;
    std::cout <<  std::format("{:%F %T}\n", ttt) << std::endl;;

    ttt = std::chrono::system_clock::now();
    // std::cout << asString(ttt) << std::endl;;
    std::cout <<  std::format("{:%F %T}\n", ttt) << std::endl;;


    ttt = std::chrono::system_clock::time_point::max();
    // std::cout << asString(ttt) << std::endl;;
    std::cout <<  std::format("{:%F %T}\n", ttt) << std::endl;;

    ttt = std::chrono::system_clock::time_point::min();
    // std::cout << "============== " << asString(ttt) << std::endl;;
    std::cout <<  std::format("{:%F %T}\n", ttt) << std::endl;;


    // std::cout << std::endl << std::format("{:%F %T}\n", std::chrono::system_clock::now()) << std::endl;;
    return 0;
}
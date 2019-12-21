#include <iostream>
#include <limits>

class Solution {
public:
    int divide(int dividend, int divisor) 
    {
        if(!dividend) return 0;
        int sig ;
        if(dividend > 0 && divisor > 0 || dividend < 0&& divisor < 0) sig = 1;
        else
        {
            sig = -1;
            if(divisor > 0)
                divisor *= -1;
            else
                dividend *= -1;
        }

        int result = 0;
        while( abs(dividend -= divisor) > 0 )
        {
            ++result;
        }
        return result*sig;
    }

    int divide2(int dividend, int divisor)
    {
        if (dividend == 0) return 0;
        if(divisor == -1)
        {
            if(dividend  == std::numeric_limits<int>::min())
                return std::numeric_limits<int>::max();
            else
                return -dividend;
        }
        else if( divisor == 1)
        {
            return dividend;
        }

        int sign = 1;
        if(dividend < 0&&divisor<0)
        {
            sign = 1;
        }
        else 
        if(dividend > 0&&divisor>0)
        {
            dividend = 0-dividend;
            divisor = 0-divisor;
        }
        else
        {
            sign = -1;
            if(dividend > 0) dividend = 0-dividend;
            else
                divisor = 0-divisor;
        }
        int res = 0;
        int div = divisor;
        while(div >= dividend )
        {
            int cur = 1;
            while(div > std::numeric_limits<int>::min()/2 &&(div + div) >= dividend)
            {
                cur += cur;
                div += div;
            }
            dividend = dividend - div;
            div = divisor;
            res += cur;
        }
        return res*sign;
    }
};

int main()
{
    std::cout << " int min: " << std::numeric_limits<int>::min() 
        << " int max: " << std::numeric_limits<int>::max() << std::endl;
    int div = 10;
    int dis = 3;
    std::cout << Solution().divide2(div, dis) << std::endl;
}
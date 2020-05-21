// 50. Pow(x, n)
// tags: Math, Binary Search
#include <iostream>
#include <math.h>

class Solution {
public:
    // 3^5 = 3^(101) = (3^4)^1 * (3^2)^0 * (3^1)^1
    double myPow(double x, int n) {
        double res = 1;
        while( n )
        {
            if( n & 1)
            {
                res *= (n > 0 ? x : 1/x);
            }
            n = n/2; // for negative number not using >> 
            x *= x;
        }
        return res;
    }
};

int main()
{
    std::cout << "result : " << Solution().myPow(2, 0) << std::endl;
}
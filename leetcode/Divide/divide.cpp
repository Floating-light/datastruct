#include <iostream>

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
};
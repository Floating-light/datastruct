// 43. Multiply Strings
// tags: Math, String
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
using namespace std;

class Solution {
public:
    string multiply(string num1, string num2) {
        string res(num1.size() + num2.size(), '0');
        char carry = 0;
        int begin = -1;
        for(auto ritr = num2.crbegin(); ritr != num2.crend(); ++ritr) 
        {
            int curr = ++begin;
            for(auto ritr1 = num1.crbegin(); ritr1 != num1.crend(); ++ritr1)
            {
                int m = ((*ritr) - '0')*((*ritr1) - '0') + carry + (res[curr]-'0');
                res[curr] = m%10 + '0';
                carry = m / 10;
                ++curr;
            }
            res[curr] = carry +'0';
            carry = 0;
        }
        std::reverse(res.begin(), res.end());
        auto beg = std::find_if(res.begin(), res.end(),
        [](const char& c)
        {
            return '0' < c;
        });
        if(beg == res.end())
            return string("0");
        else
            return string(beg, res.end());
    }

    string multiply2(string num1, string num2) {
        string res(num1.size() + num2.size(), '0');
        char carry = 0;
        int begin = -1;
        for(auto ritr = num2.crbegin(); ritr != num2.crend(); ++ritr) 
        {
            int curr = ++begin;
            for(auto ritr1 = num1.crbegin(); ritr1 != num1.crend(); ++ritr1)
            {
                int m = ((*ritr) - '0')*((*ritr1) - '0') + carry + (res[curr]-'0');
                res[curr] = m%10 + '0';
                carry = m / 10;
                ++curr;
            }
            res[curr] = carry +'0';
            carry = 0;
        }
        auto beg = std::find_if(res.crbegin(), res.crend(),
        [](const char& c)
        {
            return '0' < c;
        });
        if(beg == res.crend())
            return string("0");
        else
            return string(beg, res.crend());
    }
};

int main()
{
    string num1("11111111111");
    string num2("234654673567");

    // auto critr = num1.crbegin();
    // while(critr != num1.crend())
    // {
    //     std::cout << *critr << std::endl;
    //     ++critr;
    // }
    // for( auto critr = num1.crbegin(); critr != num1.crend(); ++critr)
    // {
    //     std::cout << (*critr - '0') << std::endl;
    // }
    string result = Solution().multiply2(num1, num2);
    std::cout << result << std::endl;
}
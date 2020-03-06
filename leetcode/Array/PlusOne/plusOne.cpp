// 66. Plus One
// tags: Array
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    vector<int> plusOne(vector<int>& digits)
    {
        int carry = 1;
        for (auto ritr = digits.rbegin(); carry !=0 && ritr != digits.rend(); ++ritr)
        {
            int cur = *ritr + carry;
            *ritr = cur%10;
            carry = cur/10;
        }
        
        if( carry != 0)
        {
            vector<int> res = {1};
            digits.insert(digits.begin(), 1);
        }
        return digits;
    }
};
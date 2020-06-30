#include <iostream>
#include <bitset>
#include <vector>
using namespace std;
// 961. N-Repeated Element in Size 2N Array
// https://leetcode-cn.com/problems/n-repeated-element-in-size-2n-array/
class Solution {
public:
    int repeatedNTimes(vector<int>& A) {
        std::bitset<10001> set;
        for(auto& item : A)
        {
            if(set[item])
            {
                return item;
            }
            set.set(item);
        }
        return -1;
    }
};
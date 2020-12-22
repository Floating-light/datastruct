#include <iostream>
#include <string>
#include <vector>

using namespace std;
/**
 * 1. 正向思考, 导致两个气球从不相邻变成相邻, 戳破时计分不好处理
 * 2. 考虑一个个添加气球, 枚举所有区间, 即先固定了区间
*/
class Solution {
public:
    int slove(int i, int j )
    {
        for(int m = i+1; m < j; ++m)
        {
            
        }
    }
    int maxCoins(vector<int>& nums) 
    {
        if(nums.size() == 0) return 0;
        vals.resize(nums.size() + 2);
        for(int i = 1 ; i <= nums.size() ; i++)
        {
            vals[i] = nums[i];
        }
        vals[0] = vals[nums.size() + 1] = 1;

        slove(0, nums.size() + 1);
    }
private:
    vector<int> vals;
};
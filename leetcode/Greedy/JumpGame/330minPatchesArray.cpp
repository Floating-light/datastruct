#include <vector>

using namespace std;
class Solution {
public:
    int minPatches(vector<int>& nums, int n) 
    {
        int res = 0;
        int index = 0;
        long long x = 1;

        while(x <= n)
        {
            // 添加一个元素nums[index] 后, 扩大了原先[1,x)的覆盖范围到
            // [1,x) 并 [nums[index] + 1, nums[index] + x] 和 nums[index]
            // 即 [nums[index], nums[index] +x ]
            if(index < nums.size() && nums[index] <= x)
            {
                x += nums[index];
                ++index;
            }
            else // x 没有被覆盖
            {
                x <<= 1;
                ++res;
            }
            
        }
        return res;
    }
};
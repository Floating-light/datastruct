// 53. Maximum Subarray
// tags: Array, Divide and Conquer
#include <vector>
#include <limits>
#include <iostream>
using namespace std;
class Solution {
    void findMax(const vector<int>& nums, int head, int& realMax)
    {
        if(head > nums.size()) return ;
        int currentSum = 0;
        for(int i = head; i < nums.size(); ++i)
        {
            currentSum += nums[i];
            if( realMax < currentSum) realMax = currentSum;
        }
        findMax(nums, head+1, realMax);
    }
public:
    int maxSubArray(vector<int>& nums) {
        int realMax = std::numeric_limits<int>::min();
        findMax(nums, 0, realMax);
        return realMax;
    }
};

class Solution2 {
public:
    int maxSubArray(vector<int>& nums) {
        int realMax = std::numeric_limits<int>::min();
        for(int i = 0; i < nums.size(); ++i)
        {
            int currentSum = 0;
            for(int j = i; j < nums.size(); ++j)
            {
                currentSum += nums[j];
                if(currentSum > realMax) realMax = currentSum;
            }
        }
        return realMax;
    }
};

// greed
class Solution3 {
public:
    int maxSubArray(vector<int>& nums)
    {
        if(nums.size() < 1) return std::numeric_limits<int>::min();
        int currentSum = nums[0];
        int realMax = nums[0];
        for ( int i = 1; i < nums.size(); ++i)
        {
            currentSum = max(currentSum + nums[i], nums[i]);
            realMax = max(currentSum, realMax);
        }
        return realMax;
    }
};

//dynamic pragramme
class Solution3 {
public:
    int maxSubArray(vector<int>& nums)
    {
        if(nums.size() < 1) return std::numeric_limits<int>::min();
        int realMax = nums[0];
        for(int i = 1; i < nums.size(); ++i)
        {
            if(nums[i - 1] > 0) nums[i] += nums[i - 1];
            realMax = std::max(realMax, nums[i]);
        }
        return realMax;
    }
};

int main()
{
    vector<int> input {-2,1,-3,4,-1,2,1,-5,4};
    int res = Solution().maxSubArray(input);
    std::cout << res << std::endl;
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// 376. https://leetcode-cn.com/problems/wiggle-subsequence/

std::string IntVectorToString(const std::vector<int>& In)
{
    std::string str = "[ ";
    for(int item : In)
    {
        str += (std::to_string(item) + ", ");
    }
    str.erase(str.end() -2 );
    str += "]";
    return str;
}

// 统计上升下降序列的数目
// 连续的算一个

class Solution
{
public:
    int wiggleMaxLength(std::vector<int>& nums)
    {
        if(nums.size() < 2)
        {
            return nums.size();
        }
        int res = 2;
        if(nums[0] == nums[1])
        {
            res = 1;
        }
        int preDiff = nums[1] - nums[0];

        for(int i = 2; i < nums.size(); ++i)
        {
            const int curDiff = nums[i] - nums[i-1];
            if( curDiff < 0 && preDiff>=0 || curDiff > 0 && preDiff <=0)
            {
                res++;
                preDiff = curDiff;
            }
        }

        return res;

    }
};


int main()
{
    const std::vector<std::vector<int>> Inputs = {   {1,7,4,9,2,5 },
                                                    {1,17,5,10,13,15,10,5,16,8},
                                                    {1,2,3,4,5,6,7,8,9},
                                                    { 0,0},
                                                    { 3,3,3,2,5} };
    for(auto input: Inputs)
    {
        std::cout << "Intput : " << IntVectorToString(input) << std::endl
                  << "Output : " << Solution().wiggleMaxLength(input) << std::endl;
    }
}
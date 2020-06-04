#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

class Solution {
    vector<int> res;
    int product(const vector<int>& nums, int target, int left)
    {
        if (target >= nums.size()) return 1;

        int proRight = product(nums, target + 1, left * nums[target]);
        res[target] = proRight * left ;
        return proRight * nums[target];
    }
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        res.resize(nums.size());
        product(nums, 0, 1);
        return res;
    }

    vector<int> productExceptSelf2(vector<int>& nums) {
        vector<int> res(nums.size());
        int left = 1;
        for(int i = 0; i < nums.size(); ++i)
        {
            res[i] = left;
            left *= nums[i];
        }
        int right = 1;
        for (int i = nums.size() - 1; i >= 0; --i)
        {
            res[i] *= right;
            right *= nums[i];
        }
        return res;
    }

    vector<int> productExceptSelf3(vector<int>& nums) {
        vector<int> res(nums.size(), 1);
        int left = 1;
        int right = 1;
        const int s = nums.size();
        for (int i = 0; i < s; ++i)
        {
            res[i] *= left;
            res[s - i - 1] *= right;
            left *= nums[i];
            right *= nums[s - i - 1];
        }
        return res;
    }
};
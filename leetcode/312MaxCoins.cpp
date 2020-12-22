#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:

    int slove(int i , int j)
    {
        if(i >= j - 1)
        {
            return 0;
        }
        if(cache[i][j] != -1)
        {
            return cache[i][j];
        }
        for(int m = i + 1; m < j; ++m)
        {
            int get = vals[i] * vals[m] * vals[j];
            cache[i][j] = std::max(get + slove(i,m) + slove(m,j), cache[i][j]);
        }
        return cache[i][j];
    }

    int maxCoins(vector<int>& nums) 
    {
        int res = 0;
        if(nums.size() == 0)
            return res;
        vals.resize(nums.size() + 2);
        vals[0] = vals[nums.size() + 1 ] = 1;
        for(int i = 1; i < nums.size() + 1; ++i)
        {
            vals[i] = nums[i-1];
        }
        cache.resize(nums.size() + 2, vector<int>(nums.size() + 2, -1));
        return slove(0, nums.size() + 1);
    }
private:
    vector<int> vals;
    vector<vector<int>> cache;
};
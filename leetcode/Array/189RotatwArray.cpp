#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class Solution {
public:
    void rotate(vector<int>& nums, int k) 
    {
        vector<int> res;
        res.resize(nums.size());
        for(int i = 0; i < nums.size(); ++i)
        {
            res[(i+k)%nums.size()] = nums[i];
        }        
        nums = res;
    }
};

class Solution {
public:
    void rotate(vector<int>& nums, int k) 
    {

    }
};
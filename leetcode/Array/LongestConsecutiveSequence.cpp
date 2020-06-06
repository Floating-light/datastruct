#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

static int n = []() {

    std::ios::sync_with_stdio(false);

    std::cin.tie(nullptr);

    return 0;

}();

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_map<int, bool> n;
        for(auto i : nums)
        {
            n[i] = true;
        }
        int maxTimes = 0;
        for(auto num : nums)
        {
            if(n[num -1])
                continue;
            int t = 1;
            while(n[num + t])
            {
                ++t;
            }
            maxTimes = std::max(t,maxTimes);
        }
        return maxTimes;
    }
};
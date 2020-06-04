#include <algorithm>
#include <vector>
#include <list>
#include <iostream>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int binarySearch(const vector<int>& nums,int aim, size_t lo, size_t hi)
    {
        while (lo < hi)
        {
           
        }
        int mid = (lo + hi)/2;
        return 0;
    }
    vector<vector<int>> pairSums(vector<int>& nums, int target) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        auto itr = nums.begin();
        auto hi = nums.end();
        while (itr != hi  && *itr <= target / 2)
        {
            auto fid = vector<int>::iterator(std::find(vector<int>::reverse_iterator(hi), vector<int>::reverse_iterator(itr + 1), target - *itr).base());
            
            if(fid != (itr+1))
            {
                res.push_back({*itr, *(fid - 1)});
                hi = fid-1;
            }
            ++itr;
        }
        return res;
    }
    vector<vector<int>> pairSums2(vector<int>& nums, int target)
    {
        vector<vector<int>> res;
        unordered_map<int, int> mp;
        for (const int& item : nums) ++mp[item];
        for (const int& i : nums)
        {
            if (--mp[i] >= 0 && --mp[target - i] >= 0)
                res.push_back({ i, target - i });
        }
        return res;
    }
};

//int main()
//{
//    std::vector<int> in = { 5 };
//    auto res = Solution().pairSums(in, 11);
//    
//    res.size();/*auto ritr = std::find(in.rbegin(), in.rend(), 2);
//    vector<int>::iterator itr(ritr.base());
//    vector<int>::reverse_iterator rritr(itr);
//    std::cout << "ritr: " << *ritr << std::endl
//        << "rritr: " << *rritr << std::endl
//        << "itr: " << *itr << std::endl;*/
//}
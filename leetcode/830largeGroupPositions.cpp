#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Solution {
public:
    vector<vector<int>> largeGroupPositions(string s) 
    {
        vector<vector<int>> res;
        res.reserve(s.size()/3 + 1);
        int l = 0,r = 1;
        while(r <= s.size())
        {
            if(r == s.size() || s[r] != s[l])
            {
                if(r - l >= 3)
                {
                    res.emplace_back(vector<int>({l, r - 1}));
                }
                l = r;
            }
            ++r;
        }
        return res;
    }
};
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        if(s.size() == 0) return 0;
        int maxLength = 1;
        int lo = 0;
        for(int i = 1; i <s.size(); ++i )
        {

            auto findedPos = find(s.cbegin() + lo , s.cbegin() + i, s[i]);
            if(findedPos != s.cbegin() + i)
            {
                maxLength = std::max(maxLength, i - lo);
                lo = findedPos - s.cbegin()  + 1;
            }
        }
        maxLength = std::max(maxLength, static_cast<int>(s.size() - lo));
        return maxLength;
    }
};

class Solution
{
    public:
    int lengthOfLongestSubstring(string s) {
        
        unordered_set<char> occ;
        int ik = -1;
        int res = 0;
        for(int i = 0; i < s.size(); ++i)
        {
            if(i != 0)
            {
                occ.erase(s[i-1]);
            }
            while(ik +1 < s.size() && !occ.count(s[ik+1]))
            {
                occ.insert(s[ik+1]);
                ++ik;
            }
            res = std::max(res, ik - i + 1);
        }
        return res;
    }
};
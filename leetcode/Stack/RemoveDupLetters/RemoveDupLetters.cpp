// 316. Remove Duplicate Letters
// tags: Stack, Greedy
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    string removeDuplicateLetters(string s) {
        vector<int> cache(26, -1);
        vector<bool> res(s.size(), false);

        for(int i = 0; i < s.size(); ++i)
        {
            int letterIndex = s[i] - 97;
            if(cache[letterIndex] == -1)
            {
                cache[letterIndex] = i;
                res[i] = true;
            }
            else // previous have
            {
                if(s[cache[letterIndex]+1] <  s[cache[letterIndex]])
                {
                    res[cache[letterIndex]] = false;
                    cache[letterIndex] = i;
                    res[i] = true;
                }
                else
                    res[i] = false;
            } 
        }
        string result;
        for(int i = 0; i < res.size(); ++i)
        {
            if(res[i])
            {
                result.push_back(s[i]);
            }
        }
        return result;
    }
};

class Solution2 {
public:
    string removeDuplicateLetters(string s) {
        string stk;
        size_t i = 0;
        for(size_t i = 0;i < s.size(); ++i)
        {
            if(stk.find(s[i]) != string::npos) continue;
            while(!stk.empty()&& stk.back() > s[i]&& 
                s.find(stk.back(), i) != string::npos)
                stk.pop_back();
            stk.push_back(s[i]);
        }
        return stk;
    }
};

int main()
{
    string res = Solution2().removeDuplicateLetters("abacb");
    std::cout << res << std::endl;
}

/*
Example 1:

Input: "bcabc"
Output: "abc"

Example 2:

Input: "cbacdcbc"
Output: "acdb"
*/
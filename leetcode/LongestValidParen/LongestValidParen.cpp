#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Solution {
public:
    int longestValidParentheses(string s) {
        if(s.size() < 2) return 0;
        vector<int> stk_left;
        vector<int> stk_right;
        stk_right.push_back(-1);

        for(int i = 0; i < s.size(); i++) 
        {
            if(s[i] == '(')
                stk_left.push_back(i);
            else // ')'
                if(stk_left.empty())   
                    stk_right.push_back(i);  
                else
                    stk_left.pop_back();
        }
        stk_left.push_back(s.size());
        stk_right.insert(stk_right.end(), stk_left.begin(), stk_left.end());
        int maxLength = 0;
        for(int i = 1; i < stk_right.size(); i++)
        {
            const int& cur = stk_right[i] - stk_right[i - 1] - 1;
            maxLength = cur > maxLength ? cur : maxLength;
        }
        return maxLength;
    }

    int longestValidParentheses2(string s) {
        if(s.size() < 2) return 0;
        vector<int> stk;
        stk.push_back(-1);
        int leftNum = 1;
        for(int i = 0; i < s.size(); i++) 
        {
            if(s[i] == '(')
                stk.push_back(i);
            else // ')'
                if(stk.size() == leftNum)
                {
                    stk.push_back(i);  
                    ++leftNum;
                }
                else
                    stk.pop_back();
        }
        stk.push_back(s.size());
        int maxLength = 0;
        for(int i = 1; i < stk.size(); i++)
        {
            const int& cur = stk[i] - stk[i - 1] - 1;
            maxLength = cur > maxLength ? cur : maxLength;
        }
        return maxLength;
    }
};

int main()
{
    vector<string> input = 
    {
        "(()",
        ")()())",
        ")))(((",
        "()(()))()()()()()"
    };

    for( string item : input)
    {
        cout << item << ": ";
        cout << Solution().longestValidParentheses2(item) << endl;
    }
    

}
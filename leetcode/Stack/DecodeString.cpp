#include <sstream>
#include <string>
#include <iostream>
#include <stack>

using namespace std;

class Solution {
    //题目保证数字不会出现在末尾
    int readNum(string::const_iterator& itr)
    {
        int i = 0;
        while(isdigit(*itr))
        {
            i = i * 10 + (*itr-48);
            ++itr;
        }
        return i;
    }
public:
    string decodeString(string s) {
        string res;
        stack<int> times;
        for(auto itr = s.cbegin(); itr != s.cend();)
        {
            if(isdigit(*itr)) 
            {
                times.push(readNum(itr));
                continue;
            }
            else if(*itr == ']') 
            {
                // 题目保证前一定有数字[]
                int curTimes = times.top();
                times.pop();

                const int ind = res.find_last_of('[');
                string temp = res.substr(ind+1);
                res.erase(ind);
                while(curTimes != 0) 
                {
                    res += temp;
                    --curTimes;
                }
            }
            else
            {
                res.push_back(*itr);
            }
            ++itr;
        }
        return res;
    }
};

/* s = "3[a]2[bc]", 返回 "aaabcbc".
s = "3[a2[c]]", 返回 "accaccacc".
s = "2[abc]3[cd]ef", 返回 "abcabccdcdcdef". */

int main()
{
    //string s("3[a2[c]]"); // "accaccacc"
    //string s("2[abc]3[cd]ef"); // "abcabccdcdcdef"
    string s("aaa3[a]2[bc]bbb"); // "abcabccdcdcdef"


    cout << Solution().decodeString(s) << endl;
}
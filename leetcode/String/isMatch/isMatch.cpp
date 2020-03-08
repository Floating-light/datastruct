// 10. Regular Expression Matching
// tags: String, Dynamic Programming, Backtracking
#include <iostream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

class Solution
{
    size_t sSize;
    size_t pSize;
public:
    bool isMatch(string s, string p)
    {
        int current = 0;
        bool arbit = false;    

        for(int i = 0; i < p.size() && current < s.size(); ++i)
        {
        if(p[i] == '.')
            ++current;
        else if(p[i] == '*')
        {
            if(i == 0)
                continue;
            if(p[i -1 ] == '.')
            {
                arbit = true;
                --current;
            }
            while(current < s.size() && s[current] == p[i - 1])
                ++current;
        }
        else
        {
            if(!arbit)
            {
                if(p[i] != s[current])
                return false;
            }
            else
            {
                while(current < s.size() && p[i] != s[current])
                    ++current;
            }
        }
        }
        return true;
    }

	// 40ms
    bool isMatch2(const string& s,const  string& p)
    {
        sSize = s.size();
        pSize = p.size();
        return doMatch(s, 0, p, 0);
    }
    // 考察s串从sIndex开始的子串与模式串p中从pIndex开始的子串是否匹配（均包括开始位置直到末尾）。
    bool doMatch(const string& s, int sIndex, const string& p, int pIndex)
    {
		if (pIndex >= pSize) return sIndex >= sSize;

		bool currentMatch = sIndex < sSize && (s[sIndex] == p[pIndex] || p[pIndex] == '.');

        if(pIndex + 1 < pSize && p[pIndex + 1] == '*')
        {
			// *匹配0个字符(无论当前字符匹不匹配这都有可能s = abbc, p = ab*bbc) || 当前字符匹配并尝试s中的下一个字符
			return doMatch(s, sIndex, p, pIndex + 2) || (currentMatch && doMatch(s, sIndex + 1, p, pIndex));
        }
        else // 没有*
        {
			// 正常匹配，包括了.
            // 匹配上就考察下一个，否则 return false
			return currentMatch && doMatch(s, sIndex + 1, p, pIndex + 1);
        }
        return false;
    }
};

// 4 ms
class Solution2 
{
    vector<vector<int>> mem;
public:
    bool isMatch(const string& s,const  string& p)
    {
        mem = vector<vector<int>>(s.size() + 1, vector<int>(p.size() + 1, -1));
        return doMatch(s, 0, p, 0);
    }
    // 对于sIndex和pIndex， 仅当它们之后的字串匹配时它们才匹配
    bool doMatch(const string& s,int sIndex, const string& p, int pIndex)
    {
		cout << "\n" << "sIndex: " << sIndex << ", pIndex: " << pIndex ;
		if (mem[sIndex][pIndex] != -1)
		{
			return mem[sIndex][pIndex];
		}
		bool res;
        if(pIndex >= p.size()) res = (sIndex >= s.size());
        else
        {
            bool currentMatch = (sIndex < s.size() && (s[sIndex] == p[pIndex] || p[pIndex] == '.'));
            if(pIndex+1 < p.size() && p[pIndex+1] =='*')
            {
                res = doMatch(s, sIndex, p, pIndex+2) || // 忽略x*
                    currentMatch&&doMatch(s, sIndex + 1, p, pIndex); // 如果*前的字符匹配，则将s前进一位
            }
            else
            {
                res = currentMatch&&doMatch(s, sIndex+1, p, pIndex + 1);
            }
        }
        mem[sIndex][pIndex] = res;
        return res;
    }
};

// 8ms
class Solution3
{
public:

	bool isMatch(string& s, string& p)
	{
		if (p.empty()) return s.empty();
		s = "$" + s;
		p = "$" + p;
		vector<vector<bool>> dp(s.size() + 1, vector<bool>(p.size() + 1, false));
		dp[0][0] = true;
		for (size_t i = 1; i <= s.size(); ++i)
		{
			for (size_t j = 1; j <= p.size(); ++j)
			{
				if (s[i-1] == p[j-1] || p[j-1] == '.')
				{
					dp[i][j] = dp[i - 1][j - 1];
				}
				else if(p[j-1] == '*')
				{
					if (p[j - 2] != s[i-1] && p[j-2] != '.') dp[i][j] = dp[i][j - 2];
					else dp[i][j] = dp[i - 1][j] || dp[i][j - 2];
				}
			}
		}
		return dp[s.size()][p.size()];
	}
};



int main()
{
    
    string s("mississippi");
    string p("mis*is*p*.");
    regex_match(s, regex(p));
    vector<pair<string, string>> inputs = {
        {"aa", "a"},
        {"aa", "a*"},
        {"ab", ".*"},
        {"aab", "c*a*b"},
        {"mississippi", "mis*is*p*."},
        {"", ".*"},
        {"", ""},
        {"", "."},
        {"aaaaaaaaaaaaab","a*a*a*a*a*a*a*a*a*a*c"},
        {"ab","a*ab"},
        {"bbbba", ".*a*a"}
    };
    Solution2 solu;
    for(int i = 0; i < inputs.size(); ++i )
    {
        std::cout << "case "<< i << ":" << std::endl
            << "s: "<< inputs[i].first << std::endl
            << "p: "<< inputs[i].second << std::endl
            //<< "is match: " << regex_match(inputs[i].first, regex(inputs[i].second))
            << "is match: \n" << solu.isMatch(inputs[i].first,inputs[i].second)
            << std::endl << std::endl;
    }
}
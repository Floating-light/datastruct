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
    bool isMatch2(string s, string p)
    {
        sSize = s.size();
        pSize = p.size();

        //if(s.empty() || p.empty()) return false;
        return doMatch(s, 0, p, 0);
    }
    bool doMatch(string s, int sIndex, string p, int pIndex)
    {
        if(sIndex == sSize && pIndex == pSize) return true;
        if(sIndex < sSize && pIndex == pSize) return false;
        if(pIndex + 1 < pSize && p[pIndex + 1] == '*')
        {
            if(sIndex < sSize && s[sIndex] == p[pIndex] || sIndex < sSize && p[pIndex] == '.')
            {
                return doMatch(s, sIndex + 1, p, pIndex) || doMatch(s, sIndex + 1, p, pIndex + 2) 
                || doMatch(s, sIndex, p, pIndex + 2);
            } 
            else
            {
                return doMatch(s, sIndex, p, pIndex+2);
            }
        }
        else
        {
            if( sIndex < sSize && p[pIndex] == s[sIndex] || sIndex < sSize && p[pIndex] == '.') return doMatch(s, sIndex + 1, p, pIndex + 1);
            else return false;
        }
        return false;
    }
};
class Solution2 
{
    vector<vector<int>> mem;
public:
    bool isMatch(string s, string p)
    {
        mem = vector<vector<int>>(s.size() + 1, vector<int>(p.size() + 1, -1));

        //if(s.empty() || p.empty()) return false;
        return doMatch(s, 0, p, 0);
    }
    // 对于sIndex和pIndex， 仅当它们之后的字串匹配时它们才匹配
    bool doMatch(const string& s,int sIndex, const string& p, int pIndex)
    {
        if(mem[sIndex][pIndex] != -1) return mem[sIndex][pIndex];
        bool ans;
        if(pIndex == p.size()) ans = (sIndex == s.size());
        else
        {
            bool firstMatch = (sIndex < s.size() && (s[sIndex] == p[pIndex] || p[pIndex] == '.'));
            if(pIndex+1 < p.size() && p[pIndex+1] =='*')
            {
                ans = doMatch(s, sIndex, p, pIndex+2) || // 忽略x*
                    firstMatch&&doMatch(s, sIndex + 1, p, pIndex); // 如果*前的字符匹配，则将s前进一位
            }
            else
            {
                ans = firstMatch&&doMatch(s, sIndex+1, p, pIndex + 1);
            }
        }
        mem[sIndex][pIndex] = ans;
        return mem[sIndex][pIndex];
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
            << "is match: " << regex_match(inputs[i].first, regex(inputs[i].second))
            //<< "is match: " << solu.isMatch2(inputs[i].first,inputs[i].second)
            << std::endl << std::endl;
    }
}
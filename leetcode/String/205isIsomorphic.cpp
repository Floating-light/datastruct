#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// 用两个map更简洁
class Solution {
public:
    bool isIsomorphic(string s, string t) {
        std::unordered_map<char, char> m;
        std::unordered_set<char> set;
        for(int i = 0; i < s.size(); ++i)
        {
            auto itr = m.find(s[i]);
            if(itr == m.end())
            {
                if(set.count(t[i]) != 0)
                {
                    return false;
                }
                set.emplace(t[i]);
                m.emplace(s[i], t[i]);
            }
            else
            {
                if(t[i] != itr->second)
                {
                    return false;
                }
            }
        }
        return true;
    }
};

// 290 单词规律
class Solution {
public:
    bool wordPattern(string pattern, string s) {
        std::unordered_map<char, std::string> p2word;
        std::unordered_map<std::string, char> word2p;

        int p = 0;
        std::string currentWord;
        for(int i = 0; i <= s.size(); ++i)
        {
            if(i<s.size() && s[i] != ' ')
            {
                currentWord.push_back(s[i]);
            }
            else
            {
                if(p < pattern.size())
                {
                    if(p2word.count(pattern[p]) == 0)
                    {
                        p2word[pattern[p]] = currentWord;
                    }
                    else if(p2word[pattern[p]] != currentWord)
                    {
                        return false;
                    }
                    
                    if(word2p.count(currentWord) == 0)
                    {
                        word2p[currentWord] = pattern[p];
                    }
                    else if(word2p[currentWord] != pattern[p])
                    {
                        return false;
                    }
                    ++p;
                }
                else 
                {
                    return false;
                }
                currentWord.clear();
            }
             
        } 

        return p == pattern.size() ;
    }
};

int main()
{
    return -1;
}
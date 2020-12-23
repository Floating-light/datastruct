#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <array>
using namespace std;

class Solution {
public:
    int firstUniqChar(string s) {
        std::unordered_map<char,int> m;
        for(char c: s) ++m[c];
        for(int i = 0; i < s.size(); ++i) 
        {
            if(m[s[i]] == 1)
            {
                return i;
            }
        }
        return -1;
    }
};

class Solution {
public:
    int firstUniqChar(string s) {
        std::vector<int> m;
        m.resize(26,0 );
        for(char c: s) ++m[c - 'a'];
        for(int i = 0; i < s.size(); ++i) 
        {
            if(m[s[i] - 'a'] == 1)
            {
                return i;
            }
        }
        return -1;
    }
};

class Solution {
public:
    int firstUniqChar(string s) {
        std::array<int, 26> m;
        m.fill(0);
        for(char c: s) ++m[c - 'a'];
        for(int i = 0; i < s.size(); ++i) 
        {
            if(m[s[i] - 'a'] == 1)
            {
                return i;
            }
        }
        return -1;
    }
};

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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

int main()
{
    string s("mississippi");
    string p("mis*is*p*.");
    vector<pair<string, string>> inputs = {
        {"aa", "a"},
        {"aa", "a*"},
        {"ab", ".*"},
        {"aab", "c*a*b"},
        {"mississippi", "mis*is*p*."},
    };
    for(int i = 0; i < inputs.size(); ++i )
    {
        std::cout << "case "<< i << ":" << std::endl
            << "s: "<< inputs[i].first << std::endl
            << "p: "<< inputs[i].second << std::endl
            << "is match: " << isMatch(inputs[i].first,inputs[i].second)
            << std::endl << std::endl;
    }
}
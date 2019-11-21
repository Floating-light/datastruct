#include <iostream>
#include <string>

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

    if(isMatch(s, p))
    {
        std::cout << "true" << std::endl;
    }
    else
    {
        std::cout << "false" << std::endl;
    }
}
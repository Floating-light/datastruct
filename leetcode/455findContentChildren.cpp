#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findContentChildren(vector<int>& g, vector<int>& s) 
    {
        sort(g.begin(), g.end());
        sort(s.begin(), s.end());
        int j = 0;
        int totalChildren = 0;
        for (int i = 0; i < g.size() ; ++i) 
        {
            while(j < s.size() && s[j] < g[i])
            {
                ++j;
            }
            if(j < s.size() )
            {
                ++totalChildren;
                ++j;
            }
            else
            {
                break;
            }
        }
        return totalChildren;
    }
};

int main()
{
    int _Umy_name_test = 0;
    std::cout << _Umy_name_test << std::endl;
}
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
    bool isOverLapping(const vector<int>& s, const int& col)
    {
        const int row = s.size();
        for( int i = 0; i < s.size(); ++i)
        {
            if( col == s[i] || row - i == col - s[i] || row - i == s[i] - col)
                return true;
        }
        return false;
    }
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<int> solu;
        vector<vector<string>> result;
        int column = 0;
        while(!solu.empty() || column != n)
        {
            if(solu.size() == n || column == n)
            {
                column = solu.back() + 1;
                solu.pop_back();
            }
            else
            {
                while(column < n && isOverLapping(solu, column))
                {
                    ++column;
                }
                if(column < n)
                {
                    solu.push_back(column);
                    if(solu.size() == n)
                    {
                        vector<string> res;
                        for( int i = 0; i < solu.size(); ++i)
                        {
                            string t(n, '.');
                            t[solu[i]] = 'Q';
                            res.push_back(t);
                        }
                        result.push_back(res);
                        // for_each(solu.cbegin(), solu.cend(),
                        // [](const int& item)
                        // {
                        //     cout << item << ", ";
                        // });
                        // cout << endl;
                    }
                    column = 0;
                }
            }
        }
        return result;
    }
};

int main()
{
    vector<vector<string>> res = Solution().solveNQueens(0);
    for (size_t i = 0; i < res.size(); ++i)
    {
        std::cout << "--------------------------------------" << std::endl;
        for( int j = 0; j < res[i].size(); ++j)
        {
            std::cout << " " << res[i][j] << std::endl;
        }
        std::cout << std::endl;
    }

    
}
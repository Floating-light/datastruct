#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Solution 
{
public:
bool IsInRange(int t, int min, int max)
{
    return t >= min && t< max;
}
    void remove(vector<vector<char>>& v, int i, int j )
    {
        
        if(IsInRange(i, 0, v.size()) && IsInRange(j, 0, v[i].size())  && v[i][j] == '1')
        {
            v[i][j] = '0';

            remove(v, i, j+1);
            remove(v, i + 1, j );
            remove(v, i, j - 1);
            remove(v, i -1 , j);
        }
    }
    int numIslands(vector<vector<char>>& grid)
    {
        int res = 0;
        for ( int i = 0; i < grid.size(); ++i)
        {
            for(int j = 0; j < grid[i].size(); ++j)
            {
                if(grid[i][j] == '1')
                {
                    ++res;
                    remove(grid, i, j );
                }
            }
        }
        return res;
    }
};

int main()
{
    std::shared_ptr<int> sp;
    {
        std::weak_ptr<int> wp(sp);  // 持有 但 不影响引用计数
        if(wp.expired())
        {
            std::shared_ptr<int> mysp = wp.lock();
            std::cout << "My integer : " << *mysp << std::endl;
        }
    }
    
}
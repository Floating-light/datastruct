#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;

// 463. Island Perimeter
// https://leetcode-cn.com/problems/island-perimeter/

class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        if(grid.empty()) return 0;
        int c = 0;
        bool water = true;
        std::unordered_map<int, bool> isLand;
        grid.push_back(std::vector<int>(grid[0].size(), 0));
        for(int i = 0; i < grid.size(); ++i)
        {
            for(int j = 0; j <= grid[i].size(); ++j)
            {
                const bool isLandCurr = (j == grid[i].size()) ? false : grid[i][j];
                if(isLandCurr && water)
                {
                    ++c;
                    water = false;
                }
                else if(!isLandCurr && !water)
                {
                    ++c;
                    water = true;
                }
                if(j == grid[i].size()) continue;
                if(!isLand[j] && isLandCurr)
                {
                    ++c;
                    isLand[j] = true;
                }else if(isLand[j] && !isLandCurr)
                {
                    ++c;
                    isLand[j] = false;
                }
            }
            water = true;
        }
        return c;
    }
};

// 仔细分析题意
// 在上面的解法中, 处理边界时的解决不够优雅
// 先加后减
class Solution {
public:
    int islandPerimeter(vector<vector<int>>& grid) {
        int c = 0;
        for(int i = 0; i < grid.size(); ++i)
        {
            for(int j = 0; j < grid[i].size(); ++j)
            {
                if(grid[i][j])
                {
                    c+=4;
                    if(j > 0 && grid[i][j - 1] == 1 ) c-=2;
                    if(i > 0 && grid[i-1][j] == 1) c-=2;
                }
                
            }
        }
        return c;
    }
};
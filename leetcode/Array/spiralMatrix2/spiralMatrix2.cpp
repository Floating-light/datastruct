// 59. Spiral Matrix II
// tags: Array
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> res(n, vector<int>(n, 0));
        int h = 0, k = n;
        int num = 0;
        int i , j = 0;
        while(h < k)
        {
            for(i = h,j = h; j < k; ++j) 
                res[i][j] = ++num;
            for(i = h + 1, j= k-1; i < k; ++i) 
                res[i][j] = ++num;
            for(i = k - 1,j = k-2; j>=h; --j) 
                res[i][j] = ++num;
            for(i = k-2, j = h; i >h; --i) 
                res[i][j] = ++num;
            ++h;
            --k;
        }     
        return res;       
    }
};

int main()
{
    vector<vector<int>> res = Solution().generateMatrix(0);
    for(auto item: res)
    {

        for(auto i: item)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }
}
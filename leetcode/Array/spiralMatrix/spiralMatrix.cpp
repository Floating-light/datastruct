// 54. Spiral Matrix
// tags: Array
#include <vector>
#include <iostream>

using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> result;
        if( matrix.size() == 0 ) return result;
        int h = 0 ,m = 0;
        int k = matrix[0].size();
        int n = matrix.size();
        while(h < k && m < n)
        {
            for( int i = h; i < k; ++i)
                result.push_back(matrix[m][i]);
            for( int i = m + 1; i < n; ++i)
                result.push_back(matrix[i][k-1]);
            if(n-1 != m)
                for(int i = k - 2; i >= h ; --i)
                    result.push_back(matrix[n - 1][i]);
            if(k-1 != h)
                for(int i = n - 2; i > m; --i)
                    result.push_back(matrix[i][h]);
            ++h;
            ++m;
            --k;
            --n;
        }
        return result;
    }
};

int main()
{
    vector<vector<int>> input = {
        {1,2,3,4,5,6}
    };
    vector<vector<int>> input2 = {
        {1},
        {2},
        {3},
        {4},
        {5},
    };
    vector<vector<int>> input4 = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };
    vector<vector<int>> input3 = {
        { 1, 2 },
        { 4, 5 }
    };
    vector<vector<int>> input5 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9,10,11,12}
    };
    vector<int> res = Solution().spiralOrder(input5);
    for( int it : res)
    {
        std::cout << it << ", " << std::endl;
    }
    std::cout << "endl" << std::endl;
}
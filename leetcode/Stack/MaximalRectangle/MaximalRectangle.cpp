#include <vector>
#include <stack>
#include <iostream>

using namespace std;

class Solution {
    int maxRectangle(vector<int>& heights)
    {
        stack<int> SIndex;
        int maximal = 0;
        heights.push_back(0);
        const int size = heights.size();
        for( int t = 0; t < size; ++t)
        {
            while( !SIndex.empty() && heights[SIndex.top()] > heights[t])
            {
                const int mid = SIndex.top();
                SIndex.pop();
                const int s = SIndex.empty() ? 0: SIndex.top() + 1;
                maximal = max(maximal, heights[mid]*(t-s));
            }
            SIndex.push(t);
        }

        // while( !SIndex.empty())
        // {
        //     const int mid = SIndex.top();
        //     SIndex.pop();
        //     const int s = SIndex.empty() ? 0: SIndex.top() + 1;
        //     maximal = max(maximal, heights[mid]*(size-s));
        // }
        return maximal;
    }
public:
    int maximalRectangle(vector<vector<char>>& matrix) {
        const int row = matrix.size();
        const int column = row ? matrix[0].size() : 0;
        int maxArea = 0;
        vector<vector<int>> cache(row, vector<int>(column, 0));
        vector<int> curRow(column, 0);
        for( int i = 0; i < row; ++i )
        {
            for( int j = 0; j < column; ++j )
            {
                // cache[i][j] = (matrix[i][j] == '1' )? 1 +cache[i-1][j]: 0;
                curRow[j] = (matrix[i][j] == '1' )? 1 +curRow[j]: 0;
                // cache[i][j] = cache[i-1][j]*temp + temp;
                // cache[i][j] = (matrix[i][j] == '1' ? 1 : 0) + cache[i-1][j];
            }
            maxArea = max(maxArea, maxRectangle(curRow));
        }
        // std::cout << std::endl;
        // for( int i = 0; i <cache.size(); ++i )
        // {
        //     for( int j = 0; j < cache[i].size(); ++j )
        //     {
        //         std::cout << cache[i][j] <<", ";
        //     }
        //     std::cout<< std::endl;
        // }
        return maxArea;
    }
};

class Solution {
public:
    int largestRectangleArea(vector<int>& heights)
    {
        int res = 0;
        stack<int> M;
        heights.push_back(-1);
        for(int i = 0; i < heights.size() ; )
        {
            if(M.empty() || heights[i] >= heights[M.top()])
            {
                M.push(i);
                ++i;
            }
            else
            {
                if(M.empty() ) break;
                int t = heights[M.top()];
                M.pop();
                int lo = M.empty() ? -1 : M.top();
                res = std::max(res, t* (i - lo -1));
            }
        }
        return res;
    }
    int maximalRectangle(vector<vector<char>>& matrix) {
        if(matrix.size() == 0)
        {
            return 0;
        }
        int res = 0;
        vector<int> h(matrix[0].size(),0);
        for(int i = 0; i < matrix.size(); ++i)
        {
            for(int j = 0; j < matrix[i].size(); ++j)
            {
                if( matrix[i][j] == '1')
                {
                    h[j] += 1;
                }
                else 
                {
                    h[j] = 0;
                }
            }
            res = std::max(res, largestRectangleArea(h));
        }
        return res;
    }

};

int main()

{
    vector<vector<char>> input
    {
        {'1','1','0','0','0'},
        {'1','1','1','1','1'},
        {'1','1','1','1', '1'},
        {'1','0','1','1', '1'}
    };
    // vector<vector<char>> input
    // {
    //     {'1'}
    // };
    int result = Solution().maximalRectangle(input);
    std::cout << "------------>result:\n" << result << std::endl;
}
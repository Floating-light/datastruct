// 48. Rotate Image
// tags: Array
#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        const int n = matrix.size();
        for( int i = 0; i <n; i++ ) 
        {
            for( int j = i; j <n; j ++)
            {
                swap(matrix[i][j], matrix[j][i]);
            }
        }
        for( int i = 0; i <n; i++ ) 
        {
            for( int j = 0; j <n/2; j ++)
            {
                swap(matrix[i][j], matrix[i][n - 1 - j]);
            }
        }
    }

    void rotate2(vector<vector<int>>& matrix)
    {
        const int n = matrix.size();
        for(int i = 0; i < (n+1)/2; ++i)
        {
            for( int j = 0; j < n/2; ++j)
            {
                const int temp = matrix[i][j];
                matrix[i][j] = matrix[n-j-1][i];
                matrix[n-j-1][i] = matrix[n-i-1][n-j-1];
                matrix[n-i-1][n-j-1] = matrix[j][n-i-1];
                matrix[j][n-i-1] = temp;
            }
        }
    }
};

int main()

{
    vector<vector<int>> input2 = 
    {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    vector<vector<int>> input = 
    {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15,16}
    };
    Solution().rotate2(input);
    for (int i = 0; i < input.size(); ++i)
    {
        for (int j = 0; j < input[i].size(); ++j)
        {
            std::cout << input[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Solution {
public:
    vector<vector<int>> transpose(vector<vector<int>>& A) {
        vector<vector<int>> 
        const int N = A.size();
        for(int i = 0; i < N; ++i)
        {
            for(int j = i+1; j < N; ++j)
            {
                swap(A[i][j], A[j][i]);
            }
        }
        return A;
    }
};
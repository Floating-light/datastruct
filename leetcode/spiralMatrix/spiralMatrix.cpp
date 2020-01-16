#include <vector>

using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> result;
        if( matrix.size() == 0 ) return result;
        int h ,m = 0;
        int k = matrix[0].size();
        int n = matrix.size();
        for( int i = h; i < k; ++i)
            result.push_back(matrix[m][i]);
        for( int i = m; m < n; ++i)
            result.push_back(matrix[k-1][i]);
        for();

    }
};
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findLength(vector<int>& A, vector<int>& B) {

    }
    int maxLengthFrom(const vector<int>& A, int from, const vector<int>& B)
    {
        int MaxLengthFromFrom = -1;
        for(int i = from; i < A.size(); ++i)
        {
            int iNow = i;
            int MaxLengthNow = -1;
            for(int j = 0; j < B.size(); j++)
            {
                if(A[iNow] == B[j])
                {
                    ++iNow;
                }
                else
                {
                    MaxLengthNow = std::max(iNow - i, MaxLengthNow);
                    iNow= i;
                }
            }
        }
    }

};
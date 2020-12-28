#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Solution {
public:
    int candy(vector<int>& ratings) 
    {
        vector<int> left(ratings.size(), 1);
        for(int i = 1; i < ratings.size(); i++)
        {
            if(ratings[i] > ratings[i-1])
            {
                left[i] = left[i-1] + 1;
            }
        }
        int totalCandy = 0;
        int curr = 0;
        for(int j = ratings.size() - 1; j >= 0; j--)
        {
            if(j != ratings.size() - 1 && ratings[j] > ratings[j + 1])
            {
                curr += 1;
            }
            else 
            {
                curr = 1;
            }
            totalCandy += std::max(left[j], curr);
        }
        return totalCandy;
    }
};

class Solution {
public:
    int candy(vector<int>& ratings) 
    {
        int increaseSeqLen = 1;
        int decreasingSeqLen = 0;
        int TotalCandy = 1;
        int preCandy = 1;
        for (int i = 1; i < ratings.size(); i++)
        {
            if(ratings[i] >= ratings[i-1])
            {
                decreasingSeqLen = 0;
                preCandy = ratings[i] == ratings[i-1] ? 1 : preCandy+1;
                
                TotalCandy += preCandy;
                increaseSeqLen = preCandy;
            }
            else 
            {
                ++decreasingSeqLen;
                if(decreasingSeqLen == increaseSeqLen)
                {
                    ++decreasingSeqLen;
                }
                TotalCandy += decreasingSeqLen;
                preCandy = 1;
            }
        }
        return TotalCandy;
    }
};
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int lastStoneWeight(vector<int>& stones) 
    {
        while(!stones.empty())
        {
            if(stones.size() == 1)
            {
                return stones.back();
            }
            std::sort(stones.begin(), stones.end());
            const int bigger = stones.back();
            stones.pop_back();
            const int div =bigger -  stones.back();
            stones.pop_back();
            if(div != 0)
            {
                stones.push_back(div);
            }

        }
        return 0;
    }
};
#include <vector>
#include <iostream>
#include <stack>
using namespace std;

class Solution {
public:
    int trap(vector<int>& height) {
        if( height.empty() ) return 0;
        height.push_back(0);
        std::pair<int, int> peak = {-1 , -1};
        std::pair<int, int> pre = {0 , height[0]};
        int cacheAre = 0;
        int totalRaining = 0;
        bool findSecondPeak = true;
        for( int i = 1; i < height.size(); ++i)
        {
            int curHeight = height[i];
            if( curHeight >= pre.second) //ascending
            {
                findSecondPeak = true;
                cacheAre += curHeight;
            }
            else // pre is the peak
            {
                if(findSecondPeak)
                {
                    if(peak.first != -1)
                    {
                        totalRaining += (pre.first - peak.first - 1)*min(pre.second, peak.second) - cacheAre + pre.second;
                    }
                    cacheAre = curHeight;
                    peak = pre;
                    findSecondPeak = false;
                }
                else // normal  descendant
                {
                    cacheAre += curHeight;
                }
            }
            pre={i, curHeight};
        }
        return totalRaining;
    }
    int trapV2(vector<int>& height) {
        if( height.empty() ) return 0;
        int total = 0;
        height.push_back(0);
        vector<int> left;
        vector<int> right(height.size(), 0);
        for( int i = 0; i < height.size(); ++i)
        {
            if( i == 0) left.push_back(0);
            else
            {
                left.push_back(max(left.back(), height[i - 1]));
            }
        }
        for( int i = height.size() - 1; i >= 0; i--)
        {
            if( i == height.size() -1)  right[i] = 0;
            else
            {
                right[i] = max(height[i + 1], right[i+1]);
            }
        }
        for(int i = 0; i < height.size(); ++i)
        {
            int res = 0;
            if((res = min(left[i], right[i]) - height[i]) > 0)
                total+= res;
        }

        return total;
    }
    int trapV3(vector<int>& height) {
        int total = 0;
        stack<int> stk;
        int curr = 0;
        while(curr < height.size() )
        {
            while(!stk.empty() && height[curr] > height[stk.top()])
            {
                const int pre = stk.top();
                stk.pop();
                if( stk.empty()) break;
                total += (curr - stk.top() - 1) * (min(height[curr], height[stk.top()]) - height[pre]);
            }
            stk.push(curr++);
            
        }
        return total;
    }
};

int main() 
{
    //vector< int> input{0,1,0,2,1,0,1,3,2,1,2,1};
    vector< int> input{5,4,1,2};
    std::cout << "out put : " << Solution().trapV3(input) << std::endl;
}
// 84. Largest Rectangle in Histogram
// tags: Stack, Array
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

class Solution {
public:
    
    int largestRectangleArea(vector<int>& heights) {
        heights.push_back(0);

        int maxRect= 0;
        stack<int> SR;

        const int size = heights.size();
        for(int t = 0; t < size; ++t)
        {
            while(!SR.empty() && (t==size || heights[SR.top()] > heights[t]))
            {
                const int r = SR.top();
                SR.pop();
                const int s = SR.empty() ? 0 : SR.top() + 1;
                maxRect = max(maxRect, heights[r]*(t-s));
            }
            if(t < size) SR.push(t);
        }
        return maxRect;
    }
};

class Solution2 {
public:
    
    int largestRectangleArea(vector<int>& heights) {
        heights.push_back(0);
        int maxRect= 0;
        stack<int> SR;

        int k = 0;
        while(k < heights.size())
        {
            if(SR.empty() || heights[SR.top()] <= heights[k])
            {
                SR.push(k);
                ++k;
            }
            else
            {
                const int t = SR.top();
                SR.pop();
                maxRect = max(maxRect, heights[t]*(k- (SR.empty()?-1:SR.top()) -1 ));
            }
        }
        return maxRect;
    }
};

class Solution
{
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
};

int main() 
{
    vector<int> input{7};
    int res = Solution().largestRectangleArea(input);
    std::cout << "=========================>" << res << std::endl;
}
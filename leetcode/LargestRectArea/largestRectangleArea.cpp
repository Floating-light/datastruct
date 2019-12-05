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
        SR.push(0);
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

int main() 
{
    vector<int> input{7};
    int res = Solution().largestRectangleArea(input);
    std::cout << "=========================>" << res << std::endl;
}
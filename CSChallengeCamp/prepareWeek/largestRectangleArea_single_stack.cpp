#include <iostream>
#include <vector>
#include <stack>

using namespace std;

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

int main()
{
    int n = 0;
    vector<int> input;
    int temp= 0;
    cin >> n;
    for( int i = 0; i < n; ++i)
    {
        cin >> temp;
        input.push_back(temp);
    }

    cout << largestRectangleArea(input);
}
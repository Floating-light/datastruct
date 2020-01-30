#include <iostream>
#include <vector>
#include <stack>
#include <string>

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

int largestRedRectangleArea(vector<vector<int>>& input, const int& r, const int& c)
{
    int maxArea = 0;
    for(int i = 0; i < r; ++i)
    {
        if(i != 0) 
        {
            for(int j = 0; j < c; ++j)
            {
                if(input[i][j] != 0)
                {
                    input[i][j] += input[i-1][j];
                }
            }
        }
        maxArea = max(maxArea, largestRectangleArea(input[i]));
    }
    return maxArea;
}

int main()
{
    int r = 0,c = 0;
    cin >> r >> c;
    vector<vector<int>> input(r,vector<int>());

    string temp;
    for(int i = 0; i < r; ++i)
    {
        cin >> temp;
        for(int j =0; j < c; ++j)
        {
            if(temp[j] == '.')
                input[i].push_back(1);
            else
                input[i].push_back(0);
        }
    }

    cout << largestRedRectangleArea(input, r, c);
    return 0;
}
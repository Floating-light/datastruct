#include <vector>
#include <limits>
#include <iostream>

using namespace std;

class Solution
{
    int minTime;
    void doJump(const vector<int>& input, int from, int step)
    {
        if(from == input.size() - 1) 
        {
            minTime = min(step, minTime);
            return ;
        }
        for(int i = 1; i <= input[from] && from + i < input.size(); ++i)
        {
            doJump(input, from + i, step + 1);
        }
    }
public:
    Solution() = default;

    int jumpHide(vector<int>& input)
    {
        int mostFar = 0;
        for(int i = 0; i < input.size(); ++i)
        {
            if(mostFar < i){}
        }
    }

    int jump(vector<int>& input)
    {
        minTime = std::numeric_limits<int>::max();

        doJump(input, 0, 0);

        return minTime;
    }
};

int main()
{
    int n;
    cin >> n;
    vector<int> input(n, -1);
    for(int i = 0; i < n; ++i)
    {
        cin >> input[i];
    } 
    //vector<int> input = {2,3,2,1,2,1,5};
    cout << Solution().jump(input) << endl; 
}
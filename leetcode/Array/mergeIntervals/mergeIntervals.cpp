// 56. Merge Intervals
// tags: Sort, Array
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if(intervals.size() >= 1) return intervals;
        sort(intervals.begin(), intervals.end(),[](const vector<int>&l, const vector<int>&r)
        {
            return l[0] < r[0];
        });

        int pre = 0;
        for(int i = 1; i < intervals.size(); ++i)
        {
            if(intervals[pre][1] >= intervals[i][0])
            {
                intervals[pre][1] = max(intervals[pre][1], intervals[i][1]);
            }
            else
            {
                intervals[++pre] = intervals[i];
            }
        }        
        return vector<vector<int>>(intervals.begin(),intervals.begin() + pre + 1);
    }
};

class Solution2{
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if(intervals.size() <=1) return intervals;
        sort(intervals.begin(), intervals.end(),
        [](const vector<int>&l, const vector<int>&r)
        {
            return l[0] < r[0];
        });
        vector<vector<int>> res;
        vector<int> temp = intervals[0];
        for(int i = 1; i < intervals.size(); ++i)
        {
            if(intervals[i][0] > temp[1])
            {
                res.push_back(temp);
                temp = intervals[i];
            }
            else
            {
                temp[1] = max(intervals[i][1], temp[1]);

            }
        }      
        res.push_back(temp);  
        return res;
    }
};

int main()
{
    vector<vector<int>> input{
        {1,3},
        {2,6},
        {8,10},
        {15,18}
    };
    input = Solution2().merge(input);

    for(auto item : input)
    {
        for(auto i : item)
        {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }

}
// 57. Insert Interval
// tags: Sort, Array
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    vector<vector<int>> merge(vector<vector<int>>& intervals,const int& pos) {
        int pre = pos;
        for(int i = pos + 1; i < intervals.size(); ++i)
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
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        if(intervals.empty()) return vector<vector<int>>{newInterval};
        if(newInterval.empty()) return intervals;

        if(newInterval[0] > intervals.back()[1]) 
        {
            intervals.push_back(newInterval);
        }
        else
        for(int i = 0; i < intervals.size(); ++i)
        {
            if(newInterval[0] > intervals[i][1]) continue;
            else if(intervals[i][0] <= newInterval[0]) // && newInterval[0] <= intervals[i][1] 
            {
                intervals[i][1] = max(intervals[i][1], newInterval[1]);
                intervals = merge(intervals, i);
                break;
            }
            else if(intervals[i][0] <= newInterval[1])
            {
                intervals[i][0] = min(intervals[i][0], newInterval[0]);
                intervals[i][1] = max(intervals[i][1], newInterval[1]);
                intervals = merge(intervals, i);
                break;
            }
            else
            {
                intervals.insert(intervals.cbegin() + i, newInterval);
                break;
            }
        }
        return intervals;
    }
};
# 56. Merge Intervals
* Link: https://leetcode-cn.com/problems/merge-intervals/
## Solution
### 1. 先排序
* 先将输入集合的所有区间按起始元素的大小升序排列。
* 迭代所有区间，判断当前区间i是否和前一区间pre相交，是则合并。
- 迭代时，使用一个变量pre保存已迭代过的最右边的有效位置，每次合并时，仅仅是更新这个位置，在无需合并时将当前区间移至该位置后，并更新pre.

```c++
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if(intervals.size() <= 1) return intervals;
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
```
### 另一种写法

```c++
class Solution{
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
```

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// no repeated number in result
vector<vector<int>> combinationSum(vector<int>& candidates, int target)
{
    if(candidates.size() == 0)
        return vector<vector<int>>{};
    vector<vector<int>> result;
    vector<int> index;
    const int length = candidates.size();
    int currentSum;

    sort(candidates.begin(), candidates.end());

    index.push_back(0);
    currentSum= candidates[0];

    while(true)
    {
        if(currentSum == target)
        {
            result.push_back(index);
        }
        const int nextIndex = index.back() + 1;
        if(currentSum >= target || nextIndex >= length)
        {
            currentSum -= candidates[nextIndex- 1];
            index.pop_back();
            
            if(index.empty())
                break;
            currentSum += candidates[index.back() + 1] - candidates[index.back()];
            ++index.back();
        }
        else
        {
            currentSum += candidates[nextIndex];
            index.push_back(nextIndex);
        }
    }
    
    for(int i = 0; i < result.size(); ++i)
    {
        std::cout << "result " << i << ": ";
        for(int j = 0; j < result[i].size(); ++j)
        {
            std::cout << (result[i][j] = candidates[result[i][j]]) << ", ";
        }
        std::cout << std::endl;
    }
    return result;
}
// 12ms
vector<vector<int>> combinationSumReal(vector<int>& candidates, int target)
{
	if (candidates.size() < 1)
		return vector<vector<int>>{};
	const int length = candidates.size();
	std::sort(candidates.begin(), candidates.end());
	vector<vector<int>> result;

	std::vector<int> index{};
	int currentSum = 0;
	int nextTryIndex = 0;

	while (!index.empty() || nextTryIndex != length)
	{
		while (currentSum < target && nextTryIndex != length)
		{
			currentSum += candidates[nextTryIndex];
			index.push_back(nextTryIndex);
		}
		if (currentSum == target)
		{
			vector<int> temp;
			for (int i = 0; i < index.size(); ++i)
			{
				temp.push_back(candidates[index[i]]);
			}
			result.push_back(temp);
		}

		if (length != nextTryIndex)
		{
			currentSum -= candidates[index.back()];
			index.pop_back(); // remove the last candidate
		}

		if (index.empty())
		{
			break;
		}

		nextTryIndex = 1 + index.back();

		currentSum -= candidates[index.back()];
		index.pop_back();
	}

	return result;
}
class Solution {
    void DFS(const vector<int> &candidates, int currentSum, 
        const int& next, const int& target, vector<vector<int>> &result, vector<int>& path)
    {
        if(currentSum == target)
        {
            result.push_back(path);
            return;
        }
        for ( int i = next; i < candidates.size(); ++i)
        {
            const int nextSum = candidates[i] + currentSum;
            if(nextSum <= target)
            {
                path.push_back(candidates[i]);
                DFS(candidates, nextSum, i, target, result, path);
                path.pop_back();
            }
            else
            {
                break;
            }
        }
            
    }
    void DFS2(const vector<int> &candidates, int currentSum, 
        const int& next, const int& target, 
        vector<vector<int>> &result, vector<int>& path)
    {
        if(currentSum == target)
        {
            result.push_back(path);
            return;
        }

        for ( int i = next; i < candidates.size(); ++i)
        {
            if(i > next && candidates[i] == candidates[i-1]) 
                continue;
            const int nextSum = candidates[i] + currentSum;
            if(nextSum <= target)
            {
                path.push_back(candidates[i]);
                DFS2(candidates, nextSum, i+1, target, result, path);
                path.pop_back();
            }
            else
            {
                break;
            }
        }
            
    }
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        if(candidates.empty())
        {
            return vector<vector<int>>{};
        }
        sort(candidates.begin(), candidates.end());
        vector<vector<int>> result;
        std::vector<int> path{};
        DFS(candidates, 0,0,target,result,path);
        return result;
    }
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) 
    {
        vector<vector<int>> res;
        sort(candidates.begin(), candidates.end());
        if(candidates.size() == 0 || candidates[0] >target) return res;
        if( candidates[0] == target)
        {
             res.push_back(vector<int>(candidates[0]));
             return res;
        } 

        vector<int> path = {};
        int currentSum = 0;
        int next = 0;
        while(!path.empty() || next>=0)
        {
            const int now = currentSum + candidates[next];
            if( now < target)
            {
                path.push_back(path.back() + 1); // never reached border
                currentSum += candidates[path.back()];
            }
            else
            {
                if( now == target) // append to result
                {
                    vector<int> r;
                    for( auto& ind : path)
                    {
                        r.push_back(candidates[ind]);
                    }
                    r.push_back(candidates[next]);
                }
                int curIndex = path.back()+1;
                while(curIndex < candidates.size() && candidates[curIndex] == candidates[path.back()] ) 
                {
                    ++curIndex;
                }
                if( curIndex == candidates.size())
                {
                    path.pop_back();
                    next = path.back() + 1;
                    path.pop_back();
                }
                else
                {
                    path.pop_back();
                    next = curIndex;
                }
            }
        }
        
    }

    vector<vector<int>> combinationSum22(vector<int>& candidates, int target) 
    {
        sort(candidates.begin(), candidates.end());

        int currentSum = 0;
        int next = 0;
        vector<vector<int>> res;
        vector<int> path;
        DFS2(candidates,currentSum, next, target, res, path);
        return res;
    }
};
int main() 
{
    //vector<int> input = {1, 2,3,5,8,4,6,7,8,9,10};
    vector<int> input = {2,5,2,1,2};
    //vector<int> input = {};
    vector<vector<int>> result = Solution().combinationSum22(input, 5);
    for(int i = 0; i < result.size(); ++i)
    {
        std::cout << "result " << i+1 << ": ";
        for(int j = 0; j < result[i].size(); ++j)
        {
            std::cout << result[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}
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

void DFS(vector<int> &candidates, int currentSum, 
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
            DFS(candidates, nextSum, next, target, result, path);
        }
        else
        {
            break;
        }
    }
        
}

vector<vector<int>> combinationSumRealDFS(vector<int>& candidates, int target)
{
    if(candidates.empty())
    {
        return vector<vector<int>>{};
    }

}

int main() 
{
    //vector<int> input = {1, 2,3,5,8,4,6,7,8,9,10};
    //vector<int> input = {1, 2,3,5,8,4,6,7,8,9,10};
    vector<int> input = {2,3,5};
    vector<vector<int>> result = combinationSumReal(input, 8);
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
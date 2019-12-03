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

int main() 
{
    //vector<int> input = {1, 2,3,5,8,4,6,7,8,9,10};
    //vector<int> input = {1, 2,3,5,8,4,6,7,8,9,10};
    vector<int> input = {};
    combinationSum(input, 8);
}
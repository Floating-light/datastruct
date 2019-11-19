#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> combinationSum(vector<int>& candidates, int target)
{
    vector<vector<int>> result;
    sort(candidates.begin(), candidates.end());
    for(int i = 0; i < candidates.size(); ++i)
    {
        int currentSum = candidates[i];
        while(currentSum < target)
        {
            
        }
    }
    return vector<vector<int>>{};
}

int main() 
{
    std::cout << "test" << std::endl;
}
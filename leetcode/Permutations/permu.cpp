#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
class Solution {
    vector<vector<int>> res;
    vector<int> numbers;
    
    void permutations(int n)
    {
        if( n == numbers.size() )
        {
            res.push_back( numbers);
            return ;
        }
        for(int i = n ; i<numbers.size() ; ++i)
        {
            swap(numbers[i], numbers[n]);
            permutations(n + 1);
            swap(numbers[n], numbers[i]);
        }
    }
    void permutationsUnique(int n)
    {
        if( n == numbers.size() )
        {
            res.push_back( numbers);
            return ;
        }

        for(int i = n ; i<numbers.size() ; ++i)
        {
            if( i != n && numbers[i] == numbers[n] )
                continue;
            swap(numbers[i], numbers[n]);
            permutationsUnique(n + 1);
            swap(numbers[n], numbers[i]);
        }
    }
public:
    vector<vector<int>> permute(vector<int>& nums) {
        numbers = nums;

        permutations( 0);
        return res;
    }
    vector<vector<int>> permute2(vector<int>& nums) {
        vector<vector<int>> rest;
        sort(nums.begin(), nums.end());
        rest.push_back(nums);
        while( next_permutation(nums.begin(), nums.end()))
        {
            rest.push_back(nums);
        }
        return rest;
    }
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        numbers = nums;
        permutationsUnique(0);
        return res;
    }
};

int main()
{
    vector<int> input = {1, 1, 2, 2};
    vector<vector<int>> res = Solution().permuteUnique(input);
    for (int i = 0; i < res.size();++i) 
    {
        for (int j = 0; j <res[i].size(); ++j)
        {
            std::cout << res[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}
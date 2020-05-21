// 47. Permutations II
// tags: Backtracking
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>

using namespace std;

// from [1, 2, 3], let ervery element be the first, and exchange remaining elements.
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
        set<int> s;
        for(int i = n ; i<numbers.size() ; ++i)
        {
            if( s.find(numbers[i]) != s.end() )
                continue;
            s.insert(numbers[i] );
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
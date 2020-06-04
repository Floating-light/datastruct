#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<bool> kidsWithCandies(vector<int>& candies, int extraCandies) {
        vector<bool> res(candies.size(), true);
        if (candies.size() < 2) return res;

        int maxItem = *std::max_element(candies.cbegin(), candies.cend());

        for (int i = 0; i < candies.size(); ++i)
        {
            if (candies[i] + extraCandies < maxItem)
            {
                res[i] = false;
            }
        }

        return res;
    }
};
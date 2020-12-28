#include <vector>
#include <iostream>
#include <limits>
using namespace std;

class Solution {
public:
    int maxProfit(int k, vector<int>& prices) 
    {
        if(prices.empty())
        {
            return 0;
        }
        k = std::min(k,static_cast<int>(prices.size()/2));
        vector<vector<int>> buy(prices.size(), vector<int>(k+1,numeric_limits<int>::min() / 2));
        vector<vector<int>> sell(prices.size(), vector<int>(k+1,numeric_limits<int>::min() / 2));
        buy[0][0] = -prices[0];
        sell[0][0] = 0;
        for(int i = 1; i < prices.size(); ++i)
        {
            buy[i][0] = std::max(sell[i-1][0] - prices[i], buy[i-1][0]);
            // j = 0 无法计算sell, 此时不存在因卖出而导致的不持有
            for(int j = 1; j < k+1; ++j)
            {
                buy[i][j] = std::max(sell[i-1][j] - prices[i], buy[i-1][j]);
                sell[i][j] = std::max(buy[i-1][j-1] + prices[i], sell[i-1][j]);
            }
        }
        int res = 0;
        for(int kk = 0; kk < k+1; ++kk)
        {
            res = std::max(res, sell[prices.size() - 1][kk]);
        }
        return res;
    }
};
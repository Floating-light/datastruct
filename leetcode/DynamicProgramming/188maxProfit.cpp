#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    vector<vector<int>> dp;
    int maxMoney;
public:
    int bfs(vector<int> prices, int k , int i )
    {
        if(k<1 || i >= prices.size()) return 0;
        
        if(dp[i][k] != -1) return dp[i][k];

        int currentMaxMoney = 0;

        for(int m = i; m < prices.size(); ++m)// 买入点
        {
            for(int j = m; j < prices.size(); j++)// 最近的可卖出点
            {
                if(prices[j] > prices[m])
                {
                    int curChange = prices[j] - prices[m];
                    currentMaxMoney = std::max(currentMaxMoney, curChange + bfs(prices, k-1, j+1));
                }
            }
        }
        dp[i][k] = currentMaxMoney;
        return dp[i][k];
    }
    int maxProfit(int k, vector<int>& prices) {
        dp = vector<vector<int>>(prices.size(), vector<int>(k+1, -1));
        return bfs(prices,k,0);
    }
};
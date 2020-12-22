#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <limits>

using namespace std;

class Solution {
public:
    void dfs(vector<int>& c, int index, int cost)
    {
        cost += c[index];
        if(index + 1 >= c.size() || index + 2 >= c.size())
        {
            ResCost = std::min(ResCost, cost);
            return ;
        }
        if(cost >= ResCost)
        return ;
        dfs(c, index + 1, cost);
        dfs(c, index + 2, cost);
    }
    int minCostClimbingStairs(vector<int>& cost) 
    {
        ResCost = numeric_limits<int>::max();
        dfs(cost, 0, 0);
        dfs(cost, 1, 0);

        return ResCost;
    }
private:
int ResCost;
};

class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) 
    {
        vector<int> dp;
        dp.resize(cost.size()  +1, 0);
        
        for(int i = 2; i <= cost.size(); i++ )
        {
            dp[i] = std::min(dp[i - 1] + cost[i - 1], dp[i - 2 ] + cost[i - 2]);
        }
        return dp[cost.size()];
    }
};

class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) 
    {
        int first = 0;
        int second = 0;
        for(int i = 2; i <= cost.size(); i++ )
        {
            int temp = std::min(second + cost[i - 1], first + cost[i - 2]);
            first = second;
            second = temp;
        }
        return second;
    }
};
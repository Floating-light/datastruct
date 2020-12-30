#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

class Solution
{
    vector<vector<int>> dp;
    int maxMoney;

public:
    int bfs(vector<int> prices, int k, int i)
    {
        if (k < 1 || i >= prices.size())
            return 0;

        if (dp[i][k] != -1)
            return dp[i][k];

        int currentMaxMoney = 0;

        for (int m = i; m < prices.size(); ++m) // 买入点
        {
            for (int j = m; j < prices.size(); j++) // 最近的可卖出点
            {
                if (prices[j] > prices[m])
                {
                    int curChange = prices[j] - prices[m];
                    currentMaxMoney = std::max(currentMaxMoney, curChange + bfs(prices, k - 1, j + 1));
                }
            }
        }
        dp[i][k] = currentMaxMoney;
        return dp[i][k];
    }
    int maxProfit(int k, vector<int> &prices)
    {
        dp = vector<vector<int>>(prices.size(), vector<int>(k + 1, -1));
        return bfs(prices, k, 0);
    }
};

class Solution
{
public:
    int maxProfit(int k, vector<int> &prices)
    {
        if (prices.empty())
        {
            return 0;
        }
        k = std::min(k, static_cast<int>(prices.size() / 2));
        vector<vector<int>> buy(prices.size(), vector<int>(k + 1, numeric_limits<int>::min() / 2));
        vector<vector<int>> sell(prices.size(), vector<int>(k + 1, numeric_limits<int>::min() / 2));
        buy[0][0] = -prices[0];
        sell[0][0] = 0;
        for (int i = 1; i < prices.size(); ++i)
        {
            buy[i][0] = std::max(sell[i - 1][0] - prices[i], buy[i - 1][0]);
            // j = 0 无法计算sell, 此时不存在因卖出而导致的不持有
            for (int j = 1; j < k + 1; ++j)
            {
                buy[i][j] = std::max(sell[i - 1][j] - prices[i], buy[i - 1][j]);
                sell[i][j] = std::max(buy[i - 1][j - 1] + prices[i], sell[i - 1][j]);
            }
        }
        int res = 0;
        for (int kk = 0; kk < k + 1; ++kk)
        {
            res = std::max(res, sell[prices.size() - 1][kk]);
        }
        return res;
    }
};

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>

class Foo
{
private:
    std::condition_variable _cv;
    std::mutex _mtx1;
    int k = 0;

public:
    void first(std::function<void()> func)
    {
        func();
        k = 1;
        _cv.notify_all();
    }

    void second(std::function<void()> func)
    {
        std::unique_lock<std::mutex> lock(_mtx1);
        _cv.wait(lock, [this]() { return k == 1; });
        func();
        k = 2;
        _cv.notify_one();
    }

    void third(std::function<void()> func)
    {
        std::unique_lock<std::mutex> lock(_mtx1);
        _cv.wait(lock, [this]() { return k == 2; });
        func();
    }
};

template <int T>
void printnum()
{
    std::cout << T << " ";
}

int main()
{
    std::vector<std::function<void()>> functions(3);
    functions[0] = printnum<0>;
    functions[1] = printnum<1>;
    functions[2] = printnum<2>;

    for (int i = 0; i < 1000; ++i)
    {
        Foo foo;
        std::cout << i << " : ";
        std::thread td0(&Foo::first, &foo, functions[0]);
        std::thread td1(&Foo::second, &foo, functions[1]);
        std::thread td2(&Foo::third, &foo, functions[2]);

        td0.join();
        td1.join();
        td2.join();
        std::cout << std::endl;
    }
    return 0;
}
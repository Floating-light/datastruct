#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

class Solution {
	int res = std::numeric_limits<int>::max();
	int target;
	void search(const vector<int>& coins, int beg, long long currentSum, int coinsNum)
	{
		if (coinsNum >= res) return;
		for (int i = beg; i < coins.size(); ++i)
		{
			long long sum = currentSum + coins[i];
			if (sum < target)
			{
				search(coins, i, currentSum + coins[i], coinsNum + 1);
			}
			else if (sum == target)
			{
				res = std::min(coinsNum + 1, res);
			}
		}
	}
public:
	int coinChange(vector<int>& coins, int amount) {
		if (amount == 0) return 0;
		sort(coins.begin(), coins.end(), [](const int l, const int r) {return l > r; });
		target = amount;
		search(coins, 0, 0, 0);
		return (res == std::numeric_limits<int>::max()) ? -1 : res;
	}
};

int main()
{
	vector<int> input = { 186, 419, 83, 408 };
	cout << Solution().coinChange(input, 6249) << endl;
}
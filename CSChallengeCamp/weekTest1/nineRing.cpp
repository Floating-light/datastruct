/* 小C拿个一个九连环玩了七七四十九天，将其中一些环解下了。现在，你拿着这个小C玩过的九连环，
问最少需要多少步，可以将所有环解下？其中，一步指将一个环解下，或将一个环挂上。

输入描述
第一行一个正整数 n，表示被解下环的数量。

第二行有 n 个正整数 xi，表示被解下的环的编号。

输出描述
输出将环解下的最少步数

样例输入
8
2 3 4 5 6 7 8 9
样例输出
1 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

class Solution
{
    public:

    int solv(vector<bool>& input)
    {
        int steps= 0;
        bool action = true; // rule 1
        int t = accumulate(input.begin(), input.end(), 0);
        if(t % 2 == 0) action = false;
        while(accumulate(input.begin(), input.end(), 0) != 0)
        {
            if(action)
            {
                input[0] = !input[0];
            }
            else // rule 2
            {
                auto itr = find(input.begin(), input.end(),true);
                *(itr+1) = ! *(itr + 1);
            }
            ++steps;
            action = !action;
        }
        return steps;
    }
};

int main()
{
    int n;
    cin >> n;
    vector<bool> input(9, true);
    int down = 0;
    for(int i = 0; i < n; ++i)
    {
        cin >> down;
        input[down-1] = false;
    }
    cout << Solution().solv(input) << endl;
}
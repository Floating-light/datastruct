/* 描述
有n个正整数排成一排，你要将这些数分成m份（同一份中的数字都是连续的，不能隔开），同时数字之和最大的那一份的数字之和尽量小。

输入
输入的第一行包含两个正整数n，m。

接下来一行包含n个正整数。

输出
输出一个数，表示最优方案中，数字之和最大的那一份的数字之和。

样例1输入
5 2
2 1 2 2 3
样例1输出
5
样例1解释
若分成2和1、2、2、3，则最大的那一份是1+2+2+3=8；

若分成2、1和2、2、3，则最大的那一份是2+2+3=7；

若分成2、1、2和2、3，则最大的那一份是2+1+2或者是2+3，都是5；

若分成2、1、2、2和3，则最大的那一份是2+1+2+2=7。

所以最优方案是第三种，答案为5。 */

// 思路：
// 分治
// 大家记得看到“最大的最小”这一类语言，一定要想二分能不能做。
// 我们二分最大的那一份的和d，然后从左向右分组，在一组中，在和不超过d的情况下尽量往右分。若最终分出来的组数小于等于m，则这显然是合法的（我们在分出来的组里随便找个地方切开，总能变到m组，且每组的和不超过d）
// 这个d显然是单调的，即，若和不超过d能分成m组，则和不超过d+1也是能分成m组的，故二分正确。

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long LL;

class Solution
{
    public:

    bool check(const vector<LL>& in, LL mid, LL m)
    {
        LL count = 1;
        LL sum = 0;
        for(LL i : in)
        {
            if(i > mid) return false;
            if(sum + i > mid)
            {
                ++count;
                sum = i;
            }
            else
            {
                sum += i;
            }
        }
        if(count <= m)
            return true;
        else
            return false;
    }
    LL solv(const vector<LL>& input, LL m,LL sum)
    {
        LL l = 1;
        LL r = sum;
        while(l <= r)
        {
            LL mid  = (l+r)/2;
            if(check(input, mid, m))
            {
                r = mid  -1;
            }
            else
            {
                l = mid + 1;
            }
        }
        return r  +1;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    LL n, m;
    cin >> n >> m;
    vector<LL> input;
    LL sum = 0;
    LL t = 0;
    for(LL i = 0; i < n; ++i)
    {
        cin >> t;
        sum += t;
        input.push_back(t);
    }
    cout << Solution().solv(input, m, sum);
}
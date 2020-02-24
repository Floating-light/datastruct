/* 描述
给定一个n个整数的序列以及一个非负整数d，请你输出这个序列中有多少个连续子序列（长度大于1），满足该子序列的最大值最小值之差不大于d。

连续子序列：序列1 2 3中长度大于1的连续子序列有：

1 2
2 3
1 2 3
输入
第一行包含两个整数n,d。

接下来一行包含n个整数。

输出
输出一个整数，表示满足条件的连续子序列个数。

样例1输入
8 5
5 5 4 8 -10 10 0 1
样例1输出
7
样例1解释
满足条件的连续子序列有：

5 5
5 5 4
5 5 4 8
5 4
5 4 8
4 8
0 1 */

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

typedef long long LL;

LL Rmax[300005];
LL Rmin[300005];


class Solution
{

    LL cal(vector<LL> & input, LL l,LL mid, LL r, LL d)
    {
        for(int i = mid + 1; i <= r; ++i)
        {
            Rmin[i] = (i == mid+1) ? input[i] : min(Rmin[i-1], input[i]);
            Rmax[i] = (i == mid+1) ? input[i] : max(Rmax[i-1], input[i]);
        }
        LL res = 0;
        LL mn = 0,mx = 0, pos = r;
        for(int i = mid; i >=l&&pos > mid; --i)
        {
            mn = (i==mid)?input[i] : min(mn, input[i]);
            mx = (i==mid)?input[i] : max(mx, input[i]);
            while(pos > mid && max(mx,Rmax[pos]) - min(mn, Rmin[pos]) > d) --pos;
            res += pos - mid;
        }
        return res;

    }
    public:
    LL solv(vector<LL>& input, LL l, LL r, LL d)
    {
        if(l == r) return 0;
        int mid = (l+r) / 2;
        return solv(input,l,mid,d) + solv(input,mid+1, r,d) + cal(input,l,mid,r,d);
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    int n, d;
    cin >> n >> d;
    vector<LL> input(n);
    for(int i = 0;i < n; ++i)
    {
        cin >> input[i];
    }
    cout << Solution().solv(input,0,n-1,d);
}
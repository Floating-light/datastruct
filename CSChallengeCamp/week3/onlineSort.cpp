/* 描述
给出n个整数，将它们从小到大排序后输出。

输入
第一行为一个正整数n，第二行为n个整数。

输出
输出一行n个整数，表示排序后的n个整数。

样例1输入
5
5 4 2 3 -1
样例1输出
-1 2 3 4 5 */

#include <vector>
#include <list>
#include <iostream>

using namespace std;

typedef long long LL;

class Solution
{
    public:

};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    list<LL> l;
    int n;
    cin >> n;
    LL temp;
    for(int i = 0; i< n; i++)
    {
        cin >> temp;
        auto itr = l.cbegin();
        while(itr != l.cend() && *itr < temp) ++itr;
        l.insert(itr, temp);
    }
    for(auto i : l)
    {
        cout << i << " ";
    }
    
}
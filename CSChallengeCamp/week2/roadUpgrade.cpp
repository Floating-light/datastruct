/* 问题描述
Z国有 n 个城市和 m 条双向道路，每条道路连接了两个不同的城市，保证所有城市之间都可以通过这些道路互达。每条道路都有一个载重量限制，这限制了通过这条道路的货车最大的载重量。道路的编号从 1 至 m 。巧合的是，所有道路的载重量限制恰好都与其编号相同。

现在，要挑选出若干条道路，将它们升级成高速公路，并满足如下要求：

所有城市之间都可以通过高速公路互达。
对于任意两个城市 u,v 和足够聪明的货车司机：只经过高速公路从 u 到达 v 能够装载货物的最大重量，与经过任意道路从 u 到达 v 能够装载货物的最大重量相等。（足够聪明的司机只关注载重量，并不在意绕路）
在上面的前提下，要求选出的道路数目尽可能少。

求需要挑选出哪些道路升级成高速公路（如果有多种方案请任意输出一种）。

输入
第一行 2 个用空格隔开的整数 n,m ，分别表示城市数目、道路数目。

第 2 行到第 m+1 行，每行 2 个用空格隔开的整数 u,v 描述一条从 u 到 v 的双向道路，第 i+1 行的道路的编号为 i 。

注意：数据只保证不存在连接的城市相同的道路（自环），并不保证不存在两条完全相同的边（重边）

输出
第一行一个整数 k ，表示升级成高速公路的道路数。

接下来 k 行每行一个整数，从小到大输出所有选出的道路的编号。

输入样例
3 3
1 2
2 3
1 3
输出样例
2
2
3 */

#include <iostream>
#include <vector>

using namespace std;

// 最小生成树， primer算法，每一步都找最优的那条边
// 所以从权重最高的边开始，一步一步地将点集连接起来
// 遇到已经连在一起的点则跳过
// 采用并查集来保存节点和已有的连接
// 在find时，记得更新节点都指向根节点
class Solution
{
    vector<int> set;
    public:
    Solution(int nn) 
    {
        for(int i = 0; i <= nn; ++i)
        {
            set.push_back(i);
        }
    }

    int find(int x)
    {
        // 更新节点直接指向根节点
        // 否则会超时
        return set[x] == x?x:set[x]=find(set[x]);

        // while(set[n] != n) n = set[n];
        // return n;
    }

    bool unionSet(int n, int m)
    {
        int setx = find(n);
        int sety = find(m);
        if( setx != sety)
        {
            set[setx] = sety;
            return true;
        }
        return false;
    }

    void solution(int m,const vector<int>& u, const vector<int>& v)
    {
        vector<int> res;
        for(int i = m -1; i >=0; --i)
        {
            if(unionSet(u[i], v[i]))
                res.push_back(i+1);
        }
        cout << res.size() << endl;
        for(int i = res.size() -1 ; i >=0; --i)
        {
            //cout << *itr << endl;
            printf("%d\n", res[i]);
        }
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    int n,m;
    cin >> n >> m;
    Solution solu(n);
    vector<int> u;
    vector<int> v;
    int tempu;
    int tempv;
    for(int i = 0; i < m; ++i)
    {
        cin >>tempu >> tempv;
        u.push_back(tempu);
        v.push_back(tempv);
    }

    solu.solution(m, u, v);
}
/* 描述
又到了吃柿饼的季节。

小莉的果园共有 n 棵柿子树，编号为 1 到 n。最开始，这些柿子树之间都没有道路相连。
小莉现在规划出了 m 对中间可能修建双向道路的柿子树，用 m 个三元组 (u,v,w) 表示，
表示在编号为 u 和 编号为 v 的柿子树之间修建道路需要花费 w 元。

小莉决定在修完道路后，将能够直接或间接通过道路连接的柿子树划分为一个子集。并且，
对于划分出的每一个子集，用这个子集中的所有式子树上长出的柿子做出一个柿饼。

小莉最终一共想要得到 k 个柿饼，请你帮他计算最小的修路费用是多少。

输入
第 1 行有三个整数 n， m ，k ，含义见题目描述。

接下来 m 行，每行三个整数 u ， v ， w ，描述每条可能修建的道路，含义如题所述。

输出
输出一行一个整数，表示小莉修路的最小花费。

如果小莉无论如何都不能做出 k 个柿饼，请输出 −1 。

输入样例1
4 4 2
1 2 3
2 3 1
4 2 1
3 4 2
输出样例1
2
样例1解释
在 2，3 与 2，4 之间修建道路。

这样我们就可以将所有柿子制作成 2 个柿饼。 */

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class Solution
{
    vector<int> set;
    int cost;
    int currentK;
    void init(int n)
    {
        set.clear();
        for(int i = 0; i <=n; ++i)
        {
            set.push_back(i);
        }
        cost = 0;
    }
    // 路径压缩
    int find(int v)
    {
        return set[v] == v? v : set[v]=find(set[v]);
    }
    // return true if succeed union, else return false.
    bool unionSet(int u, int v)
    {
        int setu = find(u);
        int setv = find(v);
        if(setu != setv)
        {
            set[setu] = setv;
            return true;
        }
        return false;
    }
    public:
    // kruskal algrithm
    int solv(int n, int m, int k, vector<pair<int,pair<int,int>>>& edges)
    {
        if( k > n) return -1;
        if (k == n) return 0;
        init(n);
        sort(edges.begin(), edges.end(), 
            [](const pair<int,pair<int,int>>& l, const pair<int,pair<int,int>>& r)
            {
                return l.first < r.first;
            });
        auto itr = edges.cbegin();
        int cake = n; //当前独立的连通子集数目
        while(cake != k && itr != edges.cend()) // 数目到达k个
        {
            if(unionSet(itr->second.first,itr->second.second))
            {
                --cake;
                cost += itr->first;
            }
            ++itr;
        }
        if(k == cake) return cost;
        return -1;
    }

};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    // n 柿子树 vertex number
    // m the possible edge number 
    // k 需要的独立的支撑树数目
    int n, m, k;
    cin >> n >> m >> k;

    vector<pair<int,pair<int,int>>> edges;
    int u,v,w;
    for(int i = 0; i < m; ++i)
    {
        cin >> u >> v >> w;
        edges.push_back(pair<int,pair<int,int>>(w,pair<int,int>(u,v)));
    }
    Solution s;
    cout << s.solv(n,m,k,edges) << endl;
}
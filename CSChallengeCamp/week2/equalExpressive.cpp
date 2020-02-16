#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
/* 描述
有n个变量和m个“相等”或“不相等”的约束条件，请你判定是否存在一种赋值方案满足所有m个约束条件。

输入
第一行一个整数T，表示数据组数。

接下来会有T组数据，对于每组数据：

第一行是两个整数n,m，表示变量个数和约束条件的个数。

接下来m行，每行三个整数a,b,e，表示第a个变量和第b个变量的关系：

若e=0则表示第a个变量不等于第b个变量；
若e=1则表示第a个变量等于第b个变量
输出
输出T行，第i行表示第i组数据的答案。若第i组数据存在一种方案则输出"Yes"；否则输出"No"（不包括引号）。

输入样例1
2
5 5
1 2 1
2 3 1
3 4 1
1 4 1
2 5 0
3 3
1 2 1
2 3 1
1 3 0
输出样例1
Yes
No */
// 思考我们如何手动解决这一问题？
// 可以先把所有相等的放在一个集合中，再考虑不等的是否和这些集合矛盾，
// 即不等的两元素不应该出现在同一集合中
// 使用并查集
class Solution
{
    vector<int> s;
    public:

    void initSet(int n)
    {
        s = vector<int>(n+1, -1);
    }
    bool inSameSet(int a, int b)
    {
        if((a = find(a)) != (b = find(b)) || a== -1)
            return false;
        else
            return true;

    }
    void tryUnion(int a, int b)
    {
        if(!inSameSet(a, b))
        {
            unionSet(a, b);
        }
    }

    // 路径压缩
    int find(int a)
    {
        return s[a] == -1?a:s[a] = find(a);
        /* while(s[a] != -1)
        {
            a = s[a];
        }
        return a; */
    }

    void unionSet(int a, int b)
    {
        s[a] = b;

    }
};

const string yes = "Yes";
const string no = "No";

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int group;
    cin >> group;
    Solution solution;
    stringstream ss;
    vector<int> allExp;
    for(int i = 0; i < group; ++i)
    {
        int elems, exps;
        cin >> elems >> exps;
        solution.initSet(elems);
        allExp.clear();
        for(int j = 0; j < exps; ++j)
        {
            int a,b,optr;
            cin >> a >> b >> optr;
            if(optr == 0)
            {
          
                allExp.push_back(a);
                allExp.push_back(b);
            }
            else
            {
                solution.tryUnion(a,b);
            }
        }
        bool isvalid = true;
        while(!allExp.empty())
        {
            int a = allExp.back();
            allExp.pop_back();
            int b = allExp.back();
            allExp.pop_back();
            if(solution.inSameSet(a, b))
            {
                isvalid = false;
                break; // remeber to clear allexp
            }
        }
        if(isvalid)
            ss << yes;
        else
            ss << no;
        ss << endl;
    }
    cout << ss.str() ;
}
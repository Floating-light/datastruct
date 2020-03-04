#include <iostream>
#include <vector>

using namespace std;
class Solution
{
public:
    // 基于递推公式C(n,m) = C(n-1, m) + C(n-1, m-1)
    // 直接递归
    int combination(int n, int m)
    {
        if(m == 0) return 1; // do nothing, is a solution.
        if(n < m) return 0; // imposible
        return combination(n-1, m-1) + combination(n -1, m);
    }

    // 记忆化
    int combinationMemoary(vector<vector<int>>& memo, int n, int m)
    {
        if(memo[n][m] != -1) return memo[n][m];
        if(m == 0) return memo[n][m] = 1; // do nothing, is a solution.
        if(n < m) return memo[n][m] = 0; // imposible
        return (memo[n-1][m-1] =combinationMemoary(memo, n-1, m-1)) 
            + (memo[n-1][m] = combinationMemoary(memo, n -1, m));
    }
    int combinationMemoaryV2(vector<vector<int>>& memo, int n, int m)
    {
        if(memo[n][m] != -1) return memo[n][m];
        
        int res = 0;
        if(m == 0) res = 1; // do nothing, is a solution.
        else if(n < m) res = 0; // imposible
        else 
        {
            res = combinationMemoary(memo, n-1, m-1) + combinationMemoary(memo, n -1, m);
        }
        return memo[n][m] = res;
    }

    // memoary n*n
    int combinationMemoDirect(int n, int m)
    {
        vector<vector<int>> memo(n+1, vector<int>(n+1, 0));
        for(int i = 0; i <= n; ++i)
        {
            memo[i][0] = 1;
            for(int j = 1; j <= i; ++j)
            {
                memo[i][j] = memo[i-1][j] + memo[i -1][j -1];
            }
        }
        return memo[n][m];
    }

    // memoary 2* n
    // 滚动数组
    int combinationMemo2mN(int n, int m)
    {
        vector<vector<int>> memo(2, vector<int>(n+1,0));
        bool cur = 1;
        bool last = 0;
        for(int i = 0; i <= n; ++i)
        {
            swap(cur,last);
            memo[cur][0] = 1;
            for(int j = 1; j <= i; ++j)
            {
                memo[cur][j] = memo[last][j] +memo[last][j-1];
            }
        }
        return memo[cur][m];
    }

    // memoary n
    int combinationMemoOneArray(int n, int m)
    {
        vector<int> memo(n+1, 0);
        memo[0] = 1;
        for(int i = 1;i <= n; ++i)
        {
            for(int j = i; j >=1; --j)
            {
                memo[j] = memo[j] + memo[j - 1];
            }
        }
        return memo[m];
    }

    // 基于通项公式C(n,m) = n!/m!(n-m)!

    // 计算阶乘
    int facDiect(int n)
    {
        int ans = 1;
        for(int i = 1; i <= n; ++i)
        {
            ans *= i; // 乘法太慢
        }
        return ans;
    }

    long long facDirectV2(int n)
    {
        fac = vector<long long>(n +1, 1);
        for(int i = 1; i <= n; ++i)
        {
            fac[i] = i*fac[i -1];
        }
        return fac[n];
    }

    long long combinationGeneral(int n, int m)
    {
        facDirectV2(n);
        return fac[n]/(fac[m]*fac[n-m]);
    }

    // 基于通项公式
    // 知道C(n,m) 就能知道相邻项的值
    // 反推即可从C(n,0)=1 求出C(n,m)
    // C(n, m+1) = C(n ,m)*(n-m)/m
    int combinationNerbor(int n, int m)
    {
        int ans=1;
        for(int i = 1; i <= m; ++i)
        {
            ans = ans*((n - i +1)/i);
        }
        return ans;
    }
private:
    vector<long long> fac;
};

int main()
{
    Solution so;
    clock_t begin = clock();
    std::cout << so.combination(30,15) << endl; // 1.95679s
    /* int n = 40;
    int m = 20;
    vector<vector<int>> memo(n+1, vector<int>(m+1, -1));
    std::cout << so.combinationMemoaryV2(memo,n,m) << endl; // 0.0001s */
    // cout << so.combinationMemoOneArray(40,20) << endl;
    cout << so.combinationNerbor(30,15) << endl;
    clock_t end = clock();
    cout << "Total time: " << double(end - begin)/CLOCKS_PER_SEC << " s" << endl;
    // 2432902008176640000
}
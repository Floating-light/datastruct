/* 题目描述
这是一道很简单的问题。

给你一个长度为 n 的序列 a1,a2,...,an，求这个序列的第 k 小的数。
k 小数的定义为：将 a 从小到大排序后第 k 个位置的值。 由于输入数据量过大，
输入只给出a1的值，并采用如下递推式生成剩余数据：ai=(pa_(i−1)+q)modM(1<i≤n)其中，
mod 运算的含义是：a mod b 的值为 a 除以 b 得到的余数。

输入格式
输入第一行为两个正整数 n,k，含义见描述。

第二行为四个非负整数 a1,p,q,M，你需要根据题目中给出式子计算完整序列。

输出格式
输出一行一个正整数，表示第原序列第 k 小的数。

输入样例1
5 4
6 7 5 10
输出样例1
6 */

// 思想
// 随机猜一个pivot，然后把大的都移到它的右边，小的都移到它的左边，
// 如果猜得不对，根据pivot的位置与k的大小关系，
// 猜大了，就把hi左移到pivot - 1，猜小了 lo = pivot +1

#include <iostream>

using namespace std;

class Solution
{
    public:
    void solv(int* input,int n, int k)
    {
        for(int lo = 0,hi = n-1; lo < hi ;)
        {
            int i = lo;
            int j = hi;
            int pivot = input[i];
            // 在lo和hi范围内，以pivot为中心，使其前面的都比它小，后面的都比他大
            while(i < j)
            {
                while(i < j&& pivot <= input[j]) --j;
                input[i] = input[j];
                while(i < j&& input[i] <= pivot) ++i;
                input[j] = input[i];
            }
            input[i] = pivot;

            if(k >= i) lo = i+1;
            if(k <= i) hi = i - 1;
            // else break;
        }
    }
};

int input[30000005];


int main()
{
    int n,k;
    cin >> n >> k;
    // ai=(pa_(i−1)+q)modM(1<i≤n)
    int a1,p,q,M;
    cin >> a1 >> p >> q >> M;
    //vector<int> input(n);
    input[0] = a1;
    for(int i = 1 ; i < n; ++i)
    {
        input[i] = (p*input[i-1] + q) % M;
    }
    Solution().solv(input,n, k-1);
    cout << input[k-1] ;
}
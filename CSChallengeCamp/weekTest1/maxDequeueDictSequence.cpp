/* 描述
给定一个1到n的排列，依次从队尾插入到队列中，在任何时刻你都可以弹出队首或队尾。最终你需要把队列清空。

问：如何使得出队序列的字典序最大？请输出这个序列。

输入
输入的第一行包含一个正整数n。

接下来1行包含n个整数，一个1到n的排列。

输出
输出1行n个整数，即所求序列。

样例1输入
5
1 4 3 5 2
样例1输出
5 3 4 2 1
样例1解释
依次加入1、4、3、5、2到队列里，我们有这些个出队序列：

4 3 2 5 1
4 3 5 2 1
等等等，但字典序最大的是

5 3 4 2 1 */

#include <iostream>
#include <vector>
#include <list>

using namespace std;

class Solution
{
    int findmax(vector<int>& in)
    {
        int m = 0;
        for(int i = 1; i < in.size(); ++i)
        {
            if( in[m] < in[i]) m = i;
        }
        return m;
    }
    public:
    void solv(vector<int> input)
    {
        int n = input.size();
        list<int> dqu;
        int target = n;
        while(!dqu.empty() || !input.empty())
        {
            int maxIndex = findmax(input);
            if(dqu.empty() || !input.empty()&&(input[maxIndex] > dqu.back()&&input[maxIndex]>dqu.front()))
            {
                while(input.size() > maxIndex+1)
                {
                    dqu.push_back(input.back());
                    input.pop_back();
                }
                //dqu.insert(dqu.cend(), input.begin() + maxIndex+1, input.end());
                cout << input.back() << " ";
                input.pop_back();
            }
            else
            {
                // while(!dqu.empty() &&(input.empty() || (dqu.front() > input[maxIndex] || dqu.back() > input[maxIndex])))
                if(!dqu.empty())
                {
                    if(dqu.front() > dqu.back())
                    {
                        cout << dqu.front() << " ";
                        dqu.pop_front();
                    }
                    else
                    {
                        cout << dqu.back() << " ";
                        dqu.pop_back();
                    }
                }
                
            }
        }
        
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    int n ;
    cin >> n;
    vector<int> input(n);
    for( int i = 0; i < n; ++i)
    {
        cin >> input[i];
    }

    Solution().solv(vector<int>(input.rbegin(), input.rend()));
}
/* 问题描述
给定长度为 2n−1 的数组，对于每个 k=1,2,...,n，求出前 2k−1 个数的中位数。

输入格式
第一行一个正整数 n。

接下来一行 2n−1 个正整数，表示所给数组。

输出格式
输出 n 行，第 i 行表示这个数组的前 2i−1 个数的中位数。

样例输入
3
8 7 6 9 9
样例输出
8
7
8 */

// 思路：用两个堆分别保存最大的k-1个元素和最小的k-1个元素

#include <vector>
#include <algorithm>
#include <iostream>
#include <queue>


using namespace std;

class Solution
{
    public:
    void solv(vector<int>& input)
    {
        if(input.size() == 0) return ;
        vector<int> biggerHeap;
        vector<int> smallerHeap;

        for(int i = 0; i < input.size(); i++)
        {
            if(biggerHeap.empty() || biggerHeap[0] < input[i])
            {
                biggerHeap.push_back(input[i]);
                push_heap(biggerHeap.begin(), biggerHeap.end(), std::greater<>());
            }
            else 
            {
                smallerHeap.push_back(input[i]);
                push_heap(smallerHeap.begin(), smallerHeap.end(), std::less<>());
            }
            // 平衡
            if(biggerHeap.size() - smallerHeap.size() == 2)
            {
                pop_heap(biggerHeap.begin(), biggerHeap.end(), std::greater<>());
                
                smallerHeap.push_back(biggerHeap.back());
                pop_heap(smallerHeap.begin(), smallerHeap.end(), std::less<>());
                
                biggerHeap.pop_back();
            }
            else if(smallerHeap.size() - biggerHeap.size() == 2)
            {
                pop_heap(smallerHeap.begin(), smallerHeap.end(), std::less<>());

                biggerHeap.push_back(smallerHeap.back());
                push_heap(biggerHeap.begin(), biggerHeap.end(), std::greater<>());

                smallerHeap.pop_back();
            }

            if(i % 2 == 0)
            {
                if(biggerHeap.size() > smallerHeap.size()) cout << biggerHeap[0] << endl;
                else cout << smallerHeap[0] << endl;
            }
        }
    }
};

class Solution2
{
    public:
    void solv(vector<int>& input)
    {
        std::priority_queue<int, vector<int>, std::greater<int>> bigger;
        std::priority_queue<int, vector<int>, std::less<int>> smaller;

        for(int i = 0; i < input.size(); ++i)
        {
            if(bigger.empty())
            {
                bigger.push(input[i]);
            }
            else if(bigger.size() == smaller.size())
            {
                if(bigger.top() < input[i])
                {
                    bigger.push(input[i]);
                }
                else
                {
                    smaller.push(input[i]);
                }
            }
            else if(bigger.size() > smaller.size())
            {
                if(input[i] <= bigger.top())
                {
                    smaller.push(input[i]);
                }
                else
                {
                    smaller.push(bigger.top());
                    bigger.pop();
                    bigger.push(input[i]);
                }
            }else // bigger.size() < smaller.size()
            {
                if(smaller.top() <= input[i] )
                {
                    bigger.push(input[i]);
                }
                else
                {
                    bigger.push(smaller.top());
                    smaller.pop();
                    smaller.push(input[i]);
                }
            }

            if(i % 2 == 0)
            {
                if(bigger.size() > smaller.size())
                {
                    cout << bigger.top() << "\n";
                }
                else
                {
                    cout << smaller.top() << "\n";
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
    const int total = 2*n-1;
    vector<int>input(total, 0);
    // int temp;
    for(int i = 0; i < total; ++i)
    {
        cin >> input[i];
        // input.push_back(temp);
    }

    Solution2().solv(input);
}
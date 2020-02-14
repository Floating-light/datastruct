#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

typedef long long LL;

struct Node
{
    Node(int i) : val(i) {}
    int val;
    vector<Node> children;
    bool operator>(const Node& r)
    {
        return val > r.val;
    }
};

class Solution
{
    
    public:
    LL shortest = 0;
    LL ReCodeK(vector<LL>& input, int k)
    {
        /* int mend = 0;
        if((mend = ((input.size() - 1) % (k - 1))) != 0 && k != 2) 
        {
            mend = k - 1 - mend;
            while(mend-- != 0) input.push_back(0);
        } */
        if(k!=2)
        {
            int r = input.size()%(k-1)==1 ? 0 : k-input.size()%(k-1);
            while(r--)
                input.push_back(0);
        }

        make_heap(input.begin(), input.end(),std::greater<>());

        while(input.size() > 1)
        {
            LL temp = 0;
            for(int i = 0; i < k; ++i)
            {
                pop_heap(input.begin(), input.end(),std::greater<>());
                temp += input.back();
                input.pop_back();
            }
            input.push_back(temp);
            shortest += temp;
            push_heap(input.begin(), input.end(),std::greater<>());
        }
        return shortest;
    }

    void dfs(const Node& n, int deep)
    {
        ++deep;
        if (n.children.empty()) 
        {
            shortest += n.val*deep;
            return ;
        }
        for(auto& child : n.children)
        {
            dfs(child, deep);
        }
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    
    int n, k;
    cin >> n >> k;
    vector<LL> input(n,0);
    for(int i = 0; i < n; ++i)
    {
        cin >> input[i];
    }
    Solution s;
    cout << s.ReCodeK(input, k);
}
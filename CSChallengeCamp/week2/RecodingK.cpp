#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
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
    int shortest = 0;
    int ReCodeK(vector<Node>& input, int k)
    {
        int mend = 0;
        if((mend = (input.size() - 1 % k - 1)) != 0) 
        {
            while(mend-- != 0) input.push_back(0);
        }
        make_heap(input.begin(), input.end(),std::greater<>());

        while(input.size() > 1)
        {
            Node nd(0);
            for(int i = 0; i < k; ++i)
            {
                pop_heap(input.begin(), input.end(),std::greater<>());
                nd.val += input.back().val;
                nd.children.push_back(std::move(input.back()));
                input.pop_back();
            }
            input.push_back(std::move(nd));
            push_heap(input.begin(), input.end(),std::greater<>());
        }
        dfs(input.back(), 0);
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
    int n, k;
    cin >> n >> k;
    vector<Node> input(n,Node(0));
    for(int i = 0; i < n; ++i)
    {
        cin >> input[i].val;
    }
    Solution s;
    s.ReCodeK(input, k);
     cout << s.shortest << endl;
}
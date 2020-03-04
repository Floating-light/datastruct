#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef long long LL;

class Solution
{
    list<LL> box;
public:
    void doSomething(const vector<LL>& nums, const vector<bool>& opts)
    {
        for(int i = 0; i < opts.size(); ++i)
        {
            if(opts[i]) pushNumber(nums[i]);
            else popNumber(nums[i]);
        }
    }
    void pushNumber(LL i)
    {
        auto itr = find(box.cbegin(), box.cend(), i);        
        if(itr != box.cend())
        {
            cout << "Failed" << endl;
        }
        else
        {
            box.push_back(i);
            cout << "Succeeded" << endl;
        }
    } 
    void popNumber(LL i)
    {
        auto itr = find(box.begin(), box.end(), i);
        if(itr != box.end())
        {
            box.erase(itr);
            cout << "Succeeded" << endl;
        }
        else
        {
            cout << "Failed" << endl;
        }
    }
};

class Solution2
{
    public:
    void doSomething(const vector<LL>& nums, const vector<bool>& opts)
    {
        std::unordered_map<LL, bool> m;
        for(int i = 0; i < nums.size(); ++i)
        {
            if(opts[i])
            {
                if(!m[nums[i]]) // 不存在
                {
                    m[nums[i]] = true;
                    cout << "Succeeded" << endl;
                }
                else
                {
                    cout << "Failed" << endl;
                }
            }
            else // pop
            {
                if(m[nums[i]]) // 存在
                {
                    m[nums[i]] = false;
                    cout << "Succeeded" << endl;
                }
                else
                {
                    cout << "Failed" << endl;
                }
            }
        }
    }

};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    int n;
    cin >> n;
    vector<bool> op(n, false);
    vector<LL> nums(n, -1);
    for(int i = 0 ; i < n; ++i)
    {
        int opt;
        cin >> opt;
        if(opt == 1)
            op[i] = true;
        cin >> nums[i];
    }
    Solution2 solu;
    solu.doSomething(nums, op);
}
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <complex.h>
using namespace std;


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    // sum - times
    unordered_map<int, int> times;
    int maxTimes;
    int subSum(TreeNode* root)
    {
        int l = 0,r = 0;
        if(root->left)
        {
            l = subSum(root->left);
        }
        if(root->right)
        {
            r = subSum(root->right);
        }
        int rootSum = root->val + l + r;
        maxTimes = std::max(++times[rootSum], maxTimes);
        return rootSum;
    }
    vector<int> findFrequentTreeSum(TreeNode* root) 
    {
        if(root == nullptr)
            return {};
        subSum(root);
        vector<int> res;
        for(const std::pair<const int, int>& item:times)
        {
            if(item.second == maxTimes)
            {
                res.push_back(item.first);
            }
        }
        return std::move(res);
    }
};

int main()
{
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(2);
    root->right = new TreeNode(-3);
    vector<int> res = Solution().findFrequentTreeSum(root);

    for ( auto item : res)
    {
        std::cout << item << std::endl;
    }
    ::operator new(sizeof(int));
}
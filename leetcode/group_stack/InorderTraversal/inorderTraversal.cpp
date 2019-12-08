#include <vector>
#include <stack>
#include <iostream>
using namespace std;
    struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
class Solution {

public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> result;
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        while(cur!= nullptr || !stk.empty())
        {
            if( cur != nullptr)
            {
                stk.push(cur);
                cur = cur->left;
            }
            else
            {
                cur = stk.top();
                stk.pop();
                result.push_back(cur->val);
                cur = cur->right;
            }
        }
        return result;
    }
};

int main() 
{
    TreeNode* root = new TreeNode(1);
    TreeNode* s = new TreeNode(2);
    TreeNode* t = new TreeNode(3);

    root->right = s;
    s->left = t;
    vector<int> result = Solution().inorderTraversal(root);
    for( auto i : result)
    {
        std::cout << i << ", ";
    }
    std::cout << std::endl << "==================================" << std::endl;

}
#include <vector>
#include <stack>
using namespace std;

class Solution 
{
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
public:
    vector<int> preorderTraversal(TreeNode* root) {
        stack<TreeNode* > stk;
        vector<int> result;
        if( root )
            stk.push(root);
        while( stk.empty() )
        {
            const TreeNode* cur = stk.top();
            stk.pop();
            result.push_back(cur->val);
            if(cur->right)
                stk.push(cur->right);
            if(cur->left)
                stk.push(cur->left);
        }
        return result;
    }
};
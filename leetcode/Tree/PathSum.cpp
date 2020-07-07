#include <iostream>
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
struct TreeNode
{
    int val;
    TreeNode * left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
// 112. Path Sum
// https://leetcode-cn.com/problems/path-sum/
class Solution {
    bool hasSum(TreeNode* r, int sum)
    {
        if(!r) return false;
        // 弄清楚递归终止的条件是
        if((sum - r->val == 0)&& r->left == nullptr && r->right == nullptr)
        {
            return true;
        }
        return hasSum(r->left, sum - r->val) || hasSum(r->right, sum - r->val);
    }
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if(!root) return false;
        return hasSum(root, sum);
    }
};
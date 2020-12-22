#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    void BFS(stack<TreeNode*> stk, vector<vector<int>>& res, bool revers)
    {
        vector<int> curRes;
        stack<TreeNode*> next;
        while(!stk.empty())
        {
            TreeNode* cur = stk.top();
            stk.pop();
            curRes.push_back(cur->val);
            if(revers)
            {
                if(cur->right != nullptr )
                    next.push(cur->right) ;
                if(cur->left != nullptr )
                    next.push(cur->left);
            }
            else
            {
                if(cur->left != nullptr )
                    next.push(cur->left);
                if(cur->right != nullptr )
                    next.push(cur->right) ;
            }
        }
        res.push_back(curRes);
        if(!next.empty())
        {
            BFS(next, res, !revers);
        }

    }
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        stack<TreeNode*> stk;
        vector<vector<int>> res;
        if(root == nullptr)
        {
            return res;
        }
        stk.push(root);
        BFS(stk, res, false);
        return res;
    }
};

class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if(!root)
            return result;
        deque<TreeNode*> stk;
        stk.push_back(root);
        bool isLeftToRight = true;
        while(!stk.empty())
        {
            int currentLevelNodeCount = stk.size();
            vector<int> levelValue;
            while(currentLevelNodeCount != 0)
            {
                if(isLeftToRight)
                {
                    TreeNode* cur = stk.back();
                    stk.pop_back();
                    levelValue.push_back(cur->val);
                    if(cur->left)
                        stk.push_front(cur->left);
                    if(cur->right)
                        stk.push_front(cur->right);
                }
                else
                {
                    TreeNode* cur = stk.front();
                    stk.pop_front();
                    levelValue.push_back(cur->val);
                    if(cur->right)
                        stk.push_back(cur->right);
                    if(cur->left)
                        stk.push_back(cur->left);
                }
                currentLevelNodeCount--;
            }
            isLeftToRight = !isLeftToRight;
            result.push_back(levelValue);
        }
        return result;
    }
};
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
using namespace std;
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
    struct TreeNode
    {
        int val;
        TreeNode *left;
        TreeNode *right;
        TreeNode(int x) : val(x), left(NULL), right(NULL) {}
    };
    
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
    vector<vector<int>> zigzagLevelOrder2(TreeNode* root) {
        vector<vector<int>> result;
        if(!root)
            return result;
        stack<TreeNode*> stk_pop;
        stack<TreeNode*> stk_push;

        stk_push.push(root);
        bool isPushLeft = true;
        while(!(stk_push.empty()&&stk_pop.empty()))
        {
            stk_pop.swap(stk_push);
            vector<int> levelValue;
            while(!stk_pop.empty())
            {
                TreeNode* cur = stk_pop.top();
                stk_pop.pop();
                levelValue.push_back(cur->val);
                if(isPushLeft)
                {
                    if(cur->left)
                        stk_push.push(cur->left);
                    if(cur->right)
                        stk_push.push(cur->right);
                }
                else
                {
                    if(cur->right)
                        stk_push.push(cur->right);
                    if(cur->left)
                        stk_push.push(cur->left);
                }

            }
            isPushLeft = !isPushLeft;
            result.push_back(levelValue);
        }
        return result;
    }
    vector<vector<int>> zigzagLevelOrder3(TreeNode* root) {
        vector<vector<int>> result;
        if(!root)
            return result;
        stack<TreeNode*> stk1;
        stack<TreeNode*> stk2;

        stk1.push(root);
        while(!(stk1.empty()&&stk2.empty()))
        {
            if(stk2.empty())
            {
                vector<int> levelValue;
                while(!stk1.empty())
                {
                    TreeNode* cur = stk1.top();
                    stk1.pop();
                    levelValue.push_back(cur->val);
                    if(cur->left)
                        stk2.push(cur->left);
                    if(cur->right)
                        stk2.push(cur->right);
                }
                result.push_back(levelValue);
                
            }
            if(stk1.empty())
            {
                vector<int> levelValue;
                while(!stk2.empty())
                {
                    TreeNode* cur = stk2.top();
                    stk2.pop();
                    levelValue.push_back(cur->val);
                    if(cur->right)
                        stk1.push(cur->right);
                    if(cur->left)
                        stk1.push(cur->left);
                }
                result.push_back(levelValue);
            }
            
        }
        return result;
    }
};
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution 
{

vector<int> result;
public:
    void rec(TreeNode* c)
    {
        if( !c) return ;
        rec(c->left);
        rec(c->right);
        result.push_back( c->val );
    }
    vector<int> postorderTraversal(TreeNode* root) {
        rec(root);
        return result;
    }
    vector<int> postorderTraversalV2(TreeNode* root) {
        vector<int> result2;
        if( !root) return result2;
        stack<pair<bool, TreeNode*>> stk;
        TreeNode* cur = root;
        while( true)
        {
            while(cur != nullptr)
            {
                stk.push({true, cur});
                if( cur->right)
                    stk.push({false, cur->right});
                cur = cur->left;
            }
            pair<bool, TreeNode*> next = stk.top();
            while(next.first)
            {
                result2.push_back(next.second->val); 
                
                stk.pop(); 
                if(stk.empty()) break;
                next = stk.top();    
            }
            cur = next.second;
            if(stk.empty()) break;
            stk.pop();
        }
        return result2;
    }
    vector<int> postorderTraversalV3(TreeNode* root) {
        vector<int> result2;
        if( !root) return result2;

        stack<TreeNode*> stk;
        TreeNode* cur = root;
        stk.push(root);
        while(!stk.empty()) 
        {
            cur = stk.top();
            stk.pop();
            result2.push_back(cur->val);
            if( cur->left)
                stk.push(cur->left);
            if( cur->right)
                stk.push(cur->right);
        }
        std::reverse(result2.begin(), result2.end());
        return result2;
    }
    // how to distinguish if a node'right child has been reached in stack 
    vector<int> postorderTraversalV4(TreeNode* root) {
        vector<int> result2;
        stack<TreeNode* > stk;
        TreeNode* preVisited = nullptr;
        TreeNode* curr = root;
        while(curr != nullptr || !stk.empty()) 
        {
            if( curr)
            {
                stk.push(curr);
                curr = curr->left;
            }
            else
            {
                curr = stk.top();
                if( curr->right != nullptr && curr->right != preVisited)
                {
                    stk.push( curr->right);
                    curr = curr->right->left;
                }
                else
                {
                    result2.push_back(curr->val);
                    preVisited = curr;
                    stk.pop();
                    curr = nullptr;
                }
            }
        }
        return result2;
    }

};

int main()
{
    TreeNode* root = new TreeNode(1);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);
    root->right = n2;
    n2->left = n3;
    vector<int> res = Solution().postorderTraversalV4(root);
    for( int item : res)
    {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
} 
#include <vector>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
    ListNode * reverse(ListNode * l)
    {
        ListNode* pre = nullptr;
        ListNode* p = l;
        while(p)
        {
            ListNode* tem = p->next;
            p->next = pre;
            pre = p;
            p = tem;
        } 
        return pre;
    }
public:
    ListNode* reverseKGroup(ListNode* head, int k) 
    {
        ListNode* l = nullptr;
        ListNode* r = head;
        int count = 0;
        while(r)
        {
            
        }
    }
};
// 25. Reverse Nodes in k-Group
// tags: linked list
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
        ListNode* h = new ListNode(-1);
        h->next = head;
        ListNode* l = h;
        ListNode* r = h;
        int count = 0;
        while(r->next)
        {
            ++count;
            r=r->next;
            if(count == k)
            {
                ListNode* templ = r->next;
                r->next = nullptr;
                ListNode* newhead = reverse(l->next);
                l->next->next = templ;  
                r = l->next;
                l->next = newhead;
                l = r;
                count = 0;
            }
        }
        return h->next;
    }

};
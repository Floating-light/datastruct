#include <iostream>
#include <vector>

struct ListNode
{
    int val;
    ListNode* next;
    ListNode(int x): val(x), next(nullptr) {}
};

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2)
{
    ListNode* result = new ListNode(-1);
    ListNode* pre = result;
    while(l2 != nullptr && l1 != nullptr)
    {
        if(l1->val <= l2->val)
        {
            pre->next = l1;
            l1 = l1->next;
        }
        else
        {
            pre->next = l2;
            l2 = l2->next;
        }
        pre = pre->next;
    }
    pre->next = l1 == nullptr ? l2 : l1;
    return result->next;
}
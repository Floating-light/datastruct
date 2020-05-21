// 61. Rotate List
// tags: Linked List, Two Pointers

#include <iostream>
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* rotateRight(ListNode* head, int k)
{
    if(head == nullptr)
        return head;
    ListNode* realHead = new ListNode(0);
    realHead->next = head;

    ListNode* l=head;
    ListNode* r=head;
    int n = 1;
    while(r->next != nullptr)
    {
        if( k > 0)
        {
            --k;
        }
        else
        {
            l = l->next;
        }
        ++n;
        r = r->next;
    }
    if(k/n == 0)
    r->next = realHead->next;
    realHead->next = l->next;
    l->next = nullptr;
    return realHead->next;
}

ListNode* rotateRightCycle(ListNode* head, int k)
{
    if(head == nullptr)
        return head;
    ListNode* realHead = new ListNode(0);
    realHead->next = head;

    ListNode* r=head;
    int n = 1;
    while(r->next != nullptr)
    {
        r = r->next;
        ++n;
    }
    int mod = k%n;
    if(mod == 0)
    {
        return head;
    }
    int move = n - mod;
    ListNode* l = realHead;
    while(move > 0)
    {
        l = l->next;
        --move;
    }
    r->next = realHead->next;
    realHead->next = l->next;
    l->next = nullptr;

    return realHead->next;
}

int main()
{

    ListNode* head = new ListNode(1);
    ListNode* realHead = head;
    head->next = new ListNode(2);head = head->next;
    head->next = new ListNode(3);head = head->next;
    head->next = new ListNode(4);head = head->next;
    head->next = new ListNode(5);head = head->next;
    ListNode* result = rotateRightCycle(realHead, 6);
    while(result != nullptr)
    {
        std::cout << result->val << ", ";
        result = result->next;
    }
}
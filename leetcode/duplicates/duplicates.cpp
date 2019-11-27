struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* Duplicates(ListNode* head) 
{
    if(head == nullptr)
        return head;
    ListNode* p = head;
    while(p->next != nullptr)
    {
        if( p->val == p->next->val)
        {
            ListNode* temp = p->next;
            p->next = p->next->next;
            delete temp;
        }
        else
        {
            p = p->next;
        }
    }
    return head;
}

ListNode* DuplicatesRecursive(ListNode* head) 
{
    if(head == nullptr || head->next == nullptr)
        return head;
    ListNode* l = DuplicatesRecursive(head->next);
    if(l->val == head->val)
    {
        head->next = l->next;
        delete l;
    }
    return head;
}
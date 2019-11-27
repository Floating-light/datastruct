struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* deleteDuplicates(ListNode* head) 
{
    ListNode* realHead = new ListNode(0);
    realHead->next = head;
    
    ListNode* p = realHead;
    ListNode* p1 = p->next;
    while(p1 != nullptr && p1->next != nullptr)
    {
        const int& cur = p1->val;
        if(cur == p1->next->val)
        {
            while(p1->next != nullptr && cur == p1->next->val)
            {
                ListNode* temp = p1->next;
                p1->next = temp->next;
                delete temp;
            }
            p->next = p1->next;//remove the first
            delete p1;
            p1 = p->next;
        }
        else
        {
            p = p1;
            p1 = p1->next;
        }
    }
    return realHead->next;
}

ListNode* deleteDuplicatesV2(ListNode* head) 
{
    ListNode* realHead = new ListNode(0);
    realHead->next = head;
    
    ListNode* p = realHead;
    ListNode* p1 = p->next;
    while(p1 != nullptr)
    {
        if(p1->next == nullptr || p->next->val != p1->next->val)
        {
            if(p->next == p1)
                p = p1;
            p->next = p1->next;   
        }
        p1 = p1->next;
    }
    return realHead->next;
}
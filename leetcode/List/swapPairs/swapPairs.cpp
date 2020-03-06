// 24. Swap Nodes in Pairs
// tags: Linked List
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* swapPairs(ListNode* head)
{
    ListNode* realHead = new ListNode(0);
    realHead->next = head;

    ListNode* p = realHead;
    ListNode* p1;
    ListNode* p2;
    while((p1 = p->next) != nullptr && (p2 = p->next->next) != nullptr)
    {
        p->next = p2;
        p1->next = p2->next;
        p2->next = p1;
        p = p1;
    }
    return realHead->next;
}

ListNode* swapPairsRecursive(ListNode* head)
{
    if(head == nullptr || head->next == nullptr)
        return head;
    ListNode* next = head->next;
    head->next = swapPairsRecursive(next->next);
    next->next = head;
    return next;
}

int main() 
{

}
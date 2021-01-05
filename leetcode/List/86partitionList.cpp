#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;


// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* partition(ListNode* head, int x) 
    {
        if(head == nullptr ) return head;
        ListNode* EmptyHead = new ListNode(-1);
        EmptyHead->next = head;
        ListNode* l = EmptyHead;
        ListNode* c = EmptyHead;
        while(c->next != nullptr)
        {
            if(c->next->val < x)
            {
                if(l == c)
                {
                    l = c = l->next;
                    continue;
                }
                ListNode* newless = c->next;
                c->next = newless->next;
                newless->next = l->next;
                l->next = newless;
                l = newless;
            }
            else
            {
                c = c->next;
            }
            
        }
        return EmptyHead->next;
    }
};

// 将两部分分别保存, 省去插入节点,
class Solution {
public:
    ListNode* partition(ListNode* head, int x) 
    {
        ListNode* lhead = new ListNode(-1);
        ListNode* rhead = new ListNode(-1);
        ListNode* l = lhead;
        ListNode* r = rhead;
        while(head)
        {
            if(head->val < x)
            {
                l = l->next = head;
            }
            else
            {
                r = r->next = head;
            }
            head = head->next;
        }
        r->next = nullptr;
        l->next = rhead->next;
        return lhead->next;
    }
};


int main()
{
    vector<int> input{1,4,3,2,5,2};
    ListNode* first = nullptr;
    for(auto i : input)
    {
        ListNode* n = new ListNode(i);
        if(first != nullptr)
        {
            first->next = n;
        }
        else
        {
            first = n;
        }
    }
    auto res = Solution().partition(first, 3);
    std::cout << "Resutl node value : " << res->val << std::endl;
}
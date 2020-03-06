// 23. Merge k Sorted Lists
// tags: Heap, Linked List, Divide and Conquer
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
using namespace std;

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
    ListNode * mergeTwo(ListNode * l1, ListNode * l2) 
    {
        ListNode * h = new ListNode(-1);
        ListNode * c = h;
        while(l1 != nullptr && l2 != nullptr)
        {
            if(l1->val <= l2->val)
            {
                c->next = l1;
                l1= l1->next;
            }
            else
            {
                c->next = l2;
                l2 = l2->next;
            }
            c = c->next;
        }
        c->next = l1 == nullptr ?  l2 : l1 ;
        return h->next;
    }
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode* head = new ListNode(std::numeric_limits<int>::max());
        head->next = nullptr;
        for(int i = 0; i < lists.size(); ++i)
        {
            head->next = mergeTwo(head->next, lists[i]);
        }
        return head->next;
    }
};
class Solution2 {
    ListNode * mergeTwo(ListNode * l1, ListNode * l2) 
    {
        ListNode * h = new ListNode(-1);
        ListNode * c = h;
        while(l1 != nullptr && l2 != nullptr)
        {
            if(l1->val <= l2->val)
            {
                c->next = l1;
                l1= l1->next;
            }
            else
            {
                c->next = l2;
                l2 = l2->next;
            }
            c = c->next;
        }
        c->next = l1 == nullptr ?  l2 : l1 ;
        return h->next;
    }
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(lists.empty()) return nullptr;
        int n = lists.size();
        while( n > 1)
        {
            for(int i = 0; i < n/2; ++i)
            {
                lists[i] = mergeTwo(lists[i], lists[n - i - 1]);
            }
            n = n/2 + n%2;
        }
        return lists[0];
    }
};
class Solution3 {
    ListNode * mergeTwo(ListNode * l1, ListNode * l2) 
    {
        ListNode * h = new ListNode(-1);
        ListNode * c = h;
        while(l1 != nullptr && l2 != nullptr)
        {
            if(l1->val <= l2->val)
            {
                c->next = l1;
                l1= l1->next;
            }
            else
            {
                c->next = l2;
                l2 = l2->next;
            }
            c = c->next;
        }
        c->next = l1 == nullptr ?  l2 : l1 ;
        return h->next;
    }
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(lists.empty()) return nullptr;
        queue<ListNode*> q(deque<ListNode*>(lists.begin(), lists.end()));
        while( q.size() > 1)
        {
            ListNode* l1 = q.front();
            q.pop();
            ListNode* l2 = q.front();
            q.pop();

            q.push(mergeTwo(l1, l2));
        }
        return q.front();
    }
};
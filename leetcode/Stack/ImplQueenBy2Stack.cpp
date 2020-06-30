#include <stack>
#include <iostream>

// 剑指 Offer 09. 用两个栈实现队列
// https://leetcode-cn.com/problems/yong-liang-ge-zhan-shi-xian-dui-lie-lcof/
// 一个栈对外只push元素,另一个栈对外只pop元素
// pop栈为空时，将push栈中的所有元素入栈到pop栈中,
using namespace std;
class CQueue {
public:
    CQueue() {

    }
    void appendTail(int value) {
        pushStack.push(value);
    }
    int deleteHead() {
        setPopStack();
        if(popStack.empty()) return -1;
        int item = popStack.top();
        popStack.pop();
        return item;
    }
private:
    std::stack<int> pushStack;
    std::stack<int> popStack;

    void setPopStack()
    {
        if(popStack.empty())
        {
            while(!pushStack.empty())
            {
                popStack.push(pushStack.top());
                pushStack.pop();
            }
        }
    }
};

/**
 * Your CQueue object will be instantiated and called as such:
 * CQueue* obj = new CQueue();
 * obj->appendTail(value);
 * int param_2 = obj->deleteHead();
 */
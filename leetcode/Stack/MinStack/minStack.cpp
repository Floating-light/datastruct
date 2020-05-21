#include <vector>
#include <iostream>
#include <stack>
#include <limits>
using namespace std;

class MinStack {
private:
    stack<int> _data;
    stack<int> _min;
public:
    /** initialize your data structure here. */

    MinStack() : 
        _data()
    {
        
    }
    
    void push(int x) {
        _data.push(x);

        if(!_min.empty()&& _min.top() < x)
        {
            x = _min.top();
        }
        _min.push(x);
    }
    
    void pop() {
        _data.pop();
        _min.pop();
    }
    
    int top() {
        return _data.top();
    }
    
    int getMin() {
        return _min.top();
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(x);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
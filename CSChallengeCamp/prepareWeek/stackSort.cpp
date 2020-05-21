#include <iostream>
#include <stack>

using namespace std;

stack<int> stackSort(stack<int>& inputStack);

int main()
{
    int n = 0;
    stack<int> input;
    cin >> n;
    int temp;
    while(n > 0)
    {
        cin >> temp;
        input.push(temp);
        --n;
    }

    stack<int> result = stackSort(input);

    while(!result.empty())
    {
        cout << result.top() << endl;
        result.pop();
    }
}

stack<int> stackSort(stack<int>& inputStack)
{
    stack<int> result;
    if( inputStack.empty()) return result;

    int temp = inputStack.top();
    inputStack.pop();

    while(!inputStack.empty() || (!result.empty()&&temp > result.top()))
    {
        // The times of result.empty() == true is the number of elements in inputStack 
        // which is bigger(or less when ascending order) than all the element in its left.
        if(result.empty() || result.top() >= temp) // must have "="
        {
            result.push(temp);
            temp = inputStack.top();
            inputStack.pop();
        }
        // the excute times of this branch equal the total reverse pair 
        // numbers in the input sequence. 
        else 
        {
            inputStack.push(result.top());
            result.pop();
        }
    }
    result.push(temp);
    return result;
}
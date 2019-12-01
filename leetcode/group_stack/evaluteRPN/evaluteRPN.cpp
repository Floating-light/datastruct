#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <iostream>

using namespace std;
int evalRPN(vector<string>& tokens) {
    std::unordered_map<string, bool> hasOptr= {
        {"+", true},
        {"-", true},
        {"*", true},
        {"/", true}
    };
    stack<int> stk;
    for(int i = 0; i < tokens.size(); ++i)
    {
        string current = tokens[i];
        if(!hasOptr[current])
        {
            stk.push(atoi(current.c_str()));
        }
        else
        {
            int operRight = stk.top();
            stk.pop();
            int operLeft = stk.top();
            stk.pop();
            switch (current[0])
            {
            case '+':
                stk.push(operLeft + operRight);
                break;
            case '-':
                stk.push(operLeft - operRight);
                break;
            case '*':
                stk.push(operLeft * operRight);
                break;
            case '/':
                stk.push(operLeft / operRight);
                break;
            default:
                break;
            }
        }
    }
    return stk.top();
}

int evalRPNV2(vector<string>& tokens) {
    std::unordered_map<string, bool> hasOptr= {
        {"+", true},
        {"-", true},
        {"*", true},
        {"/", true}
    };
    stack<int> stk;
    for(int i = 0; i < tokens.size(); ++i)
    {
        const string& current = tokens[i];
        if(!hasOptr[current])
        {
            stk.push(atoi(current.c_str()));
        }
        else
        {
            const int operRight = stk.top();
            stk.pop();
            const int operLeft = stk.top();
            stk.pop();
            const char opt = current[0];
            if(opt == '+')
                stk.push(operLeft + operRight);
            else if(opt == '-')
                stk.push(operLeft - operRight);
            else if(opt == '*')
                stk.push(operLeft * operRight);
            else
                stk.push(operLeft / operRight);
        }
    }
    return stk.top();
}

int evalRPNV3(vector<string>& tokens) {
    stack<int> stk;
    for(int i = 0; i < tokens.size(); ++i)
    {
        const string& current = tokens[i];
        if(('0' <= current[0] && current[0] <= '9') || (current.size() >= 1 && current[0] == '-'))
        {
            stk.push(atoi(current.c_str()));
        }
        else
        {
            const int operRight = stk.top();
            stk.pop();
            const int operLeft = stk.top();
            stk.pop();
            const char opt = current[0];
            if(opt == '+')
                stk.push(operLeft + operRight);
            else if(opt == '-')
                stk.push(operLeft - operRight);
            else if(opt == '*')
                stk.push(operLeft * operRight);
            else
                stk.push(operLeft / operRight);
        }
    }
    return stk.top();
}

int main()
{
    std::unordered_map<string, bool> hasOptr= {
        {"+", true},
        {"-", true},
        {"*", true},
        {"/", true}
    };
    vector<string> input = {"10","6","9","3","+","-11","*","/","*","17","+","5","+"};
    int result  = evalRPNV3(input);
    std::cout << "test output: " <<  result<< std::endl;
}
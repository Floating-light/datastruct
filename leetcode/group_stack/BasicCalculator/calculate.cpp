#include <vector>
#include <string>
#include <stack>
#include <map>

#include <iostream>

using namespace std;

static int priority[4][4] = {
       // +  -  (  ) 
/* +  */{ 1, 1, 0, 1},
/* -  */{ 1, 1, 0, 1},
/* (  */{ 0, 0, 0, -1},
/* )  */{ 1, 1, 0, 1}
};

std::map<char, int> optrCharToSeque = {
    {'+', 0},
    {'-', 1},
    {'(', 2},
    {')', 3},
};

class Solution
{
string str;
int priority[4][4] = {
       // +  -  (  ) 
/* +  */{ 1, 1, 0, 1},
/* -  */{ 1, 1, 0, 1},
/* (  */{ 0, 0, 0, -1},
/* )  */{ 1, 1, 0, 1}
};

std::map<char, int> optrCharToSeque = {
    {'+', 0},
    {'-', 1},
    {'(', 2},
    {')', 3},
};
int readNumber(int& beg)
{
    string num;
    while( isdigit(str[beg]))
    {
        num.push_back(str[beg++]);
    }
    return atoi(num.c_str());
}
public:
    int calculate(string s) {
        str =s+')';
        stack<char> optr;
        optr.push('(');
        stack<int> opnd;
        int i = 0;
        while( i < str.size() )
        {
            const char& cur = str[i];
            if(cur == ' ') { ++i; continue; }
            if( isdigit(cur))
            {
                int temp = readNumber(i);
                opnd.push(temp);
            }
            else // operator or parenthesis
            {
                int op = priority[optrCharToSeque[optr.top()]][optrCharToSeque[cur]];
                if(op == 1)
                {
                    const int rightOpnd = opnd.top();
                    opnd.pop();
                    const int leftOpnd = opnd.top();
                    opnd.pop();
                    if( optr.top() == '+')
                        opnd.push(leftOpnd + rightOpnd);
                    else
                        opnd.push(leftOpnd - rightOpnd);
                    optr.pop();
                }
                else if( op == 0)
                {
                    optr.push( cur);
                    i++;
                }
                else
                {
                    optr.pop();
                    i++;
                }
            }
        }
        return opnd.top();
    }   

int calculate2(string s)
{
    stack<int> optr, opnd;
    int rst = 0;
    long long num = 0;
    int sign = 1;
    for( const char& c : s)
    {
        if(isdigit(c))
        {
            num = 10*num + c - '0';
        }
        else
        {
            rst += sign*num;
            num = 0;
            if (c == '+')
            {
                sign = 1;
            }
            else if( c == '-')
            {
                sign = -1;
            }
            else if( c == '(')
            {
                opnd.push(rst);
                optr.push( sign);
                rst = 0;
                sign = 1;
            }
            else if( c == ')' && optr.size())
            {
                rst = rst*optr.top() + opnd.top() ;
                opnd.pop();
                optr.pop();
            }
            
        }

    }
    return (rst + sign* num);
}

int calculate3(string s)
    {
        stack<int> nums, ops;
        long long num = 0;
        int rst = 0;
        int sign = 1;
        for (char c : s)
        {
            if (isdigit(c))
            {
                num = num * 10 + c - '0';
            }
            else
            {
                rst += sign * num;
                num = 0;
                if (c == '+')
                    sign = 1;
                if (c == '-')
                    sign = -1;
                if (c == '(')
                {
                    nums.push(rst);
                    ops.push(sign);
                    rst = 0;
                    sign = 1;
                }
                if (c == ')' && ops.size())
                {
                    rst = ops.top() * rst + nums.top();
                    ops.pop();
                    nums.pop();
                }
            }
        }
        rst += sign * num;
        return rst;
    }

};

int main()
{
    //string input = "(1+(4+5+2)-3)+(6+8)";
    //string input = " 2-1 + 2 ";
    string input = "(1 +10) +(1-1)";
    std::cout << " result is : " << Solution().calculate2(input) << std::endl;
}
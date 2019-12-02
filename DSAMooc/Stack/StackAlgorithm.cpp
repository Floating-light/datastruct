#include <iostream>
#include <stack>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

#include "Stack.hpp"
#include "StackAlgorithm.hpp"

void StackAlgorithm::convert(LStack<char>& s, int n, int base)
{
    static char digit[] = {'0', '1', '2', '3', '4','5', '6', '7', '8', '9','A', 'B', 'C', 'D', 'E','F'};
    while(n > 0 )
    {
        s.push(digit[int(n%base)]);
        n /= base;
    }
}

//another version use switch in ./leetcode/group_stack/bracketMatching/bracketMatching.cpp 
bool StackAlgorithm::parenthesisMatching(const char exp[] , int low, int hight)
{
    std::stack<char> stk;
    for(int i = low; i < hight; ++i)
    {
        if( exp[i] == '(' )
        {
            stk.push(exp[i]);
        }     
        else if (!stk.empty())
        {
            stk.pop();
        }
        else
        {
            return false;
        }
    }
    return stk.empty();
}
struct Queen
{
    int x, y;
    Queen(int xx, int yy):
        x(xx), y(yy){}
    bool operator==(Queen const & q) const 
    {
        return (x == q.x)
                || (y == q.y)
                || (x + y == q.x + q.y)
                || (x - y == q.x - q.y);
    }
    bool operator!=(Queen const & q) const 
    {
        return !(*this == q);
    }
};

void StackAlgorithm::NQueensV1(int N)
{
    std::vector<Queen> solu;
    
    Queen q(0, 0);
    int count = 1;
    while( 0 < q.x || q.y < N) // the first row is out of border, q.x == 0 && q.y == N.
    {
        if(N <= solu.size() || N <= q.y)// backtracking
        {
            q = *(solu.end() - 1);
            solu.pop_back();
            q.y++; // turn to the next column of last row .
        }
        else //probing
        {
            while(q.y < N && std::find(solu.cbegin(), solu.cend(), q) != solu.cend())//find int current row
            {
                q.y++;
            }
            if(q.y < N) // if find turn to next row, otherwise will backtracking next.
            {
                solu.push_back(q);
                if(solu.size() == N)
                {
                    std::cout << "--->" << count++ << ": ";
                    std::for_each(solu.begin(), solu.end(), [](const Queen& item)
                    {
                        std::cout  << item.y << ", " ;
                    });
                    std::cout << std::endl;
                    
                }
                q.x++;
                q.y = 0;
            }
        }
    } 
    int i = 0;
}
std::vector<std::vector<int>> StackAlgorithm::NQueensV2(int N)
{

}

float StackAlgorithm::evalute(char* s, std::string& RPN)
{
    std::stack<char> optr;
    std::stack<float> opnd;
    optr.push('\0');
    while(!optr.empty())
    {
        if(isdigit(*s))
        {
            opnd.push(readNumber(s, RPN));// read number in s, and push to RPN.
        }
        else // current is a operator
        {
            char op;
            switch (orderBetween(optr.top(), *s))
            {
            case '<':
                optr.push(*s);
                ++s;
                break;
            case '=':
                optr.pop();
                ++s;
                break;
            case '>':
                op = optr.top();
                optr.pop();
                RPN.push_back(op);
                if(op == '!') // 
                {
                    const float currOpnd = opnd.top();
                    opnd.pop();
                    opnd.push(calculate(op, currOpnd));
                }
                else
                {
                    const float currOpndRight = opnd.top();
                    opnd.pop();
                    const float currOPndLeft = opnd.top();
                    opnd.pop();
                    opnd.push(calculate(currOPndLeft, op, currOpndRight));
                }
                break;
            default:
                exit(1008611);
            } // switch
        } // if else
    } // while
    return opnd.top();
}

float StackAlgorithm::evaluteRPN(const std::string& RPN)
{
    std::stack<float> opnd;
    int i = 0;
    while( i < RPN.size())
    {
        if(isdigit(RPN[i]))
        { 
            std::string temp;
            while(RPN[i] != ' ')
            {
                temp.push_back(RPN[i]);
                ++i;
            }
            opnd.push(atof(temp.c_str()));
            ++i;
        }
        else
        {
            char op = RPN[i];
            if(op == '!') // 
                {
                    const float currOpnd = opnd.top();
                    opnd.pop();
                    opnd.push(calculate(op, currOpnd));
                }
                else
                {
                    const float currOpndRight = opnd.top();
                    opnd.pop();
                    const float currOPndLeft = opnd.top();
                    opnd.pop();
                    opnd.push(calculate(currOPndLeft, op, currOpndRight));
                }
            ++i;
            //TODO
        }
    }
    return opnd.top();
}

float StackAlgorithm::readNumber(char* &s, std::string& RPN)
{
    std::string numStr;
    while(isdigit(*s) || *s == '.')
    {
        numStr.push_back(*s);
        RPN.push_back(*s);
        ++s;
    }
    RPN.push_back(' ');

    return atof(numStr.c_str());
}

char StackAlgorithm::orderBetween(char stackTop, char current)
{
    return pri[optrToEnumMap[stackTop]][optrToEnumMap[current]];
}

float StackAlgorithm::calculate(char optr, float opnd)
{
    float result;
    if(optr == '!')
    {
        int factorial_step = int(opnd);//opnd > 0
        int factorial_result = 1;
        while(factorial_step > 0) 
        {
            factorial_result *= factorial_step--;
        }
        result = factorial_result;
    }
    return result;
}

float StackAlgorithm::calculate(float opndLeft, char optr, float opndRight)
{
    float result;
    switch (optr)
    {
    case '+':
        result = opndLeft + opndRight;
        break;
    case '-':
        result = opndLeft - opndRight;
        break;
    case '*':
        result = opndLeft * opndRight;
        break;
    case '/':
        result = opndLeft / opndRight;
        break;
    case '^':
        result = pow(opndLeft, opndRight);
        break;
    default:
        break;
    }
    return result;
}


int main()
{
    StackAlgorithm alg;
    /* int n,base;
    std::cout << std::endl << "input n and base < 16: ";
    while(std::cin >> n >> base)
    {
        LStack<char> stack;
        alg.convert(stack, n, base);
        std::cout << std::endl;
        while(stack.size() > 0)
        {
            std::cout << stack.pop();
        }
        std::cout << std::endl << "input n and base < 16: ";
    } */
    // char a[] = {'(',')', '(', '(',')',')', ')'};
    //bool result = alg.parenthesisMatching(a, 0, 7);
    //std::cout << "result : " << result << std::endl;
    int n ;
    std::cout << "Input the number of queen : ";
    while(std::cin >> n)
    {
        std::cout << std::endl;
        alg.NQueensV1(n);
        std::cout << "Input the number of queen : ";
    }
    
}
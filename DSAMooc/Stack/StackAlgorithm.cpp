#include <iostream>
#include <stack>
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
    while( 0 < q.x || q.y < N)
    {
        if(N <= solu.size() || N <= q.y)// back
        {
            q = *(solu.end() - 1);
            solu.pop_back();
            q.y++;
        }
        else
        {
            while(q.y < N && std::find(solu.cbegin(), solu.cend(), q) != solu.cend())//try 
            {
                q.y++;
            }
            if(q.y < N)
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
    char a[] = {'(',')', '(', '(',')',')', ')'};
    bool result = alg.parenthesisMatching(a, 0, 7);
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
#include <iostream>

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


int main()
{
    StackAlgorithm alg;
    int n,base;
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
    }
}
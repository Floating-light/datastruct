#include <stack>
#include <iostream>
#include <unordered_map>

bool isValid(std::string s)
{
    std::unordered_map<char, char> backetsPair {{')','('}, {'}','{'}, {']','['} };
    std::stack<char> stk;
    for(char c : s)
    {
        if(backetsPair[c] == 0)
        {
            stk.push(c);
        }
        else if(!stk.empty() && backetsPair[c] == stk.top())
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

int main()
{
    std::unordered_map<char, char> backetsPair {{')','('}, {'}','{'}, {']','['} };
    std::string input{"]"};
    std::cout << "-->" << isValid(input) << "<--" << std::endl;
}
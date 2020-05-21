#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<char> people = {'A', 'B', 'C', 'D'};
    auto condition = [](char thisMan){
        return ((thisMan != 'A') + 
               (thisMan == 'C') +
               (thisMan =='D') +
               (thisMan != 'D')) == 3;
    };

    for(char c : people)
    {
        if(condition(c) )
            cout << c << " is the man who do the good things!" << std::endl;
    }
}
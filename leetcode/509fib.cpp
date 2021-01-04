#include <vector>
#include <iostream>

using namespace std;

int fib(int n )
{
    if(n == 0) return 0;
    if(n == 1 || n == 2) return 1;
    vector<int>cache(31, -1);
    cache[0] = 0;
    cache[1] = 1;
    cache[2] = 1;
    for(int i = 3; i <= n; ++i)
    {
        cache[i] = cache[i - 1] + cache[i-2];
    }
    std::cout << std::endl;

    for(int i = 0; i <= n; i++)
    {
        std::cout << cache[i] << ", ";
    }
    std::cout << std::endl;
    return cache[n];
}

int main()
{
    fib(30);
}
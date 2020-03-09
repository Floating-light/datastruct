#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Dynamic Programming
class DynamicProgramming
{
public:
	// Fibonacci
	int fib(int n)
	{
		return (2 > n) ? n : fib(n - 1) + fib(n - 2);
	}
};

int main()
{

}
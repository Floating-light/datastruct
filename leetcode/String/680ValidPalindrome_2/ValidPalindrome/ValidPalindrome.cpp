// ValidPalindrome.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

using namespace std;

static int n = []() {std::ios::sync_with_stdio(false); std::cin.tie(nullptr); return 0; }();
class Solution1 { // double 100%

    string str;
public:
    bool isPalindrome(int left, int right)
    {
        for (; left < right && str[left] == str[right]; ++left, --right);
        if (left >= right) return true;
        else return false;
    }
    bool validPalindrome(string s) {
        str = std::move(s);
        int i = 0;
        int j = str.size() - 1;
        for (; i < j && str[i] == str[j]; --j, ++i);
        return isPalindrome(i + 1, j) || isPalindrome(i, j - 1);
    }
};

class Solution1 {

    string str;
    bool delTimes = false;
public:
    bool isPalindrome(int left, int right)
    {
        if (left >= right) return true;
        if (str[left] == str[right])
            return isPalindrome(left + 1, right - 1);
        else
        {
            if (!delTimes) // 解决仅出现一次的条件，分两次循环
            {
                delTimes = true;
                return isPalindrome(left + 1, right) || isPalindrome(left, right - 1);
            }
            else
            {
                return false;
            }
        }
    }
    bool validPalindrome(string s) {
        str = std::move(s);
        return isPalindrome(0, str.size() - 1);
    }
};

int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <iostream>

using namespace std;
class Solution
{
    vector<string> result;
    int n;
    void stackPermutation(const int& countLeft, const int& countRight, string path)
    {
        if(path.size() == 2*n)
        {
            result.push_back(path);
            return;
        }
            
        if(countLeft+1 <= n)
        {
            path.push_back('(');
            stackPermutation(countLeft + 1, countRight, path);
            path.pop_back();
        }
        if( countRight + 1 <=countLeft  )
        {
            path.push_back(')');
            stackPermutation(countLeft, 1+countRight, path);
        }
    }
public:
    vector<string> generateParenthesis(int n) {
        if(n == 0)
            return vector<string>{};
        this->n = n;
        stackPermutation(0, 0, "");
        return result;
    }
};

int main() 
{
    vector<string> res = Solution().generateParenthesis(10);
    std::cout << "result: " << std::endl;
    for (int i = 0; i< res.size(); ++i)
    {
        std::cout << i+1 << ": " << res[i] << std::endl;
    }
}
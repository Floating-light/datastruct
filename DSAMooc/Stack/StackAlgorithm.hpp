#include <vector>

template <typename T>
class LStack;

class StackAlgorithm
{
public:
    void convert(LStack<char>& s, int n , int base);

    bool parenthesisMatching(const char exp[] , int low, int hight);
    
    void NQueensV1(int N);
    std::vector<std::vector<int>> NQueensV2(int N);
    
};

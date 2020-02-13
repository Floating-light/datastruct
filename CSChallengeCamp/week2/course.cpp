#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

class Solution
{
    public:
    void quickSelect( vector<int>& A, size_t k)
    {
        
    }
};
int main()
{
    vector<int> in{1,2,8,4,3,5,9,6,7} ;   
    std::make_heap(in.begin(), in.end());
    for( auto i : in)
    {
        cout << i << " ";
    }
    cout << endl;
}
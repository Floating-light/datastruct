#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
using namespace std;

class Solution {

public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> res;
        stringstream ss;
        for(const  string& str: strs)
        {
            char  arr[26] = {0};
            for( const char& c : str)
            {
                ++arr[c-97];
            }
            ss.clear();
            for( int i = 0; i < 26; ++i )
            {
                ss << "|" << arr[i];
            }
            res[ss.str()].push_back(str);
            ss.str("");
        }
        vector<vector<string>> result;
        for( auto itr = res.cbegin(); itr != res.cend(); ++itr)
        {
            result.push_back(itr->second);
        }
        return result;
    }
};

int main() 
{
    vector<string> input = {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<vector<string>> result = Solution().groupAnagrams(input);
    for(int i = 0; i < result.size(); ++i)
    {
        std::cout << i+1 << ": " ; 
        for (auto item : result[i])
        {
            std::cout << item << ", " ; 
        }
        std::cout << std::endl;
    }
}
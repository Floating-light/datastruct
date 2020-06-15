#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    string longestWord(vector<string>& words) {
        sort(words.begin(), words.end());
        string longestWord = "";
        for(std::vector<string>::const_iterator itr = words.cbegin(); itr != words.cend(); ++itr)
        {
            if(itr == words.cbegin()) continue;
            if(string(itr->begin(), itr->end()-1) == *(itr-1) && itr->size() > longestWord.size())
            {
                longestWord = *itr;
            }
        }
        return longestWord;
    }
};

class Solution2 {
public:
    string longestWord(vector<string>& words) {
        sort(words.begin(), words.end());
        unordered_map<string, bool> occu;
        string longestWord = "";
        for(std::vector<string>::const_iterator itr = words.cbegin(); itr != words.cend(); ++itr)
        {
            
            if(occu[string(itr->begin(), itr->end()-1)] || (itr->size() == 1))
            {
                occu[*itr] = true;
                longestWord = itr->size() > longestWord.size() ? *itr : longestWord;
            }
        }
        return longestWord;
    }
};

int main()
{
    std::vector<string> input = { "w","wo","wor","worl", "world"};
    std::cout << Solution().longestWord(input) << std::endl;

}
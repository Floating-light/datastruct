#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 面试题 17.13. Re-Space LCCI
// https://leetcode-cn.com/problems/re-space-lcci/

class Solution {
    std::unordered_map<std::string, bool> d;
    vector<int> maxNum;
    void dpfunc(const string& sentence, int b)
    {
        maxNum[b] = maxNum[b+1];
        for(int i = 1; i <= sentence.size() - b; ++i)
        {
            string sub = sentence.substr(b, i );
            if(d[sub])
            {
                maxNum[b] = std::max(maxNum[b], maxNum[b + i] + 1);
            }
        }
    }
public:
    int respace(vector<string>& dictionary, string sentence) {
        for( auto& item : dictionary)
        {
            d[item] = true;
        }
        maxNum = vector<int>(sentence.size(), -1);
        maxNum[sentence.size()] = 0;
        for(int i = sentence.size() - 1; i >=0; --i)
        {
            dpfunc(sentence, i);
        }
        return maxNum[0];
    }
};

int main()
{
    // string str("abcdefghijklmn");
    // int cur = 3;
    // for(int i = 1; i <= str.size() - cur; ++i)
    // {
    //     std::cout << "i = " << i << ", substr: "<< str.substr(cur, i) << std::endl;
    // }
    vector<string> dict = {"looked","just","like","her","brother"};
    string sent = "jesslookedjustliketimherbrother";
    Solution s;
    std::cout << s.respace(dict, sent) << std::endl;
}
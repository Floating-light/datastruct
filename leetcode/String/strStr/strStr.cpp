// 28. Implement strStr()
// tags: Two Pointers, String
// 找到目标子串出现的第一个位置
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

class Solution {
public:
    // BF
    int strStr(string haystack, string needle) {
        if( needle.size() == 0)
            return 0;
        for( int i = 0; i < haystack.size(); i++ )
        {
            for(int j = 0; j < needle.size(); j++)
            {
                if( haystack[i] == needle[j] )
                {
                    if( j == needle.size() - 1)
                        return i - j;
                    ++i; 
                }
                else
                {
                    i -= j;
                    break;
                }
            }
        }
        return -1;
    }
    // sunday algorithm
    int strStrV2(string haystack, string needle) {
        if( needle.size() == 0)
            return 0;
        int i = 0;
        int j = 0;
        const int haySize = haystack.size();
        const int needSize = needle.size();
        while(i < haySize)
        {
            while( i < haySize && j < needSize && haystack[i] == needle[j])
            {
                ++i;
                ++j;
            }
            if( j == needSize)
                return i - needSize;
            const int nextHay = i - j + needSize;
            if(nextHay >= haySize) return -1;
            size_t res = needle.find_last_of(haystack[nextHay]);
            if( res == string::npos)
            {
                i = nextHay;
            }
            else
            {
                i = nextHay - res; 
            }
            j = 0;
        }
        return -1;
    }
    int strStrV3(string haystack, string needle) {
        if( needle.empty())
            return 0;
        size_t res = haystack.find(needle);
        if(res == string::npos)
            return -1;
        else
            return res;
    }
};

int main() {
    //int result = Solution().strStrV2("mississippi", "issi");
    //int result = Solution().strStrV2("hello", "ll");
    //int result = Solution().strStrV2("aaaaa", "bba");
    int result = Solution().strStrV2("bbaaaaa", "bba");
    //int result = Solution().strStrV2("aaabaaabbbabaa", "babb");
    std::cout << "result: " << result << std::endl;
}
// 58. Length of Last Word
// tags: String
#include <iostream>
#include <string>

using namespace std;

class Solution {
public:
    int lengthOfLastWord(string s) {
        if(s.empty() || s == " ") return 0;
        while(s.back() == ' ') 
            s.pop_back();
        int preEmpty = -1;
        for(int i = 0; i< s.size(); ++i)
        {
            if(s[i] == ' ') preEmpty = i;
        }    
        return s.size() - preEmpty - 1;
    }
    int lengthOfLastWord2(string s) {
        if(s.empty() || s == " ") return 0;
        int taril = s.size() - 1;
        while(taril >=0 && s[taril] == ' ')
            --taril;
        int head = taril;
        while(head >= 0 && s[head] != ' ')
            --head;
        return taril == -1 ? 0 : taril - head;
    }
};

int main() 
{
    string s = "b a  ";
    std::cout << Solution().lengthOfLastWord(s) << std::endl;
}
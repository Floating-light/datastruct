#include <string>
#include <stack>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Solution {
private:
    string result;
    string::const_iterator itr;

    void readWord(string::const_iterator& itr, const string::const_iterator& end)
    {
        result.push_back('/');
        while(*itr != '/' )
        {
            result.push_back(*itr++);
        }
    }
public:
    string simplifyPath(string path) {
        if(path.back() != '/') {
            path.push_back('/');
        }
        itr = path.cbegin();
        const string::const_iterator end = path.cend();
        while( itr != end)
        {
            const char curChar = *itr;
            if(curChar == '.')
            {
                const char next = *(itr + 1) ;
                if(next == '/') // "./"
                   itr+=2;
                else if( next == '.' && *(itr + 2) == '/')// "../"
                {
                    itr += 3;
                    if(!result.empty())
                    {
                        while(result.back() != '/')
                            result.pop_back();
                        result.pop_back();
                    }
                }
                else // ".asd..fw.e../" || "..as..d./"
                {
                    readWord(itr, end);
                    ++itr;
                }
            }
            else if( curChar == '/')
            {
                ++itr;
            }
            else
            {
                readWord(itr, end);
            }
        }
        if( result.empty() )
            result.push_back('/');
        return result;
    }
};

string simplifyPath(string path) {
   stringstream is(path);
   vector<string> strs;
   string res = "", tmp = "";
   while(getline(is, tmp, '/')) {
       if(tmp == "" || tmp == ".")
           continue;
       else if(tmp == ".." && !strs.empty())
           strs.pop_back();
       else if(tmp != "..")
           strs.push_back(tmp);
   }
   for(string str:strs) 
       res +=  "/" + str;
   if(res.empty())
       return "/";
   return res;
}

int main() 
{
    vector<string> inputs {
        "/a/./b/../../c/",  //"/c"
        "/a/../../b/../c//.//", // "/c"
        "/...", // "/..."
        "/...as/s.sdf../../..asdf../asf/./", // "/...as/..asdf../asf"
        "/a//b////c/d//././/..", // "a/b/c"
        ".", // 
        "/.hidden"
    };
    for(int i = 0 ; i < inputs.size(); ++i)
    {
        string result = Solution().simplifyPath(inputs[i]);    
        std::cout << "input[" << i << "]: " << inputs[i] << std::endl;
        std::cout << "------->: " << result << std::endl;   
    }
}
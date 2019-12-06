#include <string>
#include <stack>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

class Solution {
    struct Item
    {
        int id;
        bool isStart;
        int time;
    };
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> result(n,0);
        vector<string>::const_iterator itr = logs.cbegin();
        const vector<string>::const_iterator end = logs.cend();
        stack<Item> stk;
        stringstream in;

        string temp;
        const char splitor = ':';
        Item pre;
        while(itr != end)
        {
            Item cur;
            in.clear();
            in << *itr;
            getline(in, temp, splitor);
            cur.id = atoi(temp.c_str());
            getline(in, temp, splitor);
            cur.isStart = (temp == "start");
            getline(in, temp, splitor);
            cur.time = atoi(temp.c_str());
            
            if(stk.empty())
            {
                stk.push(cur);
                pre = cur;
                continue;
            } 

            if(!cur.isStart && !pre.isStart) // end -- end
            {
                result[cur.id] += cur.time -pre.time;
                stk.pop();
            }
            else if(!cur.isStart && pre.isStart) // start -- end
            {
                result[pre.id] += cur.time -pre.time +1;
                stk.pop();
            }
            else if( pre.isStart && cur.isStart) // start -- start
            {
                result[pre.id] += cur.time -pre.time;
                stk.push(cur);
            }
            else // end -- start
            {
                Item t = stk.top();
                result[t.id] += cur.time - pre.time - 1;
                stk.push(cur);
            }
            pre = cur;
            ++itr;
        }
        return result;
    }
};

int main() 
{
    string t = "12:start:666";
    vector<string> input{
        "0:start:0", // 2
        "1:start:2",
        "1:end:5", // 4
        "2:start:6", // 3
        "1:start:9",
        "1:end:13", // 5
        "2:end:15", // 2
        "0:start:18",
        "0:end:20", // 5
        "0:end:22"}; // 2
    vector<int> result = Solution().exclusiveTime(3, input);
    for (int i = 0; i < result.size(); i++)
    {
        std::cout << "function " << i << " total execute time : " << result[i] << std::endl;
    }
}
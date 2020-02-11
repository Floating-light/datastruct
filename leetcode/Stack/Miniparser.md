# 385. Mini Parser

* Link: https://leetcode-cn.com/problems/mini-parser/

## 分析

与`DecodeString`类似，这种解析器包含了嵌套的结构，适合用递归,分治或者栈来解决。

### 1. 递归,分治

在一个方括号中以逗号分割的每一个元素都可能是一个数字或者是另一个方括号，基于此，我们可以以逗号为分界点，递归迭代每一个元素，直到遇到数字，或者是空括号。

```c++
// 8ms beat 100%
// 使用move之后，运行时间从20+ms降到8ms
class Solution {
public:
    NestedInteger deserialize(string s) {
        if(s == "") return NestedInteger(); // ""
        if(s[0] != '[') return NestedInteger(stoi(s));// number
        if(s[1] == ']') return NestedInteger();// []
        NestedInteger res;
        int brackets = 0;
        int last = 1;
        for(int i = 1; i < s.size(); ++i)
        {
            if((s[i] == ',' && !brackets) || (i == s.size()-1)) 
            {
                // 使用移动构造器很关键，直接从20+ms 到8ms
                res.add(std::move(deserialize(s.substr(last, i-last))));
                last = i+1;
            }
            else if(s[i] == '[') ++brackets;
            else if(s[i] == ']') --brackets;
        }
        return res;
    }
};
```

### 2. 栈

我们需要维护两个栈，一个保存遍历过程中创建的`NestedInteger`，另一个保存当前括号中的元素个数。

```c++
// 12ms beat 97.36%
class Solution12_itr {
    int readNum(string::iterator& itr)
    {
        string::iterator begin = itr;
        while(isdigit(*itr) || *itr == '-')
        {
            ++itr;
        }
        return stoi(string(begin, itr));
    }
public:
    NestedInteger deserialize(string s) {
        stack<int> stk;
        vector<NestedInteger> res;
        for(auto itr = s.begin(); itr != s.end();)
        {
            if(isdigit(*itr) || *itr == '-') 
            {
                res.push_back(std::move(NestedInteger(readNum(itr))));
                continue;
            }
            else if( *itr == '[')
            {
                if(*(itr+1) == ']') // 一对空的方括号
                {
                    res.push_back(NestedInteger());
                    ++itr;
                }
                else
                {
                    // 不是空的就至少有一个
                    stk.push(1); // 这对方括号的元素个数
                }
            }
            else if(*itr == ',') // 多一个','后面就多一个元素
            {
                ++stk.top();
            }
            else // ']'
            {
                int curNiNum = stk.top();

                NestedInteger realNest = NestedInteger();
                while(curNiNum != 0)
                {
                    realNest.add(std::move(res[res.size() - curNiNum]));
                    curNiNum--;
                }
                res.erase(res.end() - stk.top(), res.end());
                res.push_back(std::move(realNest));

                stk.pop(); // 元素个数
            }   
            ++itr;
        }
        return res.back();
    }
};
```

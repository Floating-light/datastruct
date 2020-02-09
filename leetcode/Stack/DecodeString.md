# 394. Decode String
* Link: https://leetcode-cn.com/problems/decode-string/

### 分析

编码规则: `k[encoded_string]`,表示将`encoded_string`重复k次。

```
"3[a]2[bc]" = "aaabcbc".
"3[a2[c]]" = "accaccacc".
"2[abc]3[cd]ef" = "abcabccdcdcdef".
"xxxxx3[a]yyyy2[bc]zzzz" = xxxxxaaayyyybcbczzzz.
```

其中的特殊情况：
* `[]`中字母长度不定
* `[]`中可以嵌套出现字母和编码串
* 编码字符直接可能有正常表达的字符
* 表示重复次数的数字可能有多位。

当从左往右扫描输入串时，前一步的结果与后一步的结果相关，容易想到可以用`栈`来解决。或是由`括号匹配`或`计算器`等问题也容易想到。

我们需要维护两个栈，`res`字符创栈用来保存字母和`[`, `times`栈用来保存方括号中的字母的出现次数。

1. 从左往右扫描输入串`s`。
2. 遇到数字，则尝试读取与它相邻的所有数字组成的十进制数，`push`进`times`中。
3. 遇到`]`, 则从`res`栈中找到最靠后的`[`，它到末尾的子串就是当前要重复的字符串，而`times`栈顶的数字则是这个子串要重复的次数。然后我们需要删除`res`中从最后一个`[`开始的字符串，再将这个子串重复`times.top()`次添加到`res`末尾。
4. 遇到其它字符，直接`push`进`res`即可。

### 代码

```c++
class Solution {
    //题目保证数字不会出现在末尾
    int readNum(string::const_iterator& itr)
    {
        int i = 0;
        while(isdigit(*itr))
        {
            i = i * 10 + (*itr-48);
            ++itr;
        }
        return i;
    }
public:
    string decodeString(string s) {
        string res;
        stack<int> times;
        for(auto itr = s.cbegin(); itr != s.cend();)
        {
            if(isdigit(*itr)) 
            {
                times.push(readNum(itr));
                continue;
            }
            else if(*itr == ']') 
            {
                // 题目保证[]前一定有数字
                int curTimes = times.top();
                times.pop();

                const int ind = res.find_last_of('[');
                string temp = res.substr(ind+1); // 需要重复的子串
                res.erase(ind); // 删除最后一个`[`到末尾的子串
                while(curTimes != 0) 
                {
                    res += temp; // 重复添加
                    --curTimes;
                }
            }
            else
            {
                res.push_back(*itr);
            }
            ++itr;
        }
        return res;
    }
};
```
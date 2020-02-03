# 402. Remove K Digits

* Link: https://leetcode-cn.com/problems/remove-k-digits/

> 要想使移除k个元素后的数最小，则应该移除最靠左的k个相邻逆序对，包括在一次移除后形成的新的逆序对。

* 逆序对只移除左边的大者即可。

我们可以先从左往右遍历num字符串，并用一个栈保存遍历过的单调不减的元素。当栈顶元素大于当前元素num\[i\]时,即形成了一个逆序对，将栈顶元素出栈即可。
1. 每次出栈后，还需要判断新的栈顶元素与当前元素的关系，所以不能++i。同时，由于移除了一个元素，需要将k-1.
2. 完成主体循环后，需要处理各种特殊情况:
    1. 如果是i == 0结束的循环， 则 k 可能不等于0, 移除掉stk末尾k个元素.
    2. 如果是k == 0结束的循环, 则 i 可能不等于0, 需要加上num中i之后的元素.
    3. 去除掉开头的零，同时保证在全是零的情况下返回一个0；
```c++
// 执行用时 : 0 ms, 在所有 C++ 提交中击败了 100%的用户
// 内存消耗 : 9.5 MB, 在所有 C++ 提交中击败了 31.19%的用户
class Solution {
public:
    string removeKdigits(string num, int k) {
        if(num.size() == k) return string(1, '0');
        string stk;
        int i = 0;
        while(k > 0 && i < num.size()) // 将num中的字符按规则移动到栈中
        {
            if(stk.empty() || stk.back() <= num[i])  // 直接入栈，并转而遍历下一个元素
            {
                stk.push_back(num[i]);
                ++i;
            }    
            else // stk.back() > num[i]
            {
                stk.pop_back();
                --k;
            }
        }
        // 1. 如果i == 0, 则 k 可能不等于0, 移除掉stk末尾k个元素.
        // 2. 如果k == 0, 则 i 可能不等于0, 需要加上num中i之后的元素.
        stk = stk.substr(0, stk.size() - k) + num.substr(i);

        // 移除开头的0,在全0的情况下保证至少剩下一个0.
        size_t beginIndex = 0;
        while(beginIndex < stk.size() - 1 && stk[beginIndex] == '0') ++beginIndex;
        
        return stk.substr(beginIndex);
    }
};
```
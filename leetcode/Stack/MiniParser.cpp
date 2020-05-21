#include <vector>
#include <iostream>
#include <stack>
#include <list>

using namespace std;
/**
 * // This is the interface that allows for creating nested lists.
 * // You should not implement it, or speculate(推测,投机) about its implementation
 * class NestedInteger {
 *   public:
 *     // Constructor initializes an empty nested list.
 *     NestedInteger();
 *
 *     // Constructor initializes a single integer.
 *     NestedInteger(int value);
 *
 *     // Return true if this NestedInteger holds a single integer, rather than a nested list.
 *     bool isInteger() const;
 *
 *     // Return the single integer that this NestedInteger holds, if it holds a single integer
 *     // The result is undefined if this NestedInteger holds a nested list
 *     int getInteger() const;
 *
 *     // Set this NestedInteger to hold a single integer.
 *     void setInteger(int value);
 *
 *     // Set this NestedInteger to hold a nested list and adds a nested integer to it.
 *     void add(const NestedInteger &ni);
 *
 *     // Return the nested list that this NestedInteger holds, if it holds a nested list
 *     // The result is undefined if this NestedInteger holds a single integer
 *     const vector<NestedInteger> &getList() const;
 * };
 */




class NestedInteger {
    int v;
    vector<NestedInteger> nest;
  public:
    // Constructor initializes an empty nested list.
    NestedInteger(){}
    // Constructor initializes a single integer.
    NestedInteger(int value):v(value){}
    // Return true if this NestedInteger holds a single integer, rather than a nested list.
    bool isInteger() const{ return nest.empty(); }
    // Return the single integer that this NestedInteger holds, if it holds a single integer
    // The result is undefined if this NestedInteger holds a nested list
    int getInteger() const { return v; }
    // Set this NestedInteger to hold a single integer.
    void setInteger(int value) { v = value; }
    // Set this NestedInteger to hold a nested list and adds a nested integer to it.
    void add(const NestedInteger &ni) { nest.push_back(ni); }
    // Return the nested list that this NestedInteger holds, if it holds a nested list
    // The result is undefined if this NestedInteger holds a single integer
    const vector<NestedInteger> &getList() const{ return nest; }
};

// 8ms beat 100%
class Solution {
public:
    NestedInteger deserialize(string s) {
        if(s == "") return NestedInteger(); // ""
        if(s[0] != '[') return NestedInteger(stoi(s));// number
        if(s[1] == ']') return NestedInteger();// []
        NestedInteger res;
        int squr = 0;
        int last = 1;
        for(int i = 1; i < s.size(); ++i)
        {
            if((s[i] == ',' && !squr) || (i == s.size()-1)) 
            {
                // 使用移动构造器很关键，直接从20+ms 到8ms
                res.add(std::move(deserialize(s.substr(last, i-last))));
                last = i+1;
            }
            else if(s[i] == '[') ++squr;
            else if(s[i] == ']') --squr;
        }
        return res;
    }
};

class Solution_16 {
    int readNum(string::iterator& itr)
    {
        int i = 0;
        int sign = 1;
        if(*itr == '-')
        {
            sign = -1;
            ++itr;
        }
        while(isdigit(*itr))
        {
            i = i * 10 + *itr - 48;
            ++itr;
        }
        return i*sign;
    }
public:
    NestedInteger deserialize(string s) {
        stack<int> stk;
        list<NestedInteger> res;
        for(auto itr = s.begin(); itr != s.end();)
        {
            if(isdigit(*itr) || *itr == '-') 
            {
                res.push_back(std::move(NestedInteger(readNum(itr))));
                // res.push_back(std::move(NestedInteger(readNum(itr))));
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

                NestedInteger ni;

                auto itr = res.end();
                while(curNiNum != 0) {--itr; --curNiNum;}
                ni.add(*itr);
                *itr = std::move(ni);

                auto back = itr;
                while(++back != res.end())
                {
                    itr->add(std::move(*back));
                }
                res.erase(++itr, res.end());

                stk.pop(); // 元素个数
            }   
            ++itr;
        }
        return res.back();
    }
};

// 16ms < iterator==12ms
class Solution12_index {
    int readNum(const string& s, int& i)
    {
        const int begin = i;
        while(isdigit(s[i]) || s[i] == '-')
        {           
            ++i;
        }
        return stoi(s.substr(begin, i - begin));
    }
public:
    NestedInteger deserialize(string s) {
        stack<int> stk;
        vector<NestedInteger> res;
        for(int i = 0; i < s.size();)
        {
            if(isdigit(s[i]) || s[i] == '-') 
            {
                res.push_back(std::move(NestedInteger(readNum(s, i))));
                continue;
            }
            else if( s[i] == '[')
            {
                if(s[i+1] == ']') // 一对空的方括号
                {
                    res.push_back(NestedInteger());
                    ++i;
                }
                else
                {
                    // 不是空的就至少有一个
                    stk.push(1); // 这对方括号的元素个数
                }
            }
            else if(s[i] == ',') // 多一个','后面就多一个元素
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
            ++i;
        }
        return res.back();
    }
};

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

// 16ms, beat 89.87%
class Solution_ {
    int readNum(string::iterator& itr)
    {
        int i = 0;
        int sign = 1;
        if(*itr == '-')
        {
            sign = -1;
            ++itr;
        }
        while(isdigit(*itr))
        {
            i = i * 10 + *itr - 48;
            ++itr;
        }
        return i*sign;
    }
public:
    NestedInteger deserialize(string s) {
        string stk;
        stack<int> niNum;
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
                //if(!niNum.empty())
                //        ++niNum.top();
                if(*(itr+1) == ']')
                {
                    res.push_back(NestedInteger());
                    ++itr;
                }
                else
                {
                    stk.push_back('[');
                    niNum.push(1);
                }
            }
            else if(*itr == ',')
            {
                ++niNum.top();
            }
            else // ']'
            {
                stk.pop_back();

                int curNiNum = niNum.top();
                //niNum.top();

                NestedInteger realNest = NestedInteger();
                if(curNiNum == 0)
                    res.push_back(realNest);

                while(curNiNum != 0)
                {
                    realNest.add(std::move(res[res.size() - curNiNum]));
                    curNiNum--;
                }
                res.erase(res.end() - niNum.top(), res.end());
                niNum.pop();
                res.push_back(std::move(realNest));
            }   
            ++itr;
        }
        return res.back();
    }
};

class Solution_1 {
public:
    NestedInteger deserialize(string s) {
        stack<NestedInteger> stk;
        string optr;
        for(auto itr = s.begin(); itr != s.end(); )
        {
            if(isdigit(*itr) || *itr == '-') 
            {
                int num = 0;
                int sign = 1;
                if(*itr == '-') 
                {
                    sign = -1;
                    ++itr;
                }
                while(isdigit(*itr))
                {
                    num = 10 * num + (*itr - 48);
                    ++itr;
                }
                stk.push(std::move(NestedInteger(num*sign)));
            }
            else if((*itr == '[') || (*itr == ','))
            {
                if((*itr) == '['&& *(itr+1) == ']')
                {
                    stk.push(NestedInteger());
                    ++itr;
                }
                else
                {
                    optr.push_back(*itr);
                }
                ++itr;
            }
            else if(*itr == ']')
            {
                NestedInteger ni;

                NestedInteger right = stk.top();
                stk.pop();

                if(optr.back() != ',')
                {
                    ni.add(right);
                    stk.push(ni);
                }
                else
                {
                    ni.add(stk.top());
                    stk.pop();

                    ni.add(right);

                    stk.push(ni);
                    
                    optr.pop_back();
                }
                optr.pop_back();
                ++itr;
            }
        }
        return stk.top();
    }
};


// "[123,[456,[789]]]"

// [123,456,[788,799,833],[[]],10,[]]

// "[123,456,[788,799,833],[[]],10,[]]"
int main()
{
    // string input = "[-123,[456,[789]]]";
    string input = "[123,456,[788,799,833],[[]],10,[]]";

    NestedInteger res;
    res = Solution12_index().deserialize(input);
    int i = 0;
}
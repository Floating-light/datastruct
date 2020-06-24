#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_set>

using namespace std;

class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        std::unordered_map<int, int> m;
        for(auto item : arr)
        {
            ++m[item];
        }
        std::unordered_map<int, bool> res;
        for(auto p : m)
        {
            if(res[p.second])
            {
                return false;
            }
            else
            {
                res[p.second] = true;
            }
        }
        return true;
    }
    bool uniqueOccurrences(vector<int>& arr) {
        std::unordered_map<int, int> m;
        for(auto item : arr)
        {
            ++m[item];
        }
        unordered_set<int> st;
        for(auto p : m)
        {
            st.insert(p.second);
        }
        return st.size() == m.size();
    }
};

int main()
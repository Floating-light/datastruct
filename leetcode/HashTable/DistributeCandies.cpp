#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <functional>
#include <bitset>
using namespace std;

static int n = []() {

    std::ios::sync_with_stdio(false);

    std::cin.tie(nullptr);

    return 0;

}();
class Solution {
private:
    template<typename A, typename B>
    std::multimap<B, A> flip_map(const std::map<A, B>& src)
    {
        std::multimap<B, A> dst;
        std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
            [](const std::pair<A, B>& p) 
        {
            return std::pair<B, A>(p.second, p.first);
        });
        return dst;
    }
public:
    int distributeCandies(vector<int>& candies) {
        // candy : number
        unordered_map<int, int> cand;
        for (const int & c : candies)
        {
            ++cand[c];
        }
        return std::min(candies.size() / 2, cand.size());

        //// map ≤ªƒ‹”√sort
        ////sort(cand.begin(), cand.end(), cand.value_comp());
        //std::multimap<int, int> res = flip_map(cand);
        //int total = candies.size()/2;
        //int s = 0;
        //for (auto item : res)
        //{
        //    cout << "number " << item.first << ", candy: " << item.second << std::endl;
        //    ++s;
        //    if ((total = (total - item.first)) <= 0)
        //        return s;
        //}
        //return s;
    }
    int distributeCandiesBitset(vector<int>& candies)
    {
        bitset<200001> bs;
        for(const int& item : candies)
        {
            bs.set(item + 100000);
        }
        return std::min(candies.size() / 2, bs.count());
    }
};

//int main()
//{
//    vector<int> input = { 1,1,2,2,3,3 };
//    Solution s;
//    std::cout << s.distributeCandies(input) << std::endl;
//}
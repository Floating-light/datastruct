#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

static int n = []() {

    std::ios::sync_with_stdio(false);

    std::cin.tie(nullptr);

    return 0;

}();
class Solution {
public:
    std::vector<int> intersect(std::vector<int>& nums1, std::vector<int>& nums2) {
        std::unordered_map<int, int> m1;
        auto itr = nums1.cbegin();
        while (itr != nums1.cend())
        {
            ++m1[*itr];
            ++itr;
        }
        nums1.clear();
        for (const int& item : nums2)
        {
            if (--m1[item] >= 0)
            {
                nums1.push_back(item);
            }
        }
        return nums1;
    }
};

// ½»
// std::set_intersection(first1, last1, first2, last2, OutputIterator result)
// std::set_union

// ²¹
// std::set_difference
// std::set_symmetric_difference

class Solution2 {
public:
    std::vector<int> intersect(std::vector<int>& nums1, std::vector<int>& nums2) {
        std::sort(nums1.begin(), nums1.end());
        std::sort(nums2.begin(), nums2.end());
        //nums1.erase(std::set_intersection(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), nums1.begin()),nums1.end());
        auto nextItr = std::set_intersection(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), nums1.begin());
        nums1.resize(nextItr - nums1.begin());
        return nums1;
    }
};

//int main()
//{
//    int set1[] = { 4,9,5,9 };
//    int set2[] = { 9,4,9,8,4,3,2,1,5 };
//    
//    sort(set1, set1+4, std::greater<int>()); // 9, 9, 5, 4
//    sort(set2, set2 + 9, std::greater<int>()); // 9, 9, 8,5, 4, 4, 3, 2, 1
//
//    std::vector<int> res;
//    if (std::includes(set2, set2 + 9, set1, set1 + 4, std::greater<int>()))
//    {
//        std::cout << "true" << std::endl;
//    }
//    else
//    {
//        std::cout << "false" << std::endl;
//    }
//    //auto itr = std::back_inserter(res);
//    /*auto itr = std::set_intersection(set1.begin(), set1.end(),
//        set2.begin(), set2.end(),
//        set1.begin(), std::greater<int>());*/
//    // ²¢¼¯
//        /*auto itr = std::set_union(set2, set2 + 8,
//        set1, set1 + 4,
//        std::back_inserter(res), std::greater<int>());*/
//    
//    // 9, 9, 8, 5, 4, 4, 3, 2, 1,
//    //set1.erase(itr, set1.end());
//    //set1.resize(itr - set1.begin());
//    auto itr2 = begin(res);
//    while (itr2 != end(res))
//    {
//        std::cout << *itr2 << ", ";
//        ++itr2;
//    }
//    std::cout << std::endl;
//}
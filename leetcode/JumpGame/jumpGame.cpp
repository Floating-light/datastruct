#include <vector>
#include <iostream>

using namespace std;

class Solution {
	bool jump(const vector<int>& nums, int currentIndex, vector<bool>& cache)
	{
		if (currentIndex == nums.size() - 1) return true;
        if(!cache[currentIndex]) return false;
		for (int i = 1; currentIndex + i < nums.size() &&  i <= nums[currentIndex] ; ++i)
		{
			if (jump(nums, currentIndex + i,cache)) return true;
		}
        cache[currentIndex] = false;
		return false;
	}
public:
	bool canJump(vector<int>& nums) {
        vector<bool> cache(nums.size(),true);
		if (jump(nums, 0, cache)) return true;
		return false;
	}
};

int main() 
{
    vector<int> inputs = {2,3,1,1,4};
    if(Solution().canJump(inputs)) std::cout <<"sucess yyyyyyy" << std::endl;
    else std::cout << "failed " << std::endl;
}
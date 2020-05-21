// 55. Jump Game
// tags: Greedy, Array

#include <vector>
#include <iostream>
#include <unordered_map>
using namespace std;


//OK
class Solution {
public:
	bool canJump(vector<int>& nums) {
		const int length = nums.size();
		int near = length-1;
		for(int cIndex = length  - 1; cIndex >=0; --cIndex)
		{
			if(cIndex + nums[cIndex] >= near) near = cIndex;
		}
		return near == 0;
	}

	bool canJump2(vector<int>& nums) {
		int k = 0;
		for(int i = 0; i < nums.size(); ++i)
		{
			if(i > k) return false;
			k = max(k, i + nums[i]);
		}
		return true;
	}
};

//time out
class Solution2 {
public:
	bool canJump(vector<int>& nums) {
        vector<bool> cache(nums.size(),false);
		const int length = nums.size();
		cache[length-1] = true;
		for(int cIndex = length  - 1; cIndex >=0; --cIndex)
		{
			for(int i = 1; i <= nums[cIndex]&&i + cIndex < length; ++i)
			{
				if(cache[i+cIndex]) 
				{
					cache[cIndex] = true;
					break;
				}
			}
		}
		return cache[0];
	}
};

//time out
class Solution1 {
	bool jump(const vector<int>& nums, int currentIndex, vector<bool>& cache)
	{
		if (currentIndex == nums.size() - 1) return true;
        if(!cache[currentIndex]) return false;
		for (int i = 1; currentIndex + i < nums.size() &&  i <= nums[currentIndex] ; ++i)
		{
			if(cache[currentIndex])
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
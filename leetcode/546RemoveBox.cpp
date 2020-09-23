#include <iostream>

#include <vector>
#include <algorithm>

using namespace std;

class Solution
{
public:
	int removeBegin(const std::vector<int>& boxes, int beginIndex)
	{
		int maxNumber = std::numeric_limits<int>::min();

		for (int i = 0; i < boxes.size(); ++i)
		{
			int j = i;
			while (boxes[j + 1] == boxes[i]) ++j;
			int currentNum = (j - i + 1) * boxes[i];
			std::vector<int> in(boxes.begin(), boxes.begin() + i + 1);
			std::copy(boxes.begin() + j + 1, boxes.end(), std::back_inserter(in));

			//maxNumber = std::max(maxNumber, currentNum + removeBegin(remove()
		}
	}

	int removeBoxes(std::vector<int>& boxes)
	{

	}
};

int main()
{
	std::vector<int> vec(10);
	std::cout << vec.size() << std::endl << vec.capacity() << std::endl;
	return 0;
}
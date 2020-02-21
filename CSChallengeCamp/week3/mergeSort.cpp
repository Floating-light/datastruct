#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

typedef long long LL;

class Solution
{
    void actualMerge(vector<LL>& input, int l, int mid, int r)
    {
        vector<LL> temp(input.begin()+l,input.begin()+mid+1);
        for(int k = l,i = 0,j = mid+1; i < temp.size()||j <=r; )
        {
            if(i<temp.size()&&(j>r || temp[i] <= input[j])) input[k++] = temp[i++];
            if(j <= r && (i>=temp.size() || temp[i] > input[j])) input[k++] = input[j++];
        }
    }
    public: 
    void mergeSort(vector<LL>& input, int l, int r)
    {
        if(r <= l) return ;
        int mid = (l + r) >> 1;
        mergeSort(input,l, mid);
        mergeSort(input,mid+1, r );
        actualMerge(input,l,mid,r);
    }
};


int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    int n ;
    cin >> n;
    vector<LL> input(n);
    for(int i = 0; i < n; ++i)
    {
        cin >> input[i];
    }
    Solution().mergeSort(input,0,n-1);
    for(LL item : input)
    {
        cout << item << " ";
    }
}
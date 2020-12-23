#include <vector>
#include <iostream>

void quickSort(std::vector<int>& v, int low, int high)
{
    int i = low;
    int j = high + 1;
    int key = v[low];
    while(true)
    {
        while(v[++i] < key)
        {
            if(i == high) break;
        }
        while(v[--j] > key)
        {
            if(j == low) break;
        }
        if(i >= j) break;
        int temp = v[j];
        v[j] = v[i];
        v[i] = temp;
    }
    int temp = v[low];
    v[low] = v[j];
    v[j] = temp;
    quickSort(v, low, i - 1);
    quickSort(v, j+1, high );
}
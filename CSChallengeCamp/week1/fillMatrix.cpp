#include <iostream>
#include <vector>

using namespace std;

enum Direction
{
    Default
};

class Matrix
{
private:
    int n;
    Direction direct;
    vector<vector<int>> m;
public:
    Matrix(int n = 8, Direction d = Default, int init= -1):
      n(n), direct(Default), m(n, vector<int>(n, init)) {}

    void Fill(int num, int begin, int size)
    {
        if( size == 0) return ;
        if( size == 1) m[begin][begin] = num;
        for (int j=0; j<size-1; j++)
            m[begin+j][begin+0] = num++;
        for (int j=0; j<size-1; j++)
            m[begin+size-1][begin+j] = num++;
        for (int j=size-1; j>0; j--)
            m[begin+j][begin+size-1] = num++;
        for (int j=size-1; j>0; j--)
            m[begin+0][begin+j] = num++;   
        Fill(num, begin+1, size - 2); 
    }

    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> res(n, vector<int>(n, 0));
        int h = 0, k = n;
        int num = 0;
        int i , j = 0;
        while(h < k)
        {
            for(i = h,j = h; j < k; ++j) 
                res[i][j] = ++num;
            for(i = h + 1, j= k-1; i < k; ++i) 
                res[i][j] = ++num;
            for(i = k - 1,j = k-2; j>=h; --j) 
                res[i][j] = ++num;
            for(i = k-2, j = h; i >h; --i) 
                res[i][j] = ++num;
            ++h;
            --k;
        }     
        return res;       
    }

    friend ostream& operator<<(ostream&, const Matrix&);
};

ostream& operator<<(ostream& os, const Matrix& m)
{
    os << m.n;
}
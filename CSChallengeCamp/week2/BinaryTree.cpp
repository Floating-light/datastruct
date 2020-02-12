#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <math.h>

using namespace std;

#define InHeap(n, i) (-1 < i)&&(i < n)
#define LChild
#define Bigger(H,i,j) (max(H[i], H[j]) ? i: j)

class Solution_false
{
    int properParent(const vector<int>& heap, int i )
    {
        const int n = heap.size();
        const int lchild = (i << 1) + 1;
        const int rchild = (i + 1) << 1;
        if(-1 < rchild && rchild < n)
        {
            int temp = heap[i]>= heap[lchild] ? i : lchild;
            return heap[temp]>= heap[rchild] ? temp : rchild; 
        }
        else if(-1 < lchild && lchild < n)
        {
            return (heap[i]>= heap[lchild]) ? i : lchild;
        }
        return i;
    }
    int down(vector<int>& heap, int begin)
    {
        int j;
        while(begin != (j = properParent(heap, begin)))
        {
            swap(heap[begin], heap[j]);
            begin = j;
        }
        return begin;
    }
public:
    void constructHeap(vector<int>& input)
    {
        int firstInternal = (input.size() - 2) >> 1;
        for(int i = firstInternal; i >= 0; --i)
        {
            down(input, i);
        }
    }
};  

struct Node
{
    Node(int v  = -1 , int l = -1, int r = -1 ): val(v), lchild(l), rchild(r) { }
    int val;
    int lchild;
    int rchild;
};

class BinaryTree
{
    size_t root = -1 ;
    vector<Node> t;
    public:
    BinaryTree(int nn): t(nn + 1, Node(-1,-1,-1)) { }

    void constructTree(const vector<int>& input)
    {
        root = input[0];
        for(size_t i = 1; i < input.size(); ++i)
        {
            insert(input[i], root);
        }

        ppre(root);
        cout << endl;
        suffix(root);
        cout << endl;

    }

    void insert(int e, int r)
    {
        if(e < r) t[r].lchild == -1 ? t[r].lchild = e : (insert(e, t[r].lchild),0);
        else if(r < e) t[r].rchild == -1 ? t[r].rchild = e : (insert(e, t[r].rchild), 0);
    }

    void ppre(size_t n)
    {
        if(n == -1 ) return ;
        cout << n << " ";
        ppre(t[n].lchild);
        ppre(t[n].rchild);
    }

    void suffix(size_t n)
    {
        if( n == -1 ) return ;
        suffix(t[n].lchild);
        suffix(t[n].rchild);
        cout << n << " ";
    }
};

int main()
{
    int n;
    cin >> n;
    vector<int> input(n, 0);
    for( int i= 0; i < n; ++i)
    {
        cin >> input[i];
    }
    //vector<int> input = vector<int>{2,6,9,3,5,7,10,8,4,1};
    BinaryTree solu(input.size());
    solu.constructTree(input);
}
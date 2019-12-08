#ifndef BINNODE_HPP
#define BINNODE_HPP

#include <stack>
#include <queue>

#define BinNodePosi(T) BinNode<T>*  // node position
typedef enum { RB_RED, RB_BLACK } RBColor;



template <typename T> struct BinNode
{
    T data;
    BinNodePosi(T) parent;
    BinNodePosi(T) lChild;
    BinNodePosi(T) rChild;
    int height;
    int npl; // null path length
    RBColor color; // red black tree

    // constructor
    BinNode() : parent(nullptr), lChild(nullptr), rChild(nullptr),
        height(0), npl(1), color( RB_RED) { }
    // construct from a data
    BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, 
        int h = 0, int l = 1, RBColor c = RB_RED) : 
        data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c)
        { }

    // the total number of descendant of the current node, 
    // also is the scale of the sub tree root by current node. 
    int size(); 

    //insert new data as the left child of current node
    BinNodePosi(T) insertAsLeftChild(const T& );

    //insert new data as the right child of current node
    BinNodePosi(T) insertAsRightChild(const T& );

    //get the direct succfix of current node
    BinNodePosi(T) succ();

    template <typename VST> void travLevel( VST& );
    template <typename VST> void travPre( VST& );
    template <typename VST> void travIn( VST& );
    template <typename VST> void travPost( VST& );

    template < typename VST> 
    void travPreRecursion(BinNodePosi(T) x, VST& visit);

    template < typename VST> 
    void travPreIteration1(BinNodePosi(T) x, VST& visit);

    template < typename VST> 
    void travPreIteration2(BinNodePosi(T) x, VST& visit);

    template <typename VST>
    void travInRecursion( BinNodePosi(T) x, VST& vist);

    template <typename VST>
    void travInIteration( BinNodePosi(T) x, VST& vist);

    template <typename VST>
    void travLevelRecursion( BinNodePosi(T) x, VST& vist);

    bool operator< (BinNode cosnt& bn ) { return data < bn.data ; }
    bool operator==( BinNode const& bn ) { return data == bn.data; }
};

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLeftChild(T const & e ) // assum lChild == nullptr
{
    return lChild = new BinNode<T>(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRightChild(T const & e ) // assum rChild == nullptr
{
    return rChild = new BinNode<T>(e, this);
}

template <typename T>
int BinNode<T>::size()
{
    int s = 1;
    if ( lChild ) s += lChild->size();
    if ( rChild ) s += rChild->size();
    return s;
} // O(n) = |size|

template <typename T> template <typename VST>
void BinNode<T>::travPreRecursion(BinNodePosi(T) x, VST& visit)
{
    if(!x) return;
    visit(x->data);

    // tail recursion, easy convert to iteration.
    travPreRecursion(x->lChild, visit);
    travPreRecursion(x->rChild, visit);
}

template <typename T> template <typename VST>
void BinNode<T>::travPreIteration1(BinNodePosi(T) x, VST& visit)
{
    if(!x) return;
    std::stack<BinNode<T>*> stk;
    stk.push(x);
    while(!stk.empty())
    {
        BinNode<T>* cur = stk.top();
        stk.pop();
        visit(cur->data);
        if(!x->rChild)
            stk.push(x->rChild);
        if(!x->lChild)
            stk.push(x->lChild);
    }
}

template <typename T> template <typename VST>
void BinNode<T>::travPreIteration2(BinNodePosi(T) x, VST& visit)
{
    stack<BinNode<T>*> stk;
    BinNode<T>* cur = x;
    while(true)
    {
        while(cur) // visit along left branch
        {
            visit(cur->data);
            if(cur->rChild)
                stk.push(x->rChild)
            cur = cur->lChild;
        }

        if(stk.empty())
            break;
        cur = stk.top();
        stk.pop();
    }
}

template <typename T> template <typename VST>
void BinNode<T>::travInRecursion( BinNodePosi(T) x, VST& vist)
{
    if(!x)
    {
        return ;
    }
    travInRecursion(x->lChild, visit);
    visit(x->data);
    travInRecursion(x->rChild, visit);
}

template <typename T> template <typename VST>
void BinNode<T>::travInIteration( BinNodePosi(T) x, VST& vist)
{
    stack<BinNodePosi(T)> stk;
    BinNodePosi(T) cur = x;
    while(true) // O(n)
    {
        while(cur) // go along left branch, O(n) ?
        {
            stk.push(cur);
            cur->lChild;
        }
        if( stk.empty()) return ;
        cur = stk.top();
        stk.pop();
        visit(cur->data);
        cur = cur->rChild;
    }
}

template <typename T> template <typename VST>
void BinNode<T>::travLevelRecursion( BinNodePosi(T) x, VST& vist)
{
    std::queue<BinNodePosi(T)> q;
    if(!x)
        q.push(x);
    while(!q.empty())
    {
        BinNodePosi(T) cur = q.front();
        q.pop();
        visit(cur->data);
        if(cur->lChild)
            q.push(cur->lChild);
        if(cur->rChild)
            q.push(cur->rChild);
    }
}
#endif
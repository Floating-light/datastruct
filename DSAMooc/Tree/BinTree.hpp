#ifndef BINTREE_HPP
#define BINTREE_HPP

#include "BinNode.hpp"

//if the node is null,  it has -1 height
#define stature(p) ( (p) ? (p)->height : -1)

template <typename T>
class BinTree
{
protected:
    int _size;
    BinNodePosi(T) _root;
    virtual int updateHeight( BinNodePosi(T) x);
    void updateHeightAbove( BinNodePosi(T) x);
public:
    int size() const { return _size; }
    bool empty() const { return !_root; }
    BinNodePosi(T) root()  const { return _root; }

    BinNodePosi(T) insertAsRightChild( BinNodePosi(T)x, T const & e);
    BinNodePosi(T) insertAsLeftChild( BinNodePosi(T)x, T const & e);
    // TODO: other interface
};

template <typename T>
int BinTree<T>::updateHeight( BinNodePosi(T) x)
{
    return x->height = 1+ max( stature(x->lChild), stature(x->rChild) );
} // noraml binary tree, O(1)

template <typename T>
void BinTree<T>::updateHeightAbove( BinNodePosi(T) x)
{
    while(x)
    {
        const int previous = x->height;
        updateHeight(x);
        if(x->height == previous)
            break;
        x = x->parent;
    }
} // O(n)

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRightChild(BinNodePosi(T) x, T const& e)
{
    ++_size;
    x->insertAsRightChild(e);
    updateHeight(x);
    return x->rChild;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLeftChild(BinNodePosi(T) x, T const& e)
{
    ++_size;
    x->insertAsLeftChild(e);
    updateHeight(x);
    return x->lChild;
}

#endif
#ifndef LLISTNODE_HPP
#define LLISTNODE_HPP

#define Posi(T) LListNode<T>* // The position of list node (ISO C++.0x, template alias)

template <typename T>
struct LListNode
{
    T data;
    Posi(T) pred;
    Posi(T) succ;
    LListNode() {} // constructor of header and trailer
    LListNode(T e, Posi(T) p = nullptr, Posi(T) s = nullptr)
    : data(e), pred(p), succ(s) {} // default constructor
    Posi(T) insertAsPred(T const& e); // predecessor insert 
    Posi(T) insertAsSucc(T const& e); // successor insert 
};

template <typename T>
Posi(T) LListNode<T>::insertAsPred(T const& e)
{
    Posi(T) x = new LListNode<T>(e, pred, this);// is expensive
    pred->succ = x;
    pred = x;
    return x;
}//if this is the first node, it's fine.

template <typename T>
Posi(T) LListNode<T>::insertAsSucc(T const& e)
{
    Posi(T) x = new LListNode<T>(e, this, succ);
    succ->pred = x;
    succ = x;
    return x;
}//if this is the last node, it's alse fine.

#endif // LLISTNODE_HPP
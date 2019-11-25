#ifndef LLIST_HPP
#define LLIST_HPP
#include "LListNode.hpp"

typedef int Rank;

template <typename T>
class LList
{
private: 
    int _size;
    Posi(T) header;
    Posi(T) trailer;
protected: // inner function
    void init();
    int clear();
    void copyNodes(Posi(T) p, int n);
    Posi(T) find(T const & e, int n, Posi(T) p) const;
public: // constructor and interface
    //copy constructor
    LList(){ init(); }

    LList(LList<T> const& L){copyNodes(L.first(), L._size);}
    
    //deconstructor
    ~LList(){ clear(); delete header; delete trailer; }

    //interface
    int size() { return _size; }

    Posi(T) first(){ return header->succ; }
    Posi(T) last() { return trailer->pred; }//no error if without return statement, and return a empty node. Amazing!!!

    Posi(T) insertAsFirst(T e) { insertA(header, e); };
    Posi(T) insertAsLast(T e){ insertB(trailer, e); }

    Posi(T) insertB(Posi(T) p, T const& e);
    Posi(T) insertA(Posi(T) p, T const& e);

    T remove(Posi(T) p);

    bool disordered();

    void sort();

    Posi(T) find(T const& e) const;

    Posi(T) search(T e);
    
    //For unordered list.
    //Given the element e, find it in the n nodes before  p
    //return the first element encounter, if any, otherwise return nullptr.
    int deduplicate();

    void uniquify();

    void traverse();
    
    //operator
    T& operator[](Rank r) const;//O(n), don't use
};

//implementation
template <typename T>
int LList<T>::clear()
{
    int oldSize = _size;
    while( 0 < _size)
    {
        remove(header->succ);//or trailer->pred
    }
    return oldSize;
}

template <typename T>
void LList<T>::init()
{
    header = new LListNode<T>;
    trailer = new LListNode<T>;
    header->succ = trailer;
    header->pred = nullptr;
    trailer->pred = header;
    trailer->succ = nullptr;
    _size = 0;
}

template <typename T>
void LList<T>::copyNodes(Posi(T) p, int n)
{
    init();
    while(n--)
    {
        insertAsLast(p->data);
        p = p->succ;
    }
}

template <typename T>
Posi(T) LList<T>::insertA(Posi(T) p, T const& e)
{ 
    _size++;
    return p->insertAsSucc(e);
}

template <typename T>
Posi(T) LList<T>::insertB(Posi(T) p, T const& e)
{ 
    _size++;
    return p->insertAsPred(e);
}

template <typename T>
T LList<T>::remove(Posi(T) p)
{
    T e = p->data;
    p->pred->succ = p->succ;
    p->succ->pred = p->pred;
    delete p;
    p = nullptr;
    _size--;
    return e;
}

template <typename T>
Posi(T) LList<T>::find(T const& e, int n, Posi(T) p) const//O(n)
{
    while(0 < n--)
    {
        if(e == (p = p->pred) -> data)
        {
            return p;
        }    
    }
    return nullptr;
}

template <typename T>
Posi(T) LList<T>::find(T const& e)const
{
    find(e, _size, trailer);
}

template <typename T>
int LList<T>::deduplicate()
{
    int oldSize = _size;
    LListNode<T>* p = first();
    LListNode<T>* q = nullptr;
    for ( Rank r = 0; p != trailer; p = p->succ, q = find(p->data, r, p) )
        q ? remove(q) : r++;//if find, delete q,  not p, because p = p->succ.
    return oldSize - _size;
}
//**********************************************
template <typename T>
T& LList<T>::operator[](Rank r ) const
{
    Posi(T) p = first();
    while(0 < r--)
        p = p->succ;
    return p->data;
}

#endif // LLIST_HPP
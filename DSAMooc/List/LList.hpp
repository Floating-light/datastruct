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
public: // constructor and interface
    
    Posi(T) insertB(Posi(T) p, T const& e);


    T& operator[](Rank r) const;//O(n), don't use


};

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
Posi(T) LList<T>::insertB(Posi(T) p, T const& e)
{ 
    _size++;
    return p->insertAsPred(e);
}

template <typename T>
T& LList<T>::operator[](Rank r ) const
{
    Posi(T) p = first();
    while(0 < r--)
        p = p->succ;
    return p->data;
}

#endif // LLIST_HPP
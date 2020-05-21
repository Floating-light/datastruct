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
    //unordered o(n)
    Posi(T) find(T const& e) const;
    
    // ordered, o(n), no  promote  because of call by position
    // find the last element that <= e, return left border if failed to find.
    Posi(T) search(T const& e, int n , Posi(T));
    
    // For unordered list.
    // Given the element e, find it in the n nodes before  p
    // return the first element encounter, if any, otherwise return nullptr.
    int deduplicate();

    int uniquify();
    
    // unordered list
    void traverse(void (*visit ) (T& ));// function pointer
    template <typename VST>
    void traverse(VST& vist);// function object
    
    // sort algorithm for list
    // selection sort algorithm <bubble sort>
    void selectionSort(Posi(T), int n);
    Posi(T) selectMax(Posi(T), int ); // painter's alg0rithm
    
    // insertion sort algorithm, o(n) -- o(n^2)
    // how about using vector strcut?
    void insertionSort(Posi(T),int);//in-place algorithm
    //total inversion number == compare times, input-sensitive
    
    void mergeSort(Posi(T)& p, int n);
    void merge(Posi(T)& p, int n, LList<T>& L, Posi(T) q, int m);
    // operator
    T& operator[](Rank r) const;// O(n), don't use
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

template <typename T>
int LList<T>::uniquify()
{
    if(_size < 2)
        return 0;
    int oldSize = _size;
    Posi(T) p = first()->succ;
    while(p != trailer)
    {
        if(p->data == p->pred->data)
        {
            remove(p->pred);
        }
        p = p->succ;
    } 
    return oldSize - _size;
}
/* {
    if(_size < 2)
        return 0;
    int oldSize = _size;
    Posi(T) p = first();
    Posi(T) q = nullptr;
    while((q = p->succ) != trailer)
    {
        if(p->data == q->data)
            remove(q);
        else
            p = q;
    }
    return oldSize - _size;
} */

template <typename T>
void LList<T>::traverse(void (*visit ) (T& ))
{
    Posi(T) p = header;
    while((p = p->succ) != trailer)
    {
        visit(p->data);
    }
}

template <typename T> template <typename VST>
void LList<T>::traverse(VST& vist)
{
    Posi(T) p = header;
    while((p = p->succ) != trailer)
    {
        visit(p->data);
    }
}

template <typename T> 
Posi(T) LList<T>::search(T const& e, int n , Posi(T) p)
{
    /* do 
    {
        p = p->pred;
        n--;
    }
    while((-1 < n) && (e<p->data)); */
    while(0 <= n--)
    {
        if( ( (p = p->pred) ->data) <= e) break;
    }
    return p;
}

template <typename T>
void LList<T>::selectionSort(Posi(T) p, int n )// o(n^2), big reduce on move operation, almost spend on compare operation
{
    Posi(T) head = p->pred;
    Posi(T) tail = p;
    for(int i  = 0; i < n; i++) tail = tail->succ;
    while(1 < n)
    {
        //if(tail->pred != selectMax(head->succ, n)) // if max node exactly before tail, this exchange is not nesserary.
                                                     // but this is not a optimization, normally this case is rare encounter.
        insertB(tail, remove(selectMax(head->succ, n)));// 3-14
                                                        // new and delete is too expensive, 100 times.
                                                        
        tail = tail->pred;
        n--;
    }
}

template <typename T>
Posi(T) LList<T>::selectMax(Posi(T) p , int n)
{
    Posi(T) max = p;
    if( !lt((p = p->succ)-> data, max->data))// >=, how about >
    {
        max = p;
    }
    return max;
}

template <typename T>
void LList<T>::insertionSort(Posi(T) p, int n)//inplace algorithm
{
    for(int i = 0; i < n; ++i)
    {
        insertA(search(p->data, i, p), p->data);
        p=p->succ;
        remove(p->pred);
    }
}

template <typename T>
void LList<T>::mergeSort(Posi(T)& p, int n)
{
    if ( n < 2) return ;
    Posi(T) q = p;
    int m = n >> 1;
    for(int i = 0; i < m; ++m) q = q->succ;
    mergeSort(p, m);
    mergeSort(q, n - m);
    merge(p, m, *this, q, n - m);

}

template <typename T>
void LList<T>::merge(Posi(T)& p, int n, LList<T>& L, Posi(T) q, int m)
{
    Posi(T) pp = p->pred;
    while( 0 < m)
    {
        if(0 < n && p->data < q->data)
        {
            if(q == (p = p->succ) ) break;
            --n;
        }
        else
        {
            insertB(p, L.remove((q = q->succ)->pred);
            m--;
        }
    }
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
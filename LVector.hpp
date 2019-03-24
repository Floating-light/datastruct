#ifndef LVECTOR_HPP
#define LVECTOR_HPP
#include <iostream>
#include "stdlib.h"

typedef int Rank;

#define DEFAULT_CAPACITY 24

template <typename T> class LVector{
protected:
    //current element x,rank r(indicate there have r element before element x) 
    //<==> _elem[r] <==> *(_elem + r)
    //<==> physical address:_elem + r 
    T *_elem;
    Rank _size;
    int _capacity;

    void copyFrom(T const* A, Rank low, Rank high);

    

    void shrink();
public:

void expand();
    LVector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0){
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    
    ~LVector(){
        delete [] _elem;
    }
    
    //print all the element in the vector
    void print();
    LVector<T> & operator= (LVector<T> const&);
    T & operator[] (Rank);
    void permute(LVector<T>);
    void unsort(Rank low, Rank high);
    Rank find(T const& e, Rank low, Rank high);
    Rank insert(T const& e, Rank r);
};

template <typename T>
void permute(LVector<T> v){
    for(int i = v._size; i > 0; --i){
        swap(v[i-1], v[rand() % i]);
    }
}

//The tenplate class must be defined 100% within the head file,
//but we can just include the implemente in the head file to aviod that.
//usually this is a bad idea, but is convenice
//or you change the implemente file name with .impl
#include "LVector.cpp"
#endif
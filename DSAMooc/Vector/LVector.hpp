#ifndef LVECTOR_HPP
#define LVECTOR_HPP
#include <iostream>
#include "stdlib.h"
#include "assert.h"

typedef int Rank;

#define DEFAULT_CAPACITY 20

template <typename T> class LVector{
protected:
    //current element x,rank r(indicate there have r element before element x) 
    //<==> _elem[r] <==> *(_elem + r)
    //<==> physical address:_elem + r 
    T *_elem;
    Rank _size;
    int _capacity;
    void copyFrom(T const* A, Rank low, Rank high);
    void expand();
    void shrink();
public:
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

    void unsort(Rank low, Rank high);

    Rank find(T const& e, Rank low, Rank high);

    Rank insert(T const& e, Rank r);

    Rank remove(Rank low, Rank high);

    Rank remove(Rank r);
    
    int deduplicate();
    
    //Traverse by function pointer
    void traverse(void (*visit ) (T& ));
    
    //Traverse by funcion object,the type TVS must overloaded () operator
    template <typename TVS> void traverse(TVS& );

    //Check whether is ordered
    //const member function,this function can not change any member varibale(except mutable varibale)
    //also can not call any non-const member function
    int disordered() const;
    
    //complixty is o(n)
    //if remove element one by one when traversing the _elem//   ,
    //the complixty will be o(n^2)
    int uniquify() ;

    //o(n^2) implement
    int uniquifySlowly();
    
    Rank search(T const& e, Rank low, Rank high);

    Rank search(T const& e);
    
    //unified sort entrance
    void sort(Rank low, Rank high);
    
    void mergeSort(Rank low, Rank high);

    void merge(Rank low, Rank mid, Rank high);
    //Static memebr is independent of any object,instead is assosiate with class itself
    //So static memeber function have not this pointer,and can't access non-static member
    //All the static data member is shared by all class instance.
    Rank binSearch(T* A, T const& e, Rank low, Rank high);
    Rank binSearch_B(T* A, T const& e, Rank low, Rank high);
    Rank binSearch_C(T* A, T const& e, Rank low, Rank high);

    Rank fibSearch(T* A, T const& e, Rank low, Rank high);
    
    void bubbleSort(Rank low, Rank high);
    bool bubble(Rank low, Rank high);
};
/*
template <typename T>
void permute(LVector<T> v){
    for(int i = v._size; i > 0; --i){
        swap(v[i-1], v[rand() % i]);
    }
}*/
//The tenplate class must be defined 100% within the head file,
//but we can just include the implemente in the head file to aviod that.
//usually this is a bad idea, but is convenice
//or you can change the implementation file name with .impl
#include "LVector.cpp"
#endif
#include "LVector.hpp"

template <typename T>
void LVector<T>::copyFrom(T const* A, Rank low, Rank high){
    _elem = new T[_capacity = (high - low)*2];
    _size = 0;
    //time complexity => O(high - low) => O(_size)
    while(low < high){
        _elem[_size++] = A[low++];
    }
}

template <typename T> LVector<T> & LVector<T>::operator= (LVector<T> const& v){
    if(_elem)
        delete [] _elem;
    copyFrom(v, 0, v._size);
    return *this;
}

template <typename T> void LVector<T>::expand(){
    if(_size < _capacity)
        return;
    if(_capacity < DEFAULT_CAPACITY) 
        _capacity = DEFAULT_CAPACITY;
    T *oldElem = _elem;
    _elem = new T[_capacity <<= 1];
    copyFrom(oldElem, 0, _size);
    delete oldElem;
}
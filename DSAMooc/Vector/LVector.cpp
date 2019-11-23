#ifndef LVECTOR_CPP
#define LVECTOR_CPP
#include "LVector.hpp"

template <typename T> void LVector<T>::print(){
    for(Rank i = 0; i < _size; ++i){
        if(i % 10 == 0) std::cout << std::endl;
        std::cout << _elem[i] << "  ";
    }
    std::cout << std::endl << "The size of this vector: " << _size << std::endl;
    std::cout << "The capacity of this vector: " << _capacity << std::endl;
}

template <typename T>
void LVector<T>::copyFrom(T const* A, Rank low, Rank high){
    _elem = new T[_capacity = (high - low)*2];
    _size = 0;
    //time complexity => O(high - low) => O(_size)
    while(low < high){
        _elem[_size++] = A[low++];
    }
}

template <typename T> 
LVector<T> & LVector<T>::operator= (LVector<T> const& v){
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
    delete [] oldElem;
}

template <typename T>
void LVector<T>::shrink(){
    if(_capacity < DEFAULT_CAPACITY<<1) return;
    if(_capacity < _size << 2) return;
    T* oldElem = _elem;
    _elem = new T[_capacity >>= 1];
    for(int i = 0; i < _size; ++i) _elem[i] = oldElem[i];
    delete [] oldElem;
}

template <typename T> 
T& LVector<T>::operator[](Rank i) {
    return _elem[i];
}

template <typename T>
void LVector<T>::unsort(Rank low, Rank high){
    T* v = _elem + low;
    for(Rank i = high - low; i > 0; --i){
        std::swap(v[i - 1], v[rand() % i]);
    }
}

//squential serch, input sensitive
template <typename T> 
Rank LVector<T>::find(T const& e, Rank low, Rank high){
    //assert(0 <= low <= high <= _size)
    //if can not find e, return low - 1
    while((low < high--) && (_elem[high] != e));
    return high;
}

template <typename T>
Rank LVector<T>::insert(T const& e, Rank r){
    //Because c++ do not check array range,so ,if r == 1000 or something else,
    //it will see be ok,and you will get something 11 like corrected.
    //assert(0 <= r <= _size)
    assert(0 <= r && r <= _size);
    expand();
    for(Rank i = _size; i > r; --i)
        _elem[i] = _elem[i - 1];
    _elem[r] = e;
    ++_size;
    return r;
}

template <typename T> 
Rank LVector<T>::remove(Rank low, Rank high){
    assert(0 <= low && low < high && high <= _size);
    while(high < _size) _elem[low++] = _elem[high++];
    _size = low;
    shrink();
    return high - low;
}

template <typename T>
Rank LVector<T>::remove(Rank r){
    assert(0<= r && r < _size);
    T e = _elem[r];
    remove(r, r+1);
    return e;
}

template <typename T>
int LVector<T>::deduplicate(){
    int oldSize = _size;
    Rank i = 1;
    while(i < _size)
        (find(_elem[i], 0, i) < 0) ? ++i : remove(i);
    return oldSize - _size; 
}

template <typename T>
void LVector<T>::traverse(void (*visit) (T& )){
    for(int i = 1; i < _size; ++i)
        visit(_elem[i]);
}

template <typename T> template <typename TVS>
void LVector<T>::traverse(TVS& visit){
    for (int i = 0; i < _size; ++i)
        visit(_elem[i]);
}

template <typename T>
int LVector<T>::disordered() const{
    int n = 0;
    for(int i = 1; i < _size; ++i){
        if(_elem[i - 1] > _elem[i]){
            ++n;
        }
    }
    return n;
}

template <typename T> 
int LVector<T>::uniquify() {
    int i = 0;
    int j = 0;
    while(++j < _size){
        if(_elem[i] != _elem[j])
            _elem[++i] = _elem[j];
    }
    shrink();
    _size = ++i;
    return j - i;
}

template <typename T>
int LVector<T>::uniquifySlowly(){
    int i = 1;
    int oldSize = _size;
    while (i < _size){
        _elem[i - 1] == _elem[i] ? remove(i) : ++i;
    }
    return oldSize - _size;
}

template <typename T> 
Rank LVector<T>::search(T const& e, Rank low, Rank high){
    return rand()%2 ? binSearch(_elem, e, low, high) : binSearch_C(_elem, e, low, high);
} 

template <typename T>
Rank LVector<T>::search(T const& e){
    return _size <= 0 ? -1 : search(e, 0, _size);
}

//Complixty = o(1.5 log n)
//耗时主要在元素大小的比较,特别是当元素为复杂对象时.
//Disadvantage：不同情况查找长度不均衡，在每步迭代中为确定左右分支方向 需分别做一到两次元素比较
template <typename T> 
Rank LVector<T>::binSearch(T* A, T const& e, Rank low, Rank high){
    while(low < high){
        Rank mid = (high + low) >> 1;
        if(A[mid] < e){
            low = ++mid;
        }else if(A[mid] > e){
            high = mid;
        }else{
            return mid;
        }
    }
    return -1;
}

template <typename T>
Rank LVector<T>::binSearch_B(T* A, T const& e, Rank low, Rank high){
    while(1 < high - low){
        Rank mid = (high + low) >> 1;
        //only one compare 
        (e < A[mid]) ? high = mid : low = mid;
    }
    return (e == A[low]) ? low : -1;
    //Can not return fail position
    //If hit multiple element,can not return the max rank element
}

template <typename T>
Rank LVector<T>::binSearch_C(T* A, T const& e, Rank low, Rank high){
    while(low < high) {
        Rank mid = (high - low) >> 1;
        (e < A[mid]) ? high = mid : low = mid + 1;
    }
    return --low;
    //return the last element <= e;maybe e or the element < e
}

template <typename T>
void LVector<T>::sort(Rank low, Rank high){
    switch(2/*rand() % 1*/){
        case 1: bubbleSort(low, high); 
                break;
        case 2: mergeSort(low, high);
    }
}

template <typename T>
void LVector<T>::bubbleSort(Rank low, Rank high){
    while(!bubble(low, high--));
}

template <typename T>
bool LVector<T>::bubble(Rank low, Rank high){
    bool sorted = true;
    while(++low < high){
        if(_elem[low - 1] > _elem[low]){
            sorted = false;
            std::swap(_elem[low - 1], _elem[low]);
        }
    }
    return sorted;
}

template <typename T> 
void LVector<T>::mergeSort(Rank low, Rank high){
    if(high - low < 2) return;
    Rank mid = (high + low) /2;
    mergeSort(low, mid);
    mergeSort(mid, high);
    merge(low, mid, high);
}

template <typename T>
void LVector<T>::merge(Rank low, Rank mid, Rank high){
    T* A = _elem + low;
    int lb = mid - low;
    T* B = new T[lb];
    for(Rank i = 0; i < lb; i++) B[i] = A[i];
    int lc = high - mid;
    T* C = _elem + mid;
    for(Rank i = 0, j = 0, k = 0; (j < lb) /*|| (k < lc)*/; ){
        //if((j < lb) && (!(k < lc) || (B[j] <= C[k]))) A[i++] = B[j++];
        //if((k < lc) && (!(j < lb) || (C[k] < B[j] ))) A[i++] = C[k++];
        if((k < lc) && (C[k] < B[j])) A[i++] = C[k++];
        if((lc <= k) || B[j] <= C[k]) A[i++] = B[j++];
    }
    delete [] B;
}
#endif
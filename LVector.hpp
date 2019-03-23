#ifndef LVECTOR_HPP
#define LVECTOR_HPP
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

    void expand();
public:
    LVector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0){
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    
    ~LVector(){
        delete [] _elem;
    }

    LVector<T> & operator= (LVector<T> const&);
};

#endif
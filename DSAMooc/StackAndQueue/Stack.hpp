#include "../Vector/LVector.hpp"

template <typename T>
class LStack : public LVector<T>
{
    public:
    //interface
    void push(T const& e){ this->insert(e,this->_size); }
    T pop() { return this->remove(this->_size-1); }
    T& top() { return (*this)[this->_size-1]; }
    int size() const { return this->_size; }

};
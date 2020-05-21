#include "StrVec.hpp"

using namespace std;

allocator<string> StrVec::alloc;

StrVec::StrVec(const StrVec& s)
{
    // call alloc_n_copy to allocate exactly as many elements as in s
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::~StrVec()
{
    free();
}

StrVec& StrVec::operator=( const StrVec& right)
{
    auto newData = alloc_n_copy(right.begin(), right.end());
    free();
    elements = newData.first;
    first_free = cap = newData.second;
    return *this;
}

void StrVec::reallocate()
{
    // we'll allocate space for teice as many elements as the current size
    size_t newcapacity = size() ? 2*size() : 1;
    // allocate new memory
    string* newData = alloc.allocate(newcapacity);
    // move the data from the old memory to the new
    string* dest = newData;
    string* elem = elements;
    for( size_t i = 0; i != size(); ++i)
    {
        alloc.construct(dest++, std::move(*elem++));
        // We don't know what value the strings in the old StrVec memory have,
        // but we are guaranteed that it is safe to run the string destructor on these objects. 
    }
    free(); // free the old space once we've moved the elements
    elements = newData;
    first_free = dest;
    cap = elements + newcapacity;
}

// uninitialized_copy calls construct on each element in the input sequence
// to “copy” that element into the destination. That algorithm uses the iterator dereference
// operator to fetch elements from the input sequence. Because we passed
// move iterators, the dereference operator yields an rvalue reference, which means
// construct will use the move constructor to construct the elements.
void StrVec::reallocateV2()
{
    size_t newcapacity = size() ? 2*size() : 1;
    string* first = alloc.allocate(newcapacity);

    // move the elements
    string* last = uninitialized_copy(make_move_iterator(begin()), 
                                        make_move_iterator(end()),
                                        first);
    free();
    elements = first;
    first_free = last;
    cap = elements + newcapacity;
}

void StrVec::push_back(const string& s)
{
    chk_n_alloc(); // ensure that there is room for another element
    // constrcut a copy of s in the element to which first_free points.
    alloc.construct(first_free++, s); 
    // to use this raw memory we must call construct
    // first argument must be a pointer to unconstructed space allocated
    // by a call to allocate.
    // remaining arguments determine which cosntructor to use to construct
    // the object
}

void StrVec::push_back(string&& s)
{
    chk_n_alloc();
    alloc.construct(first_free++, std::move(s));
}

pair<string*, string*> StrVec::alloc_n_copy(const string* b, const string* e)
{
    // allocate space to hold as many elements as are in the range
    string* data = alloc.allocate(e - b);
    // initialize and return a pair constructed from data and 
    // the value returned by uninitialized_copy
    return { data, uninitialized_copy(b, e, data) };
}

void StrVec::free()
{
    // may not pass deallocate a 0 pointer; if elements is 0, there's no work to do
    if(elements)
    {
        // destroy the old elements in reverse order
        for ( string* p= first_free; p!= elements; )
        {
            alloc.destroy(--p);
        }
        alloc.deallocate(elements, cap - elements);
    }
}

void StrVec::reserve()
{
    string* left = elements;
    string* right = --first_free;
    while( elements < first_free)
    {
        left->swap(*right);
        ++left;
        --right;
    }
}

void StrVec::chk_n_alloc()
{
    if( first_free == cap)
    {
        reallocate();
    }
}

StrVec& StrVec::operator=(StrVec &&rhs) noexcept
{
    // direct test for self-assignment
    if ( this != &rhs)// rhs may be the result of calling move
    {
        free();
        elements = rhs.elements;
        first_free = rhs.first_free;
        cap = rhs.cap;
        // leave rhs in a destructible state
        rhs.elements = rhs.first_free = rhs.cap = nullptr;
    }
    return *this;
}
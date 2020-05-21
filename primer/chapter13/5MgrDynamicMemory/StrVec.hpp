#include <string>
#include <memory>
#include <iostream>
//contiguous storage. 连续内存
class StrVec // value like behavior
{
public:
    StrVec() : // the allocator member is default initialized
        elements(nullptr), first_free(nullptr), cap(nullptr){}
    StrVec(const StrVec&); // copy constructor
    StrVec(const std::initializer_list<std::string>& in) :
        elements(nullptr), first_free(nullptr), cap(nullptr)
    { 
        for (const auto& item : in )
        {
            push_back(item);
        }    
    }
    // not allocate any resources, will not throw any exceptions.
    // noexcept is a way for us to promise that a function does not throw any exceptions.
    // must specify on both the declaration in the class header and on the definition.
    StrVec(StrVec &&s) noexcept// this tell constructor does not throw any exceptions
        : elements( s.elements), first_free(s.first_free), 
        cap(s.cap)
    {
        // leave s in a state in which it is safe to run the destructor
        s.elements = s.first_free = s.cap = nullptr;
        // if we neglected to chang this , then the moved-from 
        // object would delete the memory we just moved.
    }
    // copy-assignment operator
    // copy constructor and deconstructor
    StrVec & operator=(const StrVec&); // copy assignment

    // move-assignment operator
    // move constructor and deconstructor
    // guard against self-assignment
    StrVec& operator=(StrVec &&rhs) noexcept;

    ~StrVec(); // destructor
    void push_back(const std::string&); // copy the element
    void push_back(std::string&&); // copy the element
    size_t size() const { return first_free - elements; }
    size_t capacity() const { return cap - elements; }
    std::string* begin() const { return elements; }
    std::string* end() const { return first_free; }
    
    void reserve();
private:
    static std::allocator<std::string> alloc; // allocates the elements
    void chk_n_alloc(); // used by functions that add elements to a StrVec
    // utilities used by the copy constructor, assignment operator, and destructor.
    std::pair<std::string*, std::string*> alloc_n_copy(const std::string* , const std::string*);
    void free(); // destroy the elements and free the space.

    // several library calssed define so-called "move constructors".
    // move constructors "moving" resources from the given object to the object being cosntructed.
    // for string, copy the char array pointer.
    void reallocate(); // get more space and copy the existing elements.
    void reallocateV2(); // get more space and copy the existing elements.
                         // using move iterator

    std::string *elements; // pointer to the first element in the array
    std::string *first_free; // pointer to the first free element in the array
    std::string *cap; // pointer to one past the end of the array.
};
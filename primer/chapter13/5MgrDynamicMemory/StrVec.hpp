#include <string>
#include <memory>
//contiguous storage. 连续内存
class StrVec // value like behavior
{
public:
    StrVec() : // the allocator member is default initialized
        elements(nullptr), first_free(nullptr), cap(nullptr){}
    StrVec(const StrVec&); // copy constructor
    StrVec & operator=(const StrVec&); // copy assignment
    ~StrVec(); // destructor
    void push_back(const std::string&); // copy the element
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

    std::string *elements; // pointer to the first element in the array
    std::string *first_free; // pointer to the first free element in the array
    std::string *cap; // pointer to one past the end of the array.
};
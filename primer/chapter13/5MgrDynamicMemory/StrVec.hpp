#include <string>
#include <memory>
//contiguous storage. 连续内存
class StrVec
{
public:

private:
    static std::allocator<std::string> alloc; // allocates the elements
    void chk_n_alloc(); // used by functions that add elements to a StrVec
    // utilities used by the copy constructor, assignment operator, and destructor.
    std::pair<std::string*, std::string*> alloc_n_copy(const std::string* , const std::string*);
    void free(); // destroy the elements and free the space.
    void reallocate(); // get more space and copy the existing elements.
    std::string *elements; // pointer to the first element in the array
    std::string *first_free; // pointer to the first free element in the array
    std::string *cap; // pointer to one past the end of the array.
};
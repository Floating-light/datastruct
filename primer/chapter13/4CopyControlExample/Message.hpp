#include <string>
#include <set>

class Folder;
class Message 
{
    friend class Folder;
    friend void swap(Message& l, Message& r);
public:
    // Folders is implicitly initialized to the empty set
    explicit Message(const std::string& str = ""):// because of default argument, it is alse the Message default constructor.p290
        contents(str){}
    // copy control to manage pointers to this message
    Message(const Message&);// copy constructor
    Message(Message&& m);
    Message& operator=(const Message&);// copy assignment
    Message& operator=(Message && rhs);
    ~Message(); // destructor

    // add/remove this Message from the specified Folder's set of messages
    void save(Folder&);
    void remove(Folder&);

    // adding element to a container requires memory to be allocated,
    // which means that a bab_alloc exception might be thrown.
private:
    std::string contents;
    std::set<Folder*> folders;

    void add_to_Folders(const Message&);

    void remove_from_Folders();

    // move the Folder pointers from m to this message
    void move_Folders(Message* m);
};
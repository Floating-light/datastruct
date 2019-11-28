#include "Message.hpp"
#include "Folder.hpp"

void swap(Message& l, Message& r)
{
    using std::swap; // not strictly needed in this case, but good habit
    // remove pointers to each Message from their (original) respective Folders
    for (auto f : l.folders)
        f->remMsg(&l);
    for ( auto f : r.folders)
        f->remMsg(&r);
    // swap the contens and Folder pointer sets
    swap(l.folders, r.folders); // uses swap(set&, set& )
    swap(l.contents, r.contents); // swap(string&, string& )

    // add pointer to each Message to their (new) respective Folders
    for (auto f : l.folders)
        f->addMsg(&l);
    for(auto f : l.folders)
        f->addMsg(&r);
}


Message::Message(const Message& message):
    contents(message.contents), folders(message.folders)
{
    add_to_Folders(message); // add this message to the Folders that point to message
}

Message::~Message()
{
    remove_from_Folders();
}

void Message::save(Folder& f)
{
    folders.insert(&f); // add the given folder to our list of folders
    f.addMsg(this); // add this message to f's set of messages
}

void Message::remove(Folder& f)
{
    folders.erase(&f); // take the given folder out of our list of folders
    f.remMsg(this); // remove this message to f's set of messages
}

//add this message to the folder m in.
void Message::add_to_Folders(const Message& message)
{
    for(auto f : message.folders) // for each Folder that holds message
    {
        f->addMsg(this); // add a pointer to this Message to that Folder.
    }
}
void Message::remove_from_Folders()
{
    for (auto f : folders)
        f->remMsg(this);
    folders.clear();
}

Message& Message::operator=(const Message& m)
{
    remove_from_Folders();
    contents = m.contents;
    folders = m.folders;
    add_to_Folders(m);
    return *this;
}


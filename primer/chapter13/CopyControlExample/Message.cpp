#include "Message.hpp"
#include "Folder.hpp"

Message::Message(const Message& message):
    contents(message.contents), folders(message.folders)
{
    add_to_Folder(message); // add this message to the Folders that point to message
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

void Message::add_to_Folder(const Message& message)
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


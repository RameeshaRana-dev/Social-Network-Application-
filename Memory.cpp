#include"memory.h"
#include<iostream>
using namespace std;
Memory::Memory() : Post() //Default Constructor
{
    Original_Post = nullptr; //Setting default value to null pointer
}
Memory::Memory(string id, string text, string sharedById,
    int day, int month, int year,
    Post* originalPost)
    : Post(id, text, sharedById, day, month, year, 3)
{
    this->Original_Post = originalPost;
}
Post* Memory::getOriginalPost() const
{
    return Original_Post;
}
void Memory::display() const
{   //displaying post with memory 
    cout << "~~~ " << getSharedById()
        << " shared a memory ~~~ "
        << "...(" << getDay() << "/"
        << getMonth() << "/" << getYear() << ")" << endl;
    cout << " \"" << getText() << "\"" << endl;
    //checking the timeline of the original post
    if (Original_Post != nullptr)
    {
        int yearsAgo = getYear() - Original_Post->getYear();
        cout << " (" << yearsAgo << " Years Ago)" << endl;

        // display original post below
        cout << endl;
        Original_Post->display();
    }
    else
    {
        cout << "(Original post not available)" << endl;
    }
    //comments for memory post
    for (int i = 0; i < getCommentCount(); i++)
    {
        getComments()[i].display();
    }
}
Memory::~Memory()
{
    Original_Post = nullptr;
}
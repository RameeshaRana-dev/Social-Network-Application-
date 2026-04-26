#include "User.h"
#include <iostream>
using namespace std;

// Default Constructor
User::User()
{
    id = "";
    name = "";
    friendIDs = nullptr;
    friendCount = 0;
    likedPageIDs = nullptr;
    likedPageCount = 0;
}

// Parameterized Constructor
User::User(string id, string name)
{
    this->id = id;
    this->name = name;
    friendIDs = nullptr;
    friendCount = 0;
    likedPageIDs = nullptr;
    likedPageCount = 0;
}

// Getters
string User::getId() const
{
    return id;
}

string User::getName() const
{
    return name;
}

string* User::getFriendIDs() const
{
    return friendIDs;
}

int User::getFriendCount() const
{
    return friendCount;
}

string* User::getLikedPageIDs() const
{
    return likedPageIDs;
}

int User::getLikedPageCount() const
{
    return likedPageCount;
}
 // adding a friend
void User::addFriend(string friendId)
{
    for (int i = 0; i < friendCount; i++)
    {
        if (friendIDs[i] == friendId) // checks if new id already exists
            return;
    }
    string* temp = new string[friendCount + 1]; // larger array of string type to store friend IDs
    for (int i = 0; i < friendCount; i++)
    {
        temp[i] = friendIDs[i]; // copyingg to tem
    }
    temp[friendCount] = friendId; //adding new id

    delete[] friendIDs;
    friendIDs = temp; // putting everything in friend id array
    friendCount++;
}

//adding a Liked Page
void User::addLikedPage(string pageId)
{
    for (int i = 0; i < likedPageCount; i++)
    {
        if (likedPageIDs[i] == pageId) // checks if new id already exists
            return;
    }
    string* temp = new string[likedPageCount + 1]; // larger array of string type to store likedpage IDs
    for (int i = 0; i < likedPageCount; i++)
    {
        temp[i] = likedPageIDs[i]; // copying to temp
    }
    temp[likedPageCount] = pageId;// adding new id

    delete[] likedPageIDs;
    likedPageIDs = temp; // putting everything in liked page ids aray
    likedPageCount++;
}

// Display (Optional Helper)
void User::display() const
{
    cout << id << " - " << name << endl;
}

// Destructor
User::~User()
{
    delete[] friendIDs;
    friendIDs = nullptr;
    delete[] likedPageIDs;
    likedPageIDs = nullptr;
}

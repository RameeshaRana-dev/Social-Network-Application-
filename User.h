#pragma once
#include <string>
using namespace std;

class User
{
private:
	string id;
	string name;
	string* friendIDs;     // storing IDs 
	int friendCount;
	string* likedPageIDs;  // storing page IDs
	int likedPageCount;

public:
	// Default Constructor
	User();
	// Parametrised Constructor
	User(string id, string name);

	// Getters
	string getId() const;
	string getName() const;
	string* getFriendIDs() const;
	int getFriendCount() const;
	string* getLikedPageIDs() const;
	int getLikedPageCount() const;

	// adding Friend
	void addFriend(string friendId);
	// adding liked Page
	void addLikedPage(string pageId);

	// Display (helper function)
	void display() const;

	// Destructor
	~User();
	

};

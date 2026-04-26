#pragma once
#include <string>

class User
{
private:
	std::string id;
	std::string name;
	std::string* friendIDs;     // storing IDs 
	int friendCount;
	std::string* likedPageIDs;  // storing page IDs
	int likedPageCount;

public:
	// Default Constructor
	User();
	// Parametrised Constructor
	User(std::string id, std::string name);

	// Getters
	std::string getId() const;
	std::string getName() const;
	std::string* getFriendIDs() const;
	int getFriendCount() const;
	std::string* getLikedPageIDs() const;
	int getLikedPageCount() const;

	// adding Friend
	void addFriend(std::string friendId);
	// adding liked Page
	void addLikedPage(std::string pageId);

	// Display (helper function)
	void display() const;

	// Destructor
	~User();
	

};

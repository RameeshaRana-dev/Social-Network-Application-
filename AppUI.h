#pragma once
#include <string>
#include "User.h"
#include "Page.h"

class AppBackend; // forward declaration (Partner B)

class AppUI
{
private:
	User* users;   // dynamic array of users
	int userCount;
	Page* pages;   // dynamic array of pages
	int pageCount;
	User* currentUser;
	int day, month, year;
	AppBackend* backend; // link to backend

public:
	// Constructor 
	AppUI();
	//Destructor
	~AppUI();
	// Linking backend
	void setBackend(AppBackend* b);
	// Setting data (important for integration)
	void setUsers(User* u, int count);
	void setPages(Page* p, int count);
	// Core features
	void setCurrentUser(std::string id);
	void setSystemDate(int d, int m, int y);
	// View functions
	void viewFriendList();
	void viewLikedPages();
	void viewHome();
	void viewProfile();
	//Helper function for viewHome
	bool isLeapYear(int y) const;
	int getLastDay(int m, int y);
	bool isPreviousDay(int pDay, int pMonth, int pYear);
	
};

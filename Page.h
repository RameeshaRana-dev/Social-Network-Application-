#pragma once
#include <string>
using namespace std;

class Page
{
private:
	string id;
	string title;
public:
	//Default Constructor
	Page();
	Page(string id, string title);

	//getters
	string getID() const;
	string getTitle()  const;

	//Display
	void display() const;

	//Destructor
	~Page();
};
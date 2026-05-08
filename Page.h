#pragma once
#include <string>

class Page
{
private:
	std::string id;
	std::string title;
public:
	//Default Constructor
	Page();
	//Parametrised Constructor 
	Page(std::string id, std::string title);

	//getters
	std::string getID() const;
	std::string getTitle()  const;

	//Display
	void display() const;

	//Destructor
	~Page();
};
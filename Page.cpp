#include "Page.h"
#include <iostream>
using namespace std;

//Default Constructor
Page::Page()
{
	id = "";
	title = "";
}

//Parametrised Constructor
Page::Page(string id, string  title)
{
	this->id = id;
	this->title = title;
}

//Getters
string Page::getID() const
{
	return id;
}

string Page::getTitle() const
{
	return title;
}

void Page::display() const
{
	cout << id << " - " << title << endl;
}

Page::~Page()
{

}
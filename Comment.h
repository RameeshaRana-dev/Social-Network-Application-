#pragma once
#include <string>
using namespace std;

class Comment
{
private:
	string AuthorID;
	string text;
public:
	Comment();//Default Constructor
	Comment(string au_id, string t);//Parameterized Constructor
	//Getters
	string getAuthorID()const;
	string getText()const;
	//displaying comment function
	void display()const;
	//destructor
	~Comment();
	
};

	
#include"Comment.h"
#include<iostream>
using namespace std;

Comment::Comment()
{
	AuthorID = "";
	text = "";
}

Comment::Comment(string au_id, string t)
{
	AuthorID = au_id;
	text = t;
}

string Comment::getAuthorID()const //getting writer's id
{
	return AuthorID;
}

string Comment::getText()const  // getting comments
{
	return text;
}

void Comment::display()const
{
	//displaying comments
	cout << " " << AuthorID << " wrote: \"" << text << "\"" << endl;
}

Comment::~Comment()
{
}

#pragma once
#include"Post.h"
using namespace std;
class Memory : public Post
{
private:
	Post* Original_Post; // pointing to the original post 
public:
	Memory(); //Default Constructor
	Memory(string id, string text, string sharedById, int day, int month, int year, Post* originalPost);//Parameterized Constructor
	Post* getOriginalPost() const; //getter for original post
	void display() const override; //overriding display function;
	~Memory() override; //destructor
};

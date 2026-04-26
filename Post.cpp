#include "Post.h"
#include<iostream>
#include "Comment.h"   
using namespace std;
Post::Post()
{

	text = "";
	ID = "";
	sharedById = "";
	day = 0;
	month = 0;
	year = 0;
	likeCount = 0;
	commentCount = 0;
	postType = 0;
	likedBy = new string[10];
	comments = new Comment[10];

}
Post::Post(string id, string text, string sharedById, int day, int month, int year, int postType)

{
	this->ID = id;
	this->text = text;
	this->sharedById = sharedById;
	this->day = day;
	this->month = month;
	this->year = year;
	this->postType = postType;
	this->likeCount = 0;
	this->commentCount = 0;
	this->likedBy = new string[10];
	this->comments = new Comment[10];
}
string Post::getID() const
{

	return ID;
}
//-- Getters--
int Post::getDay() const
{
	return day;
}
int Post::getMonth() const
{
	return month;
}
int Post::getYear() const
{
	return year;
}
int Post::getLikeCount() const
{
	return likeCount;
}
int Post::getCommentCount() const
{
	return commentCount;
}
string Post::getText() const
{
	return text;
}
string Post::getSharedById() const
{
	return sharedById;
}
int Post::getPostType() const
{
	return postType;
}
string* Post::getLikedBy() const
{

	return likedBy;
}
Comment* Post::getComments() const
{
	return comments;
}
void Post::addLike(string id)
{   // Checking the limits of likes
	if (likeCount >= 10)
	{
		cout << "Opps!!Likes limit reached!!!" << endl;
		return;
	}
	//Checking for duplicates
	for (int i = 0; i < likeCount; i++)
	{
		if (likedBy[i] == id)
		{
			cout << "This post is already liked by you. Duplicate likes are not allowed." << endl;
			return;
		}

	}
	likedBy[likeCount] = id;//storing the ids in array
	likeCount++;//moving forward for empty slots (incrementing like counts)
}
void Post::addComment(Comment* c)
{
	//Check for comments limits
	if (commentCount >= 10)
	{
		cout << "Sorryyy!!Can't add more than 10 comments!!";
		return;

	}

	comments[commentCount] = *c;//storing the comments in the array
	commentCount++;////moving forward for empty slots (incrementing comment counts)

}
void Post::display() const
{
	// displaying  post with date
	cout << "--- " << sharedById << " shared ";
	cout << "\"" << text << "\"";
	cout << " ...(" << day << "/" << month
		<< "/" << year << ")" << endl;

	// displaying  all comments under post 
	for (int i = 0; i < commentCount; i++)
	{
		comments[i].display();
	}
}
Post::~Post()
{
	// freeing all the dynamically allocated arrays
	delete[] likedBy;
	likedBy = nullptr;
	delete[] comments;
	comments = nullptr;
}
#pragma once
#include "Post.h"
#include"memory.h"
#include"Comment.h"
#include "Activity.h"
#include "User.h"
#include "Page.h"
using namespace std;
class AppBackend
{
private:
	Post** posts;
	int postCount;//to keep track of no. of posts
	Comment** comments;
	int commentCount;//to keep track of no. of comments
	User** users;
	int userCount;//to keep track of no. of users
	Page** pages;
	int pageCount;//to keep track of no. of pages
	User* currentUser;//to keep track of the current user logged in(or to select that user logged in would be selected as current user)
	//System Date
	int systemDay;
	int systemMonth;
	int systemYear;
	//Helper functions to find the index of post, user and page in their respective arrays using their IDs
	bool isUserLoggedIn() const;//checking if the user is logged in or not
	Post* getValidPost(string postId) const;
	void trimLeading(string& str);
public:
	AppBackend();
	void setCurrentUser(string Userid);//to set the current user using their ID
	void setSystemDate(int day, int month, int year);
	Post* findPost(string postId) const;
	User* findUser(string userId) const;
	Page* findPage(string pageId) const;
	int getPostCount() const;
	int getUserCount() const;
	int getPageCount() const;
	string getCurrentUserName() const;
		// current user getter
	string getCurrentUserId() const;
	void loadPages();
	void loadUsers();
	void loadPosts();
	void loadComments();
	void loadAllData();
	void likePost(string postID);//to like a post
	void viewLikedList(string postId);
	void addComment(string postid, string text);
	void ViewPost(string postId);
	void ViewPage(string pageId);
	void SeeMemory();
	void shareMemory(string postId, string text);//sharing memories by creating a new post 
	~AppBackend();
};


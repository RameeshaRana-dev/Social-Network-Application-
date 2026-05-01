#include"Appbackend.h"
#include<fstream>
#include<iostream>
using namespace std;
AppBackend::AppBackend()
{
	posts = nullptr;
	postCount = 0;
	comments = nullptr;
	commentCount = 0;
	users = nullptr;
	userCount = 0;
	pages = nullptr;
	pageCount = 0;
	currentUser = nullptr;
	systemDay = 0;
	systemMonth = 0;
	systemYear = 0;
}
Post* AppBackend::findPost(string postId) const
{
	for (int i = 0; i < postCount; i++)
	{
		if (posts[i]->getID() == postId)
			return posts[i];  // return pointer directly
	}
	return nullptr;  // post not found
}
User* AppBackend::findUser(string userId) const
{
	for (int i = 0; i < userCount; i++)
	{
		if (users[i]->getId() == userId)
			return users[i];  // return pointer directly
	}
	return nullptr;  // user not found
}
Page* AppBackend::findPage(string pageId) const
{
	for (int i = 0; i < pageCount; i++)
	{
		if (pages[i]->getID() == pageId)
			return pages[i];  // return pointer directly
	}
	return nullptr;  // page not found
}
int AppBackend:: getPostCount() const 
{ 
	return postCount;
}
int AppBackend::getUserCount() const 
{ 
	return userCount;
}
int AppBackend::getPageCount() const
{ 
	return pageCount;
}

// current user getter
string AppBackend::getCurrentUserName() const
{
	if (currentUser != nullptr)
		return currentUser->getName();
	return "";
}

string AppBackend::getCurrentUserId() const
{
	if (currentUser != nullptr)
		return currentUser->getId();
	return "";
}
void AppBackend::setCurrentUser(string userId)
{
	User* user = findUser(userId);
	if (user != nullptr)
	{
		currentUser = user;
		cout << currentUser->getName()
			<< " successfully set as Current User" << endl;
	}
	else
	{
		cout << "User with ID " << userId
			<< " not found!!!" << endl;
	}
}
void AppBackend::setSystemDate(int day, int month, int year)
{
	systemDay = day;
	systemMonth = month;
	systemYear = year;
	cout << "System Date: " << day << "/"
		<< month << "/" << year << endl;
}
void AppBackend::trimLeading(string& str)
{
	int start = 0;
	while (start < str.length() &&
		(str[start] == ' ' || str[start] == '\t'))
	{
		start++;
	}
	string clean = "";
	for (int i = start; i < str.length(); i++)
	{
		clean += str[i];
	}
	str = clean;
}
void AppBackend::loadPages()
{
	ifstream fin("Pages.txt");
	if (!fin)
	{
		cout << "Error:Pages.txt not found!!" << endl;
		return;
	}
	fin >> pageCount;
	pages = new Page * [pageCount];
	for (int i = 0; i < pageCount; i++)
	{
		string p_id;
		string p_title;
		fin >> p_id;
		fin.ignore();
		getline(fin, p_title);
		trimLeading(p_title);
		pages[i] = new Page(p_id, p_title);
	}
	fin.close();
	cout << "Pages loaded:" << pageCount << endl;
}
void AppBackend::loadUsers()
{
	ifstream fin("Users.txt");
	if (!fin)
	{
		cout << "Error:Users.txt not found!!" << endl;
		return;
	}
	fin >> userCount;
	users = new User * [userCount];
	for (int i = 0; i < userCount; i++)
	{
		string u_id;
		string u_Firstname;
		string u_Lastname;
		fin >> u_id >> u_Firstname >> u_Lastname;
		string u_name = u_Firstname + " " + u_Lastname;
		users[i] = new User(u_id, u_name);
		string FriendsToken;
		while (fin >> FriendsToken && FriendsToken != "-1")
		{
			users[i]->addFriend(FriendsToken);
		}
		//read pages till encounter -1
		while (fin >> FriendsToken && FriendsToken != "-1")
		{
			users[i]->addLikedPage(FriendsToken);
		}
	}
	fin.close();
	cout << "Users loaded: " << userCount << endl;
}
void AppBackend::loadPosts()
{
	ifstream fin("Posts.txt");
	if (!fin)
	{
		cout << "Error:Posts.txt not found!!" << endl;
		return;
	}
	fin >> postCount;
	posts = new Post * [postCount];
	for (int i = 0; i < postCount; i++)
	{
		string post_id;
		int post_type;
		fin >> post_type >> post_id;
		int day;
		int month;
		int year;
		fin >> day >> month >> year;
		string text;
		fin.ignore();
		getline(fin, text);
		int activityType = 0;
		string activityValue = "";
		if (post_type == 2)
		{
			fin >> activityType;
			fin.ignore();
			getline(fin, activityValue);
		}
		string sharedById;
		fin >> sharedById;
		if (post_type == 1)
		{
			posts[i] = new Post(post_id, text, sharedById,
				day, month, year, 1,"");
		}
		else if (post_type == 2)
		{
			posts[i] = new Activity(post_id, text, sharedById,
				day, month, year,
				activityType, activityValue);
		}
		string token;
		while (fin >> token && token != "-1")
		{
			posts[i]->addLike(token);
		}
	}
	fin.close();
	cout << "Posts loaded: " << postCount << endl;
}
void AppBackend::loadComments()
{
	ifstream fin("Comments.txt");
	if (!fin)
	{
		cout << "Error: Comments.txt not found!" << endl;
		return;
	}

	int totalComments;
	fin >> totalComments;
	fin.ignore();
	commentCount = totalComments;

	for (int i = 0; i < totalComments; i++)
	{
		string c_id, post_id, author_id, c_text;
		fin >> c_id >> post_id >> author_id;
		fin.ignore();
		getline(fin, c_text);
		trimLeading(c_text);
		Post* post = findPost(post_id);
		if (post != nullptr)
		{
			Comment* c = new Comment(author_id, c_text);
			post->addComment(c);
			delete c;
		}
	}
	fin.close();
	cout << "Comments loaded: " << totalComments << endl;
}
void AppBackend::loadAllData()
{
	loadPages();
	loadUsers();
	loadPosts();
	loadComments();
	cout << "All data loaded successfullyyy!" << endl;
}
bool AppBackend::isUserLoggedIn() const
{
	if (currentUser == nullptr)
	{
		cout << "No user is currently logged in!!!" << endl;
		return false;
	}
	return true;
}
Post* AppBackend::getValidPost(string postId) const
{
	Post* post = findPost(postId);
	if (post == nullptr)
	{
		cout << "Post not found!" << endl;
		return nullptr;
	}
	return post;
}
void AppBackend::likePost(string postId)
{
	if (!isUserLoggedIn())
	{
		return;
	}
	Post* post = getValidPost(postId);
	if (post == nullptr)
	{
		return;
	}
	post->addLike(currentUser->getId());
	cout << "Post liked by " << currentUser->getName() << endl;

}
void AppBackend::viewLikedList(string postId)
{
	// checking if user is logged in?
	if (!isUserLoggedIn())
	{
		return;
	}
	Post* post = getValidPost(postId);
	if (post == nullptr)
	{
		return;
	}
	// checking if anyone has liked or not 
	if (post->getLikeCount() == 0)
	{
		cout << "No likes yet." << endl;
		return;
	}

	cout << "Post Liked By:" << endl;
	// get liked by array
	string* likedBy = post->getLikedBy();

	for (int i = 0; i < post->getLikeCount(); i++)
	{
		string liked_id = likedBy[i];

		// checking if likes are from  user or any page
		if (liked_id[0] == 'u')
		{
			User* u = findUser(liked_id);
			if (u != nullptr)
				cout << liked_id << " - " << u->getName() << endl;
		}
		else if (liked_id[0] == 'p')
		{
			Page* p = findPage(liked_id);
			if (p != nullptr)
				cout << liked_id << " - " << p->getTitle() << endl;
		}
	}
}
void AppBackend::addComment(string postid, string text)
{
	if (!isUserLoggedIn())
	{
		return;
	}
	Post* post = getValidPost(postid);
	if (post == nullptr)
	{
		return;
	}
	Comment* c = new Comment(currentUser->getId(), text);
	post->addComment(c);
	delete c;
	cout << "Comment added successfully!" << endl;
}
void AppBackend::ViewPost(string postId)
{
	if (!isUserLoggedIn())
	{
		return;
	}
	// checking valid post
	Post* post = getValidPost(postId);
	if (post == nullptr)
	{
		return;
	}
	post->display();
}
void AppBackend::ViewPage(string pageId)
{
	// finding the page
	Page* page = findPage(pageId);
	if (page == nullptr)
	{
		cout << "Page not found!" << endl;
		return;
	}
	// print page title
	cout << page->getTitle() << endl;
	cout << "-------------------------------------------" << endl;
	// find all posts shared by this page
	bool foundAny = false;
	for (int i = 0; i < postCount; i++)
	{
		if (posts[i]->getSharedById() == pageId)
		{
			posts[i]->display();
			foundAny = true;
		}
	}
	if (!foundAny)
	{
		cout << "No posts on this page." << endl;
	}
}
void AppBackend::SeeMemory()
{
	if (!isUserLoggedIn())
	{
		return;
	}
	cout << "We hope you enjoy looking back and sharing";
	cout << " your memories on Facebook," << endl;
	cout << "On this Day" << endl;
	Post** memoryPosts = new Post * [postCount];
	int memoryCount = 0;
	//collecting memories shared by current user on this day in past years
	for (int i = 0; i < postCount; i++)
	{
		if (posts[i]->getSharedById() == currentUser->getId())
		{
			if (posts[i]->getDay() == systemDay &&
				posts[i]->getMonth() == systemMonth &&
				posts[i]->getYear() < systemYear)
			{
				memoryPosts[memoryCount] = posts[i];
				memoryCount++;
			}
		}
	}
	// no memories found
	if (memoryCount == 0)
	{
		cout << "No memories on this day." << endl;
		delete[] memoryPosts;
		return;
	}
	// sorting(most recent memory first
	// bubble sort
	for (int i = 0; i < memoryCount - 1; i++)
	{
		for (int j = 0; j < memoryCount - i - 1; j++)
		{
			// swap if older year comes before newer year
			if (memoryPosts[j]->getYear() < memoryPosts[j + 1]->getYear())
			{
				Post* temp = memoryPosts[j];
				memoryPosts[j] = memoryPosts[j + 1];
				memoryPosts[j + 1] = temp;
			}
		}
	}
	// display sorted memories
	for (int i = 0; i < memoryCount; i++)
	{
		int yearsAgo = systemYear - memoryPosts[i]->getYear();

		if (yearsAgo == 1)
		{
			cout << "1 Year Ago" << endl;
		}
		else
		{
			cout << yearsAgo << " Years Ago" << endl;
		}

		memoryPosts[i]->display();
		cout << endl;
	}
	delete[] memoryPosts;
	memoryPosts = nullptr;
}
void AppBackend::shareMemory(string postId, string text)
{
	// check user logged in
	if (!isUserLoggedIn())
	{
		return;
	}

	// find original post
	Post* original = getValidPost(postId);
	if (original == nullptr)
	{
		return;
	}
	string memId = "mem";
	int num = postCount;
	if (num >= 10)
	{
		memId += (char)('0' + num / 10);
	}
	memId += (char)('0' + num % 10);
	///Dynamically creating  a Memory object
	// 'new' allocates memory on heap and returns pointer
	// Memory inherits from Post, so it can be stored in Post* array
	Memory* m = new Memory
	(
		memId,
		text,
		currentUser->getId(),
		systemDay,
		systemMonth,
		systemYear,
		original
	);
	Post** newPosts = new Post * [postCount + 1];
	// Copy existing posts into new array
	for (int i = 0; i < postCount; i++)
	{
		newPosts[i] = posts[i];
	}
	newPosts[postCount] = m;
	delete[] posts;
	posts = newPosts;
	postCount++;

	cout << "Memory shared successfully!" << endl;
}
AppBackend::~AppBackend()
{
	// delete posts
	for (int i = 0; i < postCount; i++)
	{
		delete posts[i];
		posts[i] = nullptr;
	}
	delete[] posts;
	posts = nullptr;

	// delete users
	for (int i = 0; i < userCount; i++)
	{
		delete users[i];
		users[i] = nullptr;
	}
	delete[] users;
	users = nullptr;

	// delete pages
	for (int i = 0; i < pageCount; i++)
	{
		delete pages[i];
		pages[i] = nullptr;
	}
	delete[] pages;
	pages = nullptr;

	// comments are not being deleted here because they are owned by posts and will be deleted in Post destructor
	comments = nullptr;

	currentUser = nullptr;
	postCount = 0;
	userCount = 0;
	pageCount = 0;
	commentCount = 0;
}
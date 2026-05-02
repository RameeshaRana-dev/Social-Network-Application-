#include "AppUI.h"
#include "AppBackend.h"
#include <iostream>
using namespace std;

AppUI::AppUI()
{
	users = nullptr;
	userCount = 0;
	pages = nullptr;
	pageCount = 0;
	currentUser = nullptr;
	day = 0;
	month = 0;
	year = 0;
	backend = nullptr;
}

// Destructor
AppUI::~AppUI()
{
	users = nullptr; // no deletio as ido  not create these
	pages = nullptr;
	currentUser = nullptr;
	backend = nullptr;
}
//linking backend
void AppUI::setBackend(AppBackend* b)
{
	backend = b;
}
// setting users and their count
void AppUI::setUsers(User* u, int count)
{
	users = u;
	userCount = count;
}
// setting pages and their count 
void AppUI::setPages(Page* p, int count)
{
	pages = p;
	pageCount = count;
}
//setting up current user to be same as backend
void AppUI::setCurrentUser(string id)
{
	if (backend == nullptr)
	{
		cout << "backend not connected yet.\n";
		return;
	}
	backend->setCurrentUser(id);
	currentUser = backend->getCurrentUser(); // for consistency

	if (currentUser == nullptr)
	{
		cout << "failed to set current user.\n";
	}
}
//setting up date to be same as backend
void AppUI::setSystemDate(int d, int m, int y)
{
	if (backend == nullptr)
	{
		cout << "backend not connected yet.\n";
		return;
	}
	day = d;
	month = m;
	year = y;

	backend->setSystemDate(d, m, y);
}
// printing current users friend list
void AppUI::viewFriendList()
{
	if (backend == nullptr)
	{
		cout << "Backend not connected!" << endl;
		return;
	}
	if (currentUser == nullptr)
	{
		cout << "No user is currently set!" << endl;
		return;
	}

	cout << "Friend List of " << currentUser->getName() << ":" << endl;

	string* friends = currentUser->getFriendIDs(); //storing the friend ids
	int fCount = currentUser->getFriendCount(); //storing friend cout 

	if (fCount == 0) //checking if pointer is empty/null
	{
		cout << "OOPS! No friend found! \n";
		return;
	}
	for (int i = 0; i < fCount; i++)
	{
		User* u = backend->findUser(friends[i]);
		if (u != nullptr)
		{
			cout << friends[i] << " - " << u->getName() << endl;
		}

	}
}
// pri nting liked pagge of th user
void AppUI::viewLikedPages()
{
	if (backend == nullptr)
	{
		cout << "Backend not connected!" << endl;
		return;
	}
	if (currentUser == nullptr)
	{
		cout << "No user is currently set!" << endl;
		return;
	}
	string* likedPages = currentUser->getLikedPageIDs(); // getting likedpage ids
	int lCount = currentUser->getLikedPageCount(); // getting liked page count

	cout << "Liked Pages of " << currentUser->getName() << ":" << endl;
	if (lCount == 0) // seeing if there are no liked pages
	{
		cout << "No Pages liked yet: \n";
		return;
	}

	for (int i = 0; i < lCount; i++) //printingg liked page ids and names
	{
		Page* p = backend->findPage(likedPages[i]); // obtaining name against ids
		if (p != nullptr)
		{
			cout << likedPages[i] << " - " << p->getTitle() << endl;
		}
	}
}
// Helpers for ensuring accurate date check in view home
bool AppUI::isLeapYear(int y)  //check if the year is leap 
{
	return ((y % 4) == 0 && (y % 100) != 0) || (y % 400 == 0);
}

int AppUI::getLastDay(int m, int y) //handles leap year ad returns month accordingly
{
	if (m == 2)
	{
		if (isLeapYear(y))
		{
			return 29;
		}
		else
		{
			return 28;
		}
	}
	if (m == 4 || m == 6 || m == 9 || m == 11)
	{
		return 30;
	}
	return 31;
}

bool AppUI::isPreviousDay(int pDay, int pMonth, int pYear) // handles date checks and returns true if post is of the same day or previous day
{
	// when day = 1 so we need previous month
	if (day == 1)
	{
		int prevMonth = month - 1;
		int prevYear = year;

		// If month is January we need to decremnet year
		if (prevMonth == 0)
		{
			prevMonth = 12;
			prevYear--;
		}

		int lastDay = getLastDay(prevMonth, prevYear);

		if (pYear == prevYear && pMonth == prevMonth &&pDay == lastDay)
		{
			return true;
		}
	}
	// when month remains same
	if (pYear == year && pMonth == month && pDay == day - 1)
	{
		return true;
	}
	return false;
}
//filters through posts and finding apost shared by us or our friend in tthe past 24hours
void AppUI::viewHome()
{
	if (backend == nullptr)
	{
		cout << "Backend not connected!" << endl;
		return;
	}
	if (currentUser == nullptr)
	{
		cout << "No user is currently set!" << endl;
		return;
	}

	cout << "Home Feed for " << currentUser->getName() << ":" << endl;

	Post** post2d = backend->getPosts();
	int postCount = backend->getPostCount();

	string* likedPages = currentUser->getLikedPageIDs(); // getting likedpage ids
	int lCount = currentUser->getLikedPageCount();

	string* friends = currentUser->getFriendIDs(); //getting the friend ids
	int fCount = currentUser->getFriendCount();

	bool foundAny = false;// checks if any recent posts found
	for (int i = 0; i < postCount; i++)
	{
		Post* post = post2d[i];
		string sharedBy = post->getSharedById(); //getting shared by id of the 1d array elements one by one

		bool isRelevant = false;
		for (int j = 0; j < fCount; j++)
		{
			if (sharedBy == friends[j]) //comparng w friends id to seeif friendposted this
			{
				isRelevant = true;
				break;
			}
		}

		if (!isRelevant) // if its not our friends post we check if its our post
		{
			for (int j = 0; j< lCount; j++)
			{
				if (sharedBy == likedPages[j])
				{
					isRelevant = true;
					break;
				}
			}
		}

		if (!isRelevant) //if its neither our nor our friendspost we check the next post and dont perform date checks
		{
			continue;
		}

		//if post is relevant now seeing if it was posted within the last 24 hours

		int postDay = post->getDay();
		int postMonth = post->getMonth();
		int postYear = post->getYear();

		bool isValid = false;
		if (postDay == day && postMonth == month && postYear == year) //checks if post is of the same day
		{ 
			isValid = true;
		}
		else if (isPreviousDay(postDay, postMonth, postYear)) // checks if the post is of previous day
		{
			isValid = true;
		}

		if (!isValid)// checks the next post as date is older than 24hours
		{
			continue;
		}

		post->display(); // display the post when all checks meet 
		cout << endl;
		foundAny = true; // if any post displayed then  make found any true
	}
	if (!foundAny)
	{
		cout << "No recent posts to show\n";
	}
}
void AppUI::viewProfile()
{
	if (backend == nullptr)
	{
		cout << "Backend not connected!" << endl;
		return;
	}
	if (currentUser == nullptr)
	{
		cout << "No user is currently set!" << endl;
		return;
	}
	cout << "Timeline of " << currentUser->getName() << ":\n";
	Post** posts = backend->getPosts();
	int postCount = backend->getPostCount();

	// Temporary array for user's posts
	Post** userPosts = new Post * [postCount];
	int count = 0;

	for (int i = 0; i < postCount; i++)
	{
		if (posts[i]->getSharedById() == currentUser->getId())
		{
			userPosts[count] = posts[i];
			count++;
		}
	}
	if (count == 0)
	{
		cout << "No posts to show." << endl;
		delete[] userPosts;
		return;
	}

	for (int i = 0; i < count - 1; i++)
	{
		for (int j = 0; j < count - i - 1; j++)
		{
			Post* a = userPosts[j];
			Post* b = userPosts[j + 1];

			bool swapNeeded = false;

			// Compare by year
			if (a->getYear() < b->getYear())
				swapNeeded = true;
			else if (a->getYear() == b->getYear())
			{
				// Compare by month
				if (a->getMonth() < b->getMonth())
					swapNeeded = true;
				else if (a->getMonth() == b->getMonth())
				{
					// Compare by day
					if (a->getDay() < b->getDay())
						swapNeeded = true;
				}
			}

			if (swapNeeded)
			{
				Post* temp = userPosts[j];
				userPosts[j] = userPosts[j + 1];
				userPosts[j + 1] = temp;
			}
		}
	}
	for (int i = 0; i < count; i++)
	{
		userPosts[i]->display();
		cout << endl;
	}

	delete[] userPosts;
}


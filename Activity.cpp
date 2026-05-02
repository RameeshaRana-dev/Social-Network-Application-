#include "Activity.h"
#include <iostream>
using namespace std;
Activity::Activity() :Post()//Setting default values
{
	activityType = 0;
	activityValue = "";

}
//Parameterized constructor
Activity::Activity(string id, string text, string sharedById,
	int day, int month, int year,
	int activityType, string activityValue)
	: Post(id, text, sharedById, day, month, year, 2,"")
	// postType = 2 always — Activity post never changes this
{
	// validate activity type strictly 1 to 4
	if (activityType >= 1 && activityType <= 4)
	{
		this->activityType = activityType;
	}
	else
	{
		// invalid type — setting to default value 
		this->activityType = 0;
		cout << "WARNING!!! Invalid activity type passed!" << endl;
	}
	this->activityValue = activityValue;
}
int Activity::getActivityType() const
{
	return activityType;
}
string Activity::getActivityValue() const
{
	return activityValue;          //returns the activity value
	//e.g "excited", "Kashmiri Chai", "a birthday"
}
string Activity::getActivityLabel() const
{
	if (activityType == 1)
	{
		return "feeling";
	}
	if (activityType == 2)
	{
		return "thinking about";
	}
	if (activityType == 3)
	{
		return "making";
	}
	if (activityType == 4)
	{
		return "celebrating";
	}
	return "invalid activity"; // for invalid type
}
void Activity::display() const
{
	//Printing the activity post header with activity label and value
	cout << "--- " << getSharedById() << " is ";

	if (activityType == 0)
	{
		cout << "UNKNOWN ACTIVITY";
	}
	else
	{
		cout << getActivityLabel() << " " << activityValue;
	}

	cout << endl;
	// printing the post text with date
	cout << " \"" << getText() << "\"";
	cout << " ...(" << getDay() << "/"
		<< getMonth() << "/"
		<< getYear() << ")" << endl;

	// printing all comments under this post
	for (int i = 0; i < getCommentCount(); i++)
	{
		getComments()[i].display();
	}

}
Activity::~Activity()//destructor 
{
}
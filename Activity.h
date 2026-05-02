#pragma once
#pragma once
#include "Post.h"
#include<string>
using namespace std;

class Activity : public Post
{
private:
    int activityType;
    // 1 = feeling
    // 2 = thinking about
    // 3 = making
    // 4 = celebrating
    string activityValue; // excited, life, Kashmiri Chai, a birthday etc

public:
    // default constructor
    Activity();

    // parameterized constructor
    Activity(string id, string text, string sharedById, int day, int month, int year, int activityType, string activityValue);

    // getters
    int getActivityType() const;
    string getActivityValue() const;
    // converting numeric activity type into readable label (example: "is feeling" etc)
    string getActivityLabel() const;
    // overriding display from Post
    void display() const override;
    // destructor
    ~Activity() override;
};

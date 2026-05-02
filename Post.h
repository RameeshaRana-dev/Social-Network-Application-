#pragma once
#include <string>
#include "Comment.h"
using namespace std;

class Post
{
private:
    string ID;
    string text;
    string sharedById;
    int day;
    int month;
    int year;
    string* likedBy;
    int likeCount;
    Comment* comments;
    int commentCount;
    int postType;          // 1=simple , 2=activity
    string imagePath;
public:
    // default constructor
    Post();
    // parameterized constructor
    Post(string id, string text, string sharedById,
        int day, int month, int year, int postType, string imagePath);
    // getters
    string getID() const;
    string getText() const;
    string getSharedById() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getLikeCount() const;
    int getCommentCount() const;
    int getPostType() const;
    string* getLikedBy() const;
    Comment* getComments() const;
    void setImagePath(string path);
    string getImagePath() const;
    // adding likes
    void addLike(string id);
    //adding comments
    void addComment(Comment* c);
    // display
    virtual void display() const;
    // destructor
    virtual ~Post();
};
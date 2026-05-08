#include "Activity.h"
#include "SFMLUI.h"
#include <iostream>
#include <sstream>
using namespace std;

// DATE VALIDATION

bool SFMLUI::isValidDate(int d, int m, int y)
{
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12)   return false;
    int maxDay = 31;
    if (m == 4 || m == 6 || m == 9 || m == 11) maxDay = 30;
    else if (m == 2)
    {
        bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
        maxDay = leap ? 29 : 28;
    }
    return (d >= 1 && d <= maxDay);
}

// CONSTRUCTOR

SFMLUI::SFMLUI()
    : window(VideoMode(1400, 900), "Social Networking App")
{
    backend.loadAllData();
    backend.setCurrentUser("u7");
    backend.setSystemDate(15, 11, 2017);

    ui.setBackend(&backend);

    if (!font.loadFromFile("segoeuib.ttf"))
        cout << "Font not loaded!\n";

    likeTexture.loadFromFile("like.png");
    commentTexture.loadFromFile("comment.png");
    likeIcon.setTexture(likeTexture);
    commentIcon.setTexture(commentTexture);
    likeIcon.setScale(0.05f, 0.05f);
    commentIcon.setScale(0.05f, 0.05f);

    darkMode = false;
    bgColor = Color::White;
    cardColor = Color(240, 240, 240);
    textColor = Color::Black;
    sidebarColor = Color(220, 220, 220);

    currentScreen = HOME;
    selectedPost = 0;
    scrollOffset = 0;
    typingInInput = false;
    statusMessage = "";
    showErrorPopup = false;
    popupMessage = "";

    // share memory state
    shareStep = 0;
    sharePostId = "";

    // view page state
    viewPageStep = 0;
    viewPageId = "";

    setupButton(homeBtn, "Home Feed", 20, 30, Color(100, 149, 237), Color::White);
    setupButton(profileBtn, "Profile", 20, 88, Color(100, 149, 237), Color::White);
    setupButton(timelineBtn, "Timeline", 20, 146, Color(100, 149, 237), Color::White);
    setupButton(memoryBtn, "Memories", 20, 204, Color(100, 149, 237), Color::White);
    setupButton(shareMemoryBtn, "Share Memory", 20, 262, Color(100, 149, 237), Color::White);
    setupButton(friendsBtn, "Friend List", 20, 320, Color(100, 149, 237), Color::White);
    setupButton(pagesBtn, "Liked Pages", 20, 378, Color(100, 149, 237), Color::White);
    setupButton(likedPagesBtn, "View Page", 20, 436, Color(100, 149, 237), Color::White);
    setupButton(viewLikedListBtn, "View Liked List", 20, 494, Color(100, 149, 237), Color::White);
    setupButton(setUserBtn, "Set User", 20, 552, Color(100, 149, 237), Color::White);
    setupButton(setDateBtn, "Set Date", 20, 610, Color(100, 149, 237), Color::White);
    setupButton(viewPostBtn, "View Post", 20, 668, Color(100, 149, 237), Color::White);
    setupButton(exitBtn, "Exit", 20, 726, Color(220, 20, 60), Color::White);
    setupButton(darkBtn, "Switch Theme", 20, 784, Color(255, 140, 0), Color::White);

    // POST ACTION BUTTONS (for POST_VIEW screen)
    setupButton(likeActionBtn, "Like", 320, 650, Color(100, 149, 237), Color::White);
    setupButton(commentActionBtn, "Comment", 480, 650, Color(34, 139, 34), Color::White);
    setupButton(backBtn, "Back to Feed", 640, 650, Color(220, 20, 60), Color::White);

    inputBox.setPosition(310, 840);
    inputBox.setSize(Vector2f(1060, 45));
    inputBox.setFillColor(Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(Color::Black);

    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(Color::Black);
    inputText.setPosition(318, 848);
}

// BUTTON SETUP

void SFMLUI::setupButton(Button& btn, string title,
    float x, float y, Color bg, Color txtColor)
{
    btn.box.setPosition(x, y);
    btn.box.setSize(Vector2f(240, 50));
    btn.box.setFillColor(bg);
    btn.text.setFont(font);
    btn.text.setString(title);
    btn.text.setCharacterSize(18);
    btn.text.setFillColor(txtColor);
    btn.text.setPosition(x + 8, y + 12);
}

// DARK MODE

void SFMLUI::toggleDarkMode()
{
    darkMode = !darkMode;
    if (darkMode)
    {
        bgColor = Color(30, 30, 30); cardColor = Color(50, 50, 50);
        textColor = Color::White;  sidebarColor = Color(40, 40, 40);
    }
    else
    {
        bgColor = Color::White;    cardColor = Color(240, 240, 240);
        textColor = Color::Black;  sidebarColor = Color(220, 220, 220);
    }
}

// EMPTY / WHITESPACE CHECK

bool isBlank(const string& s)
{
    for (char c : s)
        if (c != ' ' && c != '\t')
        {
            return false;
        }
    return true;
}

// ERROR POPUP

void SFMLUI::drawErrorPopup()
{
    if (!showErrorPopup)
    {
        return;
    }
    RectangleShape overlay(Vector2f(1400, 900));
    overlay.setFillColor(Color(0, 0, 0, 160));
    window.draw(overlay);

    RectangleShape popup(Vector2f(700, 200));
    popup.setPosition(350, 350);
    popup.setFillColor(Color(200, 40, 40));
    popup.setOutlineThickness(3);
    popup.setOutlineColor(Color::White);
    window.draw(popup);

    Text icon;
    icon.setFont(font); icon.setString("!");
    icon.setCharacterSize(48); icon.setFillColor(Color::White);
    icon.setStyle(Text::Bold); icon.setPosition(375, 355);
    window.draw(icon);

    Text msg;
    msg.setFont(font); msg.setString(popupMessage);
    msg.setCharacterSize(20); msg.setFillColor(Color::White);
    msg.setStyle(Text::Bold); msg.setPosition(420, 365);
    window.draw(msg);

    Text sub;
    sub.setFont(font);
    sub.setString("Click anywhere or press any key to dismiss");
    sub.setCharacterSize(16); sub.setFillColor(Color(255, 190, 190));
    sub.setPosition(420, 405); window.draw(sub);

    RectangleShape okBox(Vector2f(130, 44));
    okBox.setPosition(665, 468); okBox.setFillColor(Color::White);
    window.draw(okBox);

    Text okTxt;
    okTxt.setFont(font); okTxt.setString("  OK");
    okTxt.setCharacterSize(22); okTxt.setFillColor(Color(200, 40, 40));
    okTxt.setStyle(Text::Bold); okTxt.setPosition(668, 473);
    window.draw(okTxt);
}

// ======================
// LIKE HANDLER
// ======================
void SFMLUI::handleLike()
{
    int count = backend.getPostCount();
    if (count == 0 || selectedPost >= count) return;

    Post* p = backend.getPosts()[selectedPost];
    string uid = backend.getCurrentUser()->getId();

    string* likedBy = p->getLikedBy();
    for (int i = 0; i < p->getLikeCount(); i++)
    {
        if (likedBy[i] == uid)
        {
            popupMessage = "You already liked this post!";
            showErrorPopup = true;
            return;
        }
    }
    backend.likePost(p->getID());
    statusMessage = "Post liked successfully!";
}

// ======================
// COMMENT HANDLER
// ======================
void SFMLUI::handleComment()
{
    int count = backend.getPostCount();
    if (count == 0 || selectedPost >= count) return;

    if (commentInput.empty() || isBlank(commentInput))
    {
        popupMessage = "Comment cannot be empty or just spaces!";
        showErrorPopup = true;
        return;
    }

    backend.addComment(
        backend.getPosts()[selectedPost]->getID(),
        commentInput);
    statusMessage = "Comment added!";
    commentInput.clear();
}

// ======================
// STATUS BAR
// ======================
void SFMLUI::drawStatusMessage()
{
    if (statusMessage.empty()) return;

    RectangleShape bar(Vector2f(1090, 36));
    bar.setPosition(295, 856);
    bar.setFillColor(Color(40, 100, 200, 220));
    window.draw(bar);

    Text msg; msg.setFont(font);
    msg.setString(statusMessage);
    msg.setCharacterSize(17); msg.setFillColor(Color::White);
    msg.setPosition(304, 862); window.draw(msg);
}

// ======================
// SIDEBAR
// ======================
void SFMLUI::drawSidebar()
{
    RectangleShape sidebar(Vector2f(268, 900));
    sidebar.setFillColor(sidebarColor);
    window.draw(sidebar);

    Text title; title.setFont(font);
    title.setString("Social Networking App");
    title.setCharacterSize(18);
    title.setPosition(18, 10);
    title.setFillColor(textColor);
    title.setStyle(Text::Bold);

    window.draw(title);

    window.draw(homeBtn.box);          window.draw(homeBtn.text);
    window.draw(profileBtn.box);       window.draw(profileBtn.text);
    window.draw(timelineBtn.box);      window.draw(timelineBtn.text);
    window.draw(memoryBtn.box);        window.draw(memoryBtn.text);
    window.draw(shareMemoryBtn.box);   window.draw(shareMemoryBtn.text);
    window.draw(friendsBtn.box);       window.draw(friendsBtn.text);
    window.draw(pagesBtn.box);         window.draw(pagesBtn.text);
    window.draw(likedPagesBtn.box);    window.draw(likedPagesBtn.text);
    window.draw(viewLikedListBtn.box); window.draw(viewLikedListBtn.text);
    window.draw(setUserBtn.box);       window.draw(setUserBtn.text);
    window.draw(setDateBtn.box);       window.draw(setDateBtn.text);
    window.draw(viewPostBtn.box);      window.draw(viewPostBtn.text);
    window.draw(exitBtn.box);          window.draw(exitBtn.text);
    window.draw(darkBtn.box);          window.draw(darkBtn.text);
}

// ======================
// INPUT BOX
// ======================
void SFMLUI::drawInputBox()
{
    inputBox.setFillColor(darkMode ? Color(60, 60, 60) : Color::White);
    inputBox.setOutlineColor(typingInInput
        ? Color(100, 149, 237) : Color(180, 180, 180));

    string display = commentInput.empty() ? "Click here and type..." : commentInput;
    inputText.setFillColor(commentInput.empty() ? Color(160, 160, 160)
        : (darkMode ? Color::White : Color::Black));
    inputText.setString(display);

    window.draw(inputBox);
    window.draw(inputText);
}

// ======================
// HOME FEED
// ======================
void SFMLUI::drawHomeFeed()
{
    Post** posts = backend.getPosts();
    int count = backend.getPostCount();
    User* cu = backend.getCurrentUser();

    if (!cu || count == 0)
    {
        Text empty;
        empty.setFont(font);
        empty.setString("No posts to display.");
        empty.setCharacterSize(24);
        empty.setFillColor(textColor);
        empty.setPosition(600, 400);
        window.draw(empty);

        drawInputBox();
        return;
    }

    string* friendIDs = cu->getFriendIDs();
    int friendCount = cu->getFriendCount();

    string* pageIDs = cu->getLikedPageIDs();
    int pageCount = cu->getLikedPageCount();

    const float cardX = 285.f;
    const float cardWidth = 1080.f;

    float currentY = 20.f - scrollOffset;

    for (int i = 0; i < count; i++)
    {
        Post* post = posts[i];
        string sharedBy = post->getSharedById();

        if (post->getPostType() == 3)
            continue;

        if (post->getLikeCount() == 0)
            continue;

        bool relevant = false;

        for (int j = 0; j < friendCount; j++)
        {
            if (sharedBy == friendIDs[j])
            {
                relevant = true;
                break;
            }
        }

        for (int j = 0; j < pageCount; j++)
        {
            if (sharedBy == pageIDs[j])
            {
                relevant = true;
                break;
            }
        }

        if (sharedBy == cu->getId())
            relevant = true;

        if (!relevant)
            continue;

        string posterName = sharedBy;

        if (sharedBy[0] == 'u')
        {
            User* u = backend.findUser(sharedBy);
            if (u) posterName = u->getName();
        }
        else
        {
            Page* p = backend.findPage(sharedBy);
            if (p) posterName = p->getTitle();
        }

        string headerLine;

        Activity* act = dynamic_cast<Activity*>(post);

        if (act)
        {
            headerLine =
                "--- " + posterName +
                " is " +
                act->getActivityLabel() +
                " " +
                act->getActivityValue();
        }
        else
        {
            headerLine =
                "--- " + posterName +
                " shared";
        }

        int visibleComments = min(post->getCommentCount(), 3);
        float cardHeight = 170.f + (visibleComments * 38.f);

        RectangleShape card(Vector2f(cardWidth, cardHeight));
        card.setPosition(cardX, currentY);
        card.setFillColor(cardColor);

        card.setOutlineThickness(i == selectedPost ? 3 : 1);
        card.setOutlineColor(
            i == selectedPost
            ? Color(100, 149, 237)
            : Color(180, 180, 180)
        );

        window.draw(card);

        Text headerTxt;
        headerTxt.setFont(font);
        headerTxt.setCharacterSize(21);
        headerTxt.setStyle(Text::Bold);
        headerTxt.setFillColor(textColor);
        headerTxt.setString(headerLine);
        headerTxt.setPosition(cardX + 15, currentY + 15);

        window.draw(headerTxt);

        string postText = "\"" + post->getText() + "\"";

        if (postText.length() > 130)
            postText = postText.substr(0, 130) + "...";

        Text postTxt;
        postTxt.setFont(font);
        postTxt.setCharacterSize(20);
        postTxt.setFillColor(textColor);
        postTxt.setString(postText);
        postTxt.setPosition(cardX + 20, currentY + 55);

        window.draw(postTxt);

        Text dateTxt;
        dateTxt.setFont(font);
        dateTxt.setCharacterSize(14);
        dateTxt.setFillColor(Color(140, 140, 140));
        dateTxt.setString(
            to_string(post->getDay()) + "/" +
            to_string(post->getMonth()) + "/" +
            to_string(post->getYear())
        );
        dateTxt.setPosition(cardX + 20, currentY + 95);

        window.draw(dateTxt);

        Text likeTxt;
        likeTxt.setFont(font);
        likeTxt.setCharacterSize(16);
        likeTxt.setFillColor(Color(100, 149, 237));
        likeTxt.setString("Likes: " + to_string(post->getLikeCount()));
        likeTxt.setPosition(cardX + 20, currentY + 120);

        window.draw(likeTxt);

        float commentY = currentY + 150;
        Comment* cmts = post->getComments();

        for (int c = 0; c < post->getCommentCount() && c < 3; c++)
        {
            string authorID = cmts[c].getAuthorID();
            string authorName = authorID;

            User* au = backend.findUser(authorID);
            if (au) authorName = au->getName();

            string cmt = authorName + " wrote: \"" + cmts[c].getText() + "\"";

            Text cmtTxt;
            cmtTxt.setFont(font);
            cmtTxt.setCharacterSize(16);
            cmtTxt.setFillColor(Color(80, 80, 80));
            cmtTxt.setString(cmt);
            cmtTxt.setPosition(cardX + 25, commentY);

            window.draw(cmtTxt);
            commentY += 34.f;
        }

        currentY += cardHeight + 25.f;
    }
    drawStatusMessage();
}

// ======================
// GENERIC SCREEN
// ======================
void SFMLUI::drawGenericScreen(const string& title, const string& content)
{
    RectangleShape card(Vector2f(1110, 870));
    card.setPosition(278, 10); card.setFillColor(cardColor);
    card.setOutlineThickness(1); card.setOutlineColor(Color(180, 180, 180));
    window.draw(card);

    Text heading; heading.setFont(font); heading.setCharacterSize(26);
    heading.setFillColor(Color(100, 149, 237)); heading.setStyle(Text::Bold);
    heading.setString(title); heading.setPosition(290, 18);
    window.draw(heading);

    Text body; body.setFont(font); body.setCharacterSize(18);
    body.setFillColor(textColor);
    body.setString(content); body.setPosition(290, 58);
    window.draw(body);
}

// ======================
// BUILD: PROFILE
// ======================
string buildProfileContent(AppBackend& b)
{
    User* u = b.getCurrentUser();
    if (!u) return "No user set.";
    Post** posts = b.getPosts(); int count = b.getPostCount();
    ostringstream o;
    o << "User: " << u->getName() << "  (ID: " << u->getId() << ")\n\n";
    Post** mine = new Post * [count]; int mc = 0;
    for (int i = 0; i < count; i++)
        if (posts[i]->getSharedById() == u->getId()) mine[mc++] = posts[i];
    if (mc == 0) { o << "No posts yet."; delete[]mine; return o.str(); }
    for (int i = 0; i < mc - 1; i++) for (int j = 0; j < mc - i - 1; j++)
    {
        Post* a = mine[j], * bx = mine[j + 1]; bool sw = false;
        if (a->getYear() < bx->getYear()) sw = true;
        else if (a->getYear() == bx->getYear()) {
            if (a->getMonth() < bx->getMonth()) sw = true;
            else if (a->getMonth() == bx->getMonth() && a->getDay() < bx->getDay()) sw = true;
        }
        if (sw) { Post* tmp = mine[j]; mine[j] = mine[j + 1]; mine[j + 1] = tmp; }
    }
    for (int i = 0; i < mc; i++) {
        string txt = mine[i]->getText();
        if (txt.length() > 80) txt = txt.substr(0, 80) + "...";
        o << "[" << mine[i]->getDay() << "/" << mine[i]->getMonth() << "/"
            << mine[i]->getYear() << "]  " << txt
            << "\n  Likes: " << mine[i]->getLikeCount()
            << "   Comments: " << mine[i]->getCommentCount() << "\n\n";
    }
    delete[]mine; return o.str();
}

// ======================
// BUILD: TIMELINE - FIXED
// ======================
string buildTimelineContent(AppBackend& b)
{
    User* u = b.getCurrentUser();
    if (!u) return "No user set.";

    Post** posts = b.getPosts();
    int count = b.getPostCount();

    ostringstream o;
    o << u->getName() << " - Timeline\n\n";

    for (int i = 0; i < count; i++)
    {
        Post* p = posts[i];

        if (!p) continue;

        bool isMemory = (p->getPostType() == 3);

        if (isMemory)
        {
            o << "~~~ " << u->getName()
                << " shared a memory ~~~ ("
                << p->getDay() << "/"
                << p->getMonth() << "/"
                << p->getYear() << ")\n";
        }

        Activity* act = dynamic_cast<Activity*>(p);

        if (act)
        {
            User* poster = b.findUser(p->getSharedById());
            string name = poster ? poster->getName() : p->getSharedById();

            o << "--- " << name
                << " is " << act->getActivityLabel()
                << " " << act->getActivityValue() << "\n";
        }
        else
        {
            o << "--- " << p->getSharedById() << " shared\n";
        }

        o << "\"" << p->getText() << "\"\n";

        if (p->getLikeCount() > 0)
            o << "(" << p->getLikeCount() << " Likes)\n";

        o << "\n";
    }

    return o.str();
}

// ======================
// BUILD: FRIENDS
// ======================
string buildFriendContent(AppBackend& b)
{
    User* u = b.getCurrentUser();
    if (!u) return "No user set.";
    string* ids = u->getFriendIDs(); int fc = u->getFriendCount();
    ostringstream o;
    o << "Friends of " << u->getName() << ":\n\n";
    if (fc == 0) { o << "No friends found."; return o.str(); }
    for (int i = 0; i < fc; i++) {
        User* f = b.findUser(ids[i]);
        o << ids[i] << "  -  " << (f ? f->getName() : "(unknown)") << "\n";
    }
    return o.str();
}

// ======================
// BUILD: LIKED PAGES LIST
// ======================
string buildLikedPagesContent(AppBackend& b)
{
    User* u = b.getCurrentUser();
    if (!u) return "No user set.";
    string* ids = u->getLikedPageIDs(); int lc = u->getLikedPageCount();
    ostringstream o;
    o << "Pages liked by " << u->getName() << ":\n\n";
    if (lc == 0) { o << "No liked pages."; return o.str(); }
    for (int i = 0; i < lc; i++) {
        Page* p = b.findPage(ids[i]);
        o << (p ? p->getID() : ids[i]) << "  -  " << (p ? p->getTitle() : "(unknown)") << "\n";
    }
    return o.str();
}

// ======================
// BUILD: VIEW ONE PAGE by ID
// ======================
string buildOnePageContent(AppBackend& b, const string& pageId)
{
    Page* page = b.findPage(pageId);
    if (!page) return "Page not found";

    Post** posts = b.getPosts();
    int count = b.getPostCount();

    ostringstream o;

    o << page->getTitle() << "\n\n";

    for (int i = 0; i < count; i++)
    {
        Post* p = posts[i];

        if (p->getSharedById() != page->getID())
            continue;

        Activity* act = dynamic_cast<Activity*>(p);

        if (act)
        {
            o << "--- " << page->getTitle()
                << " is " << act->getActivityLabel()
                << " " << act->getActivityValue() << ".\n";
        }
        else
        {
            o << "--- " << page->getTitle() << " post\n";
        }

        o << "\"" << p->getText() << "\"\n";

        Comment* cm = p->getComments();

        for (int c = 0; c < p->getCommentCount(); c++)
        {
            string name = cm[c].getAuthorID();
            User* u = b.findUser(name);
            if (u) name = u->getName();

            o << name << " wrote: \"" << cm[c].getText() << "\"\n";
        }

        o << "\n";
    }

    return o.str();
}

// ======================
// BUILD: MEMORIES
// ======================
string buildMemoriesContent(AppBackend& b)
{
    User* u = b.getCurrentUser();
    if (!u) return "No user set.";
    Post** posts = b.getPosts(); int count = b.getPostCount();
    int sd = b.getSystemDay(), sm = b.getSystemMonth(), sy = b.getSystemYear();

    ostringstream o;
    o << "We hope you enjoy looking back and sharing your\n"
        << "memories on Facebook, from the most recent to\n"
        << "those long ago.\n\n"
        << "On This Day\n\n";

    Post** mem = new Post * [count]; int mc = 0;
    for (int i = 0; i < count; i++) {
        if (posts[i]->getSharedById() != u->getId()) continue;
        if (posts[i]->getDay() == sd && posts[i]->getMonth() == sm
            && posts[i]->getYear() < sy)
            mem[mc++] = posts[i];
    }

    if (mc == 0) {
        o << "No memories for today.\n\n"
            << "Note: Memories show your original posts made\n"
            << "on this day in past years only.";
        delete[]mem; return o.str();
    }

    for (int i = 0; i < mc - 1; i++) for (int j = 0; j < mc - i - 1; j++)
        if (mem[j]->getYear() < mem[j + 1]->getYear())
        {
            Post* tmp = mem[j]; mem[j] = mem[j + 1]; mem[j + 1] = tmp;
        }

    for (int i = 0; i < mc; i++)
    {
        int ago = sy - mem[i]->getYear();
        o << ago << " Year" << (ago > 1 ? "s" : "") << " Ago  ("
            << mem[i]->getDay() << "/" << mem[i]->getMonth() << "/" << mem[i]->getYear() << ")\n";
        o << mem[i]->getText() << "\n\n";
    }

    delete[]mem; return o.str();
}

// ======================
// BUILD: LIKED LIST
// ======================
string buildLikedListContent(AppBackend& b)
{
    User* u = b.getCurrentUser();
    if (!u) return "No user set.";

    Post** posts = b.getPosts();
    int count = b.getPostCount();

    ostringstream o;
    o << "Like counts for posts by " << u->getName() << ":\n\n";

    bool any = false;

    for (int i = 0; i < count; i++)
    {
        if (posts[i]->getSharedById() != u->getId())
            continue;

        if (posts[i]->getLikeCount() == 0)
            continue;

        any = true;

        string txt = posts[i]->getText();
        if (txt.length() > 60)
            txt = txt.substr(0, 60) + "...";

        o << "Post " << posts[i]->getID() << ": \"" << txt << "\"\n"
            << "  Total likes: " << posts[i]->getLikeCount() << "\n";

        string* lb = posts[i]->getLikedBy();

        for (int j = 0; j < posts[i]->getLikeCount(); j++)
        {
            string lid = lb[j];
            string name = lid;

            if (!lid.empty() && lid[0] == 'u')
            {
                User* fu = b.findUser(lid);
                if (fu) name = fu->getName();
            }
            else
            {
                Page* fp = b.findPage(lid);
                if (fp) name = fp->getTitle();
            }

            o << "    * " << lid << "  -  " << name << "\n";
        }

        o << "\n";
    }

    if (!any)
        o << "No posts found for this user.";

    return o.str();
}

// ======================
// SHARE MEMORY SCREEN - FIXED
// ======================
void SFMLUI::drawShareMemoryScreen()
{
    RectangleShape card(Vector2f(1110, 870));
    card.setPosition(278, 10); card.setFillColor(cardColor);
    card.setOutlineThickness(1); card.setOutlineColor(Color(180, 180, 180));
    window.draw(card);

    Text heading; heading.setFont(font); heading.setCharacterSize(26);
    heading.setFillColor(Color(100, 149, 237)); heading.setStyle(Text::Bold);
    heading.setString("Share a Memory"); heading.setPosition(290, 18);
    window.draw(heading);

    if (shareStep == 0)
    {
        Text instr; instr.setFont(font); instr.setCharacterSize(20);
        instr.setFillColor(textColor);
        instr.setString(
            "Step 1 of 2\n\n"
            "Type the Post ID of the memory you want to share,\n"
            "then press Enter.\n\n"
            "Example:  p1\n\n");
        instr.setPosition(290, 60); window.draw(instr);
    }
    else // shareStep==1
    {
        Text instr; instr.setFont(font); instr.setCharacterSize(20);
        instr.setFillColor(textColor);
        instr.setString(
            "Step 2 of 2\n\n"
            "Type a caption for sharing this memory,\n"
            "then press Enter.\n\n"
            "Selected Post ID: " + sharePostId + "\n\n");
        instr.setPosition(290, 60); window.draw(instr);
    }

    drawInputBox();
    drawStatusMessage();
}

// ======================
// VIEW PAGE SCREEN
// ======================
void SFMLUI::drawViewPageScreen()
{
    if (viewPageStep == 0)
    {
        string hint = buildLikedPagesContent(backend);
        hint += "\n\nType a Page ID in the box below and press Enter.";
        drawGenericScreen("View Page", hint);
        drawInputBox();
        drawStatusMessage();
    }
    else
    {
        drawGenericScreen("Page: " + viewPageId,
            buildOnePageContent(backend, viewPageId));
        Text back; back.setFont(font); back.setCharacterSize(16);
        back.setFillColor(Color(160, 160, 160));
        back.setString("Press Backspace or click Home to go back");
        back.setPosition(290, 840); window.draw(back);
    }
}

// ======================
// POST CARD VIEW - FIXED
// ======================
void SFMLUI::drawPostCard()
{
    Post** posts = backend.getPosts();
    int count = backend.getPostCount();

    if (count == 0 || selectedPost >= count)
    {
        RectangleShape card(Vector2f(1110, 870));
        card.setPosition(278, 10); card.setFillColor(cardColor);
        card.setOutlineThickness(1); card.setOutlineColor(Color(180, 180, 180));
        window.draw(card);

        Text empty;
        empty.setFont(font);
        empty.setString("No post selected or invalid post.");
        empty.setCharacterSize(24);
        empty.setFillColor(textColor);
        empty.setPosition(600, 400);
        window.draw(empty);
        return;
    }

    Post* post = posts[selectedPost];

    RectangleShape card(Vector2f(1060, 580));
    card.setPosition(295, 30);
    card.setFillColor(cardColor);
    card.setOutlineThickness(2);
    card.setOutlineColor(Color(100, 149, 237));
    window.draw(card);

    string posterName = post->getSharedById();
    if (posterName[0] == 'u')
    {
        User* u = backend.findUser(posterName);
        if (u) posterName = u->getName();
    }
    else
    {
        Page* p = backend.findPage(posterName);
        if (p) posterName = p->getTitle();
    }

    Text author;
    author.setFont(font);
    author.setCharacterSize(24);
    author.setStyle(Text::Bold);
    author.setFillColor(textColor);
    author.setString(posterName);
    author.setPosition(315, 45);
    window.draw(author);

    Text date;
    date.setFont(font);
    date.setCharacterSize(16);
    date.setFillColor(Color(140, 140, 140));
    date.setString(
        to_string(post->getDay()) + "/" +
        to_string(post->getMonth()) + "/" +
        to_string(post->getYear())
    );
    date.setPosition(315, 75);
    window.draw(date);

    Text content;
    content.setFont(font);
    content.setCharacterSize(20);
    content.setFillColor(textColor);
    content.setString("\"" + post->getText() + "\"");
    content.setPosition(315, 110);
    window.draw(content);

    Text likes;
    likes.setFont(font);
    likes.setCharacterSize(18);
    likes.setFillColor(Color(100, 149, 237));
    likes.setString("Likes: " + to_string(post->getLikeCount()));
    likes.setPosition(315, 180);
    window.draw(likes);

    Text commentsTitle;
    commentsTitle.setFont(font);
    commentsTitle.setCharacterSize(20);
    commentsTitle.setStyle(Text::Bold);
    commentsTitle.setFillColor(textColor);
    commentsTitle.setString("Comments:");
    commentsTitle.setPosition(315, 220);
    window.draw(commentsTitle);

    float yPos = 255;
    Comment* cmts = post->getComments();
    for (int i = 0; i < post->getCommentCount(); i++)
    {
        string authorID = cmts[i].getAuthorID();
        string authorName = authorID;

        User* u = backend.findUser(authorID);
        if (u) authorName = u->getName();

        Text cmt;
        cmt.setFont(font);
        cmt.setCharacterSize(17);
        cmt.setFillColor(Color(80, 80, 80));
        cmt.setString(authorName + ": \"" + cmts[i].getText() + "\"");
        cmt.setPosition(325, yPos);
        window.draw(cmt);

        yPos += 30;
    }

    window.draw(likeActionBtn.box);
    window.draw(likeActionBtn.text);
    window.draw(commentActionBtn.box);
    window.draw(commentActionBtn.text);
    window.draw(backBtn.box);
    window.draw(backBtn.text);
}

// ======================
// HANDLE EVENTS - FIXED
// ======================
void SFMLUI::handleEvents()
{
    Event e;
    while (window.pollEvent(e))
    {
        if (e.type == Event::Closed) window.close();

        if (showErrorPopup)
        {
            if (e.type == Event::MouseButtonPressed
                || e.type == Event::KeyPressed)
            {
                showErrorPopup = false;
                return;
            }
            continue;
        }

        if (e.type == Event::MouseButtonPressed
            && e.mouseButton.button == Mouse::Left)
        {
            Vector2f mp = window.mapPixelToCoords(Mouse::getPosition(window));
            statusMessage = "";

            if (darkBtn.isClicked(mp))
                toggleDarkMode();
            if (exitBtn.isClicked(mp))
                window.close();
            if (homeBtn.isClicked(mp))
            {
                currentScreen = HOME; scrollOffset = 0;
                shareStep = 0; viewPageStep = 0;
            }
            if (profileBtn.isClicked(mp))
                currentScreen = PROFILE;
            if (timelineBtn.isClicked(mp))
                currentScreen = TIMELINE;
            if (memoryBtn.isClicked(mp))
                currentScreen = MEMORIES;
            if (shareMemoryBtn.isClicked(mp))
            {
                currentScreen = SHARE_MEMORY;
                shareStep = 0; sharePostId = "";
                commentInput.clear(); typingInInput = true;
            }
            if (friendsBtn.isClicked(mp))
                currentScreen = FRIENDS;
            if (pagesBtn.isClicked(mp))
                currentScreen = LIKED_PAGES;
            if (likedPagesBtn.isClicked(mp))
            {
                currentScreen = PAGES;
                viewPageStep = 0; viewPageId = "";
                commentInput.clear(); typingInInput = true;
            }
            if (viewLikedListBtn.isClicked(mp))
                currentScreen = LIKED_LIST;
            if (setUserBtn.isClicked(mp))
            {
                currentScreen = SET_USER;
                commentInput.clear(); typingInInput = true;
            }
            if (setDateBtn.isClicked(mp))
            {
                currentScreen = SET_DATE;
                commentInput.clear(); typingInInput = true;
            }
            if (viewPostBtn.isClicked(mp))
                currentScreen = POST_VIEW;

            if (currentScreen == POST_VIEW)
            {
                if (likeActionBtn.isClicked(mp))    handleLike();
                if (commentActionBtn.isClicked(mp)) handleComment();
                if (backBtn.isClicked(mp))
                {
                    currentScreen = HOME; scrollOffset = 0; statusMessage = "";
                }
            }

            if (currentScreen == HOME)
            {
                Post** posts = backend.getPosts();
                int cnt = backend.getPostCount();

                User* cu2 = backend.getCurrentUser();
                const float cardX = 285.f;
                if (!cu2)
                    return;

                string* fids = cu2->getFriendIDs();
                int fc2 = cu2->getFriendCount();

                string* pids = cu2->getLikedPageIDs();
                int pc2 = cu2->getLikedPageCount();

                float drawY2 = 20.f - scrollOffset;
                int cardIndex = 0;

                for (int i = 0; i < cnt; i++)
                {
                    Post* p = posts[i];
                    string sb = p->getSharedById();

                    if (p->getPostType() == 3) continue;
                    if (p->getLikeCount() == 0) continue;

                    bool rel = false;

                    for (int j = 0; j < fc2; j++)
                        if (sb == fids[j]) rel = true;

                    for (int j = 0; j < pc2; j++)
                        if (sb == pids[j]) rel = true;

                    if (sb == cu2->getId()) rel = true;

                    if (!rel) continue;

                    int vc = min(p->getCommentCount(), 3);
                    float cardH = 170.f + vc * 38.f;

                    FloatRect cardRect(cardX, drawY2, 1080.f, cardH);

                    if (cardRect.contains(mp))
                    {
                        selectedPost = i;
                        currentScreen = POST_VIEW;
                        commentInput.clear();
                        typingInInput = true;
                        return;
                    }

                    drawY2 += cardH + 25.f;
                    cardIndex++;
                }
            }

            typingInInput = inputBox.getGlobalBounds().contains(mp);
        }

        if (e.type == Event::MouseWheelScrolled && currentScreen == HOME)
        {
            scrollOffset -= e.mouseWheelScroll.delta * 30.f;
            if (scrollOffset < 0)scrollOffset = 0;
        }

        if (e.type == Event::TextEntered && typingInInput)
        {
            if (e.text.unicode == '\b')
            {
                if (!commentInput.empty())
                    commentInput.pop_back();
            }
            else if (e.text.unicode == '\r' || e.text.unicode == '\n')
            {
                if (currentScreen == POST_VIEW)
                    handleComment();

                else if (currentScreen == SET_USER && !commentInput.empty())
                {
                    backend.setCurrentUser(commentInput);
                    statusMessage = "User set to: " + commentInput;
                    commentInput.clear();
                    currentScreen = HOME;
                }

                else if (currentScreen == SET_DATE)
                {
                    istringstream iss(commentInput);
                    int d, m, y;
                    if (iss >> d >> m >> y)
                    {
                        if (!isValidDate(d, m, y))
                        {
                            popupMessage = "Invalid date! Check month (1-12) and day."; showErrorPopup = true;
                        }
                        else
                        {
                            backend.setSystemDate(d, m, y);
                            statusMessage =
                                "System Date set to: " +
                                to_string(d) + "/" +
                                to_string(m) + "/" +
                                to_string(y);
                            currentScreen = HOME;
                        }
                    }
                    else
                    {
                        popupMessage = "Format: DD MM YYYY   e.g. 15 11 2017"; showErrorPopup = true;
                    }
                    commentInput.clear();
                }

                else if (currentScreen == SHARE_MEMORY)
                {
                    if (shareStep == 0)
                    {
                        if (commentInput.empty() || isBlank(commentInput))
                        {
                            popupMessage = "Please enter a Post ID."; showErrorPopup = true;
                        }
                        else
                        {
                            Post* orig = backend.findPost(commentInput);
                            if (!orig)
                            {
                                popupMessage = "Post '" + commentInput + "' does not exist!"; showErrorPopup = true;
                            }
                            else
                            {
                                sharePostId = commentInput; shareStep = 1;
                                commentInput.clear();
                            }
                        }
                    }
                    else
                    {
                        if (commentInput.empty() || isBlank(commentInput))
                        {
                            popupMessage = "Caption cannot be empty!"; showErrorPopup = true;
                        }
                        else
                        {
                            backend.shareMemory(sharePostId, commentInput);
                            statusMessage = "Memory shared successfully!";
                            shareStep = 0; sharePostId = "";
                            commentInput.clear();
                            currentScreen = MEMORIES;
                        }
                    }
                }

                else if (currentScreen == PAGES)
                {
                    if (viewPageStep == 0)
                    {
                        if (commentInput.empty() || isBlank(commentInput))
                        {
                            popupMessage = "Please enter a Page ID."; showErrorPopup = true;
                        }
                        else
                        {
                            Page* pg = backend.findPage(commentInput);
                            if (!pg)
                            {
                                popupMessage = "Page '" + commentInput + "' not found!"; showErrorPopup = true;
                            }
                            else
                            {
                                viewPageId = commentInput; viewPageStep = 1; commentInput.clear();
                            }
                        }
                    }
                }
            }
            else if (e.text.unicode >= 32 && e.text.unicode < 128)
                commentInput += (char)e.text.unicode;
        }

        if (e.type == Event::KeyPressed)
        {
            if (e.key.code == Keyboard::BackSpace && currentScreen == PAGES && viewPageStep == 1)
            {
                viewPageStep = 0; viewPageId = "";
            }

            int cnt = backend.getPostCount();
            if (e.key.code == Keyboard::Down && selectedPost < cnt - 1) selectedPost++;
            if (e.key.code == Keyboard::Up && selectedPost > 0)     selectedPost--;
        }
    }
}

// ======================
// DRAW UI - FIXED
// ======================
void SFMLUI::drawUI()
{
    window.clear(bgColor);
    drawSidebar();

    switch (currentScreen)
    {
    case HOME:
        drawHomeFeed();
        break;

    case POST_VIEW:
        drawPostCard();
        drawInputBox();
        drawStatusMessage();
        break;

    case PROFILE:
        drawGenericScreen("My Profile", buildProfileContent(backend));
        break;

    case TIMELINE:
        drawGenericScreen("Timeline", buildTimelineContent(backend));
        break;

    case MEMORIES:
        drawGenericScreen("Memories", buildMemoriesContent(backend));
        break;

    case SHARE_MEMORY:
        drawShareMemoryScreen();
        break;

    case FRIENDS:
        drawGenericScreen("Friend List", buildFriendContent(backend));
        break;

    case LIKED_PAGES:
        drawGenericScreen("My Liked Pages", buildLikedPagesContent(backend));
        break;

    case PAGES:
        drawViewPageScreen();
        break;

    case LIKED_LIST:
        drawGenericScreen("Who Liked My Posts", buildLikedListContent(backend));
        break;

    case SET_USER:
        drawGenericScreen("Set User",
            "Type user ID then press Enter.\n\nCurrent user: "
            + (backend.getCurrentUser()
                ? backend.getCurrentUser()->getName() : "none"));
        drawInputBox();
        drawStatusMessage();
        break;

    case SET_DATE:
        drawGenericScreen("Set Date",
            "Type:  DD MM YYYY  then press Enter.\n\n"
            "Example: 15 11 2017\n\n"
            "Current Date: " +
            to_string(backend.getSystemDay()) + "/" +
            to_string(backend.getSystemMonth()) + "/" +
            to_string(backend.getSystemYear()));
        drawInputBox();
        drawStatusMessage();
        break;
    }

    drawErrorPopup();

    window.display();
}

// ======================
// RUN
// ======================
void SFMLUI::run()
{
    while (window.isOpen())
    {
        handleEvents();
        drawUI();
    }
}
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "AppBackend.h"
#include "AppUI.h"

using namespace sf;
using namespace std;

class SFMLUI
{
private:

    RenderWindow window;

    AppBackend backend;
    AppUI ui;

    // ======================
    // SCREEN ENUM
    // ======================
    enum Screen
    {
        HOME,
        POST_VIEW,
        PROFILE,
        TIMELINE,
        MEMORIES,
        SHARE_MEMORY,
        FRIENDS,
        LIKED_PAGES,
        PAGES,              // FIXED (was VIEW_PAGE)
        LIKED_LIST,
        SET_USER,
        SET_DATE
    };

    Screen currentScreen;

    // ======================
    // GENERAL UI STATE
    // ======================
    float scrollOffset;
    bool typingInInput;
    bool darkMode;

    // ======================
    // POPUPS
    // ======================
    

    bool showErrorPopup;
    string popupMessage;

    string statusMessage;

    // ======================
    // SHARE MEMORY STATE
    // ======================
    int shareStep;          // 0 = enter post ID
    // 1 = enter caption

    string sharePostId;

    // ======================
    // VIEW PAGE STATE
    // ======================
    int viewPageStep;       // 0 = ask page ID
    // 1 = show page

    string viewPageId;

    // ======================
    // GRAPHICS
    // ======================
    Font font;

    Color bgColor;
    Color cardColor;
    Color textColor;
    Color sidebarColor;

    Texture likeTexture;
    Texture commentTexture;

    Sprite likeIcon;
    Sprite commentIcon;

    // ======================
    // INPUT
    // ======================
    string commentInput;

    RectangleShape inputBox;
    Text inputText;

    int selectedPost;

    // ======================
    // BUTTON STRUCT
    // ======================
    struct Button
    {
        RectangleShape box;
        Text text;

        bool isClicked(Vector2f mp)
        {
            return box.getGlobalBounds().contains(mp);
        }
    };

    // ======================
    // SIDEBAR BUTTONS
    // ======================
    Button homeBtn;
    Button timelineBtn;
    Button profileBtn;
    Button memoryBtn;
    Button shareMemoryBtn;

    Button friendsBtn;
    Button pagesBtn;
    Button likedPagesBtn;

    Button setUserBtn;
    Button setDateBtn;

    Button exitBtn;
    Button darkBtn;

    Button viewPostBtn;
    Button viewLikedListBtn;

    // ======================
    // POST ACTION BUTTONS
    // ======================
    Button likeActionBtn;
    Button commentActionBtn;
    Button backBtn;

    // ======================
    // FUNCTIONS
    // ======================
    void setupButton(Button&, string, float, float, Color, Color);

    void handleEvents();

    void drawUI();

    void drawSidebar();
    void drawPostCard();
    void handleLike();

    void handleComment();

    void drawHomeFeed();

    void drawGenericScreen(const string&, const string&);

    void drawInputBox();

    void toggleDarkMode();

    void drawStatusMessage();

    void drawErrorPopup();

    

    void drawShareMemoryScreen();

    void drawViewPageScreen();

    bool isValidDate(int d, int m, int y);

public:

    SFMLUI();

    void run();
};
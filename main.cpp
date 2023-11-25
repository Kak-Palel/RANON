/**
 * File: main.cpp
 * Author: Farrel Ganendra
 * Date: 2023 - 11 - 25
 * Description:
 * 
 * Hello!, this is the main file of my first ever project. This project is called RANON.
 * RANON is a 2d runner and platformer game where the player can go around with their grapplehook
 * and gain score by killing enemies. the game is written with the RAyLIB library, an easy to use
 * library for c and cpp for making games or other apps. I write this game for my assignment in
 * university but I am planning to keep updating the game after I have submitted it to my professor.s
 * I hope whoever played this game enjoys it and have a good time!
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include "raylib.h"

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define MIN(a, b) ((a)<(b)? (a) : (b))

typedef enum GameScreen { TITLE = 0, T_TitleGameplay, GAMEPLAY, DEATHSCREEN, T_TitleSettings, SETTINGS, T_TitleTutorial, TUTORIAL} GameScreen;

/*
=================================================================================================================================================================||
    CLASSES                                                                                                                                                      ||
=================================================================================================================================================================||
*/

/**
 * The Button Class
 * 
 * This class draws button, it doesn't have the functionality of the button so we have to put it
 * the implementation of what the button actually do in the main function. This is because buttons
 * can do almost anything and I can't think of a way to generalize that
 *  
 * to use this class, you need to 3 things:
 * initialization
 * 1. declaration
 * 2. outlineColor
 * 3. textColor
 * 
 * Implementation
 * 1. check the collision between button and the mouse (and then do something)
 * 2. check whether the mouseButton is clicked
 * 3. the implementation of what the button do
 * 
 * draw method
 * call the draw method with a text parameter, this text is whats gonna be displayed on the buttons
 */
class Button
{
    public:
        //button position
        Rectangle area;
        Color buttonColor;
        Color outlineColor;
        int outlineThickness;

        //button text
        Color textColor;
        int textSize = 25;

        void draw(const char *name)
        {
            DrawRectangle(area.x - outlineThickness, area.y - outlineThickness, area.width + 2*outlineThickness, area.height + 2*outlineThickness, outlineColor);
            DrawRectangle(area.x, area.y, area.width, area.height, buttonColor);  
            DrawText(name, area.x + (area.width / 2) - MeasureText(name, textSize)/2, area.y + (area.height / 2) - textSize/2, textSize, textColor);
        }
};

/**
 * The Slider Class
 * 
 * This class is for a slider that can edit a variable type float. This class is created so that
 * user can easily adjusts things in the game so that its to their preferences. it has a textbox at
 * the left side of the slider for the name of thing that its changing, and a number at the right
 * side to show the value of what its changing
 * 
 * to use this class, you need 3 things:
 * Initialization
 * 1. declaration
 * 2. sliderFront - the color of the slider that have a value
 * 3. sliderBack - the color of the slider that doesn't have a value
 * 4. position - the top left corner position of the slider textbox
 * 5. outlinecolor
 * 6. height - fontsize + 2*outlineThickness
 * 7. fontSize
 * 8. minValue - the minimum value of the thing that its changing
 * 9. maxValue - the maximum value of the thing that its changing
 * 10. valueEdit - a pointer to the float variable that its changing
 * 
 * changeValue method
 * you just need to call this method function, it has no parameter and it has a return value (void)
 * 
 * draw method
 * call this function with a text parameter to display the slider to the screen
 * the text parameter is for the name of the slider or the name of the cariable
 * its changing
*/
class Slider
{
    public:
        Vector2 position;
        float height;
        float sliderLength;
        Color textColor;
        Color outlineColor;
        Color sliderBack;
        Color sliderFront;
        Rectangle sliderMain;
        float fontSize;

        float minValue;
        float maxValue;
        float *valueEdit;

        void draw(const char *name)
        {

            sliderMain.x = position.x + MeasureText(name, fontSize) + fontSize*3/2;
            sliderMain.y = position.y + fontSize/2;
            sliderMain.height = height - fontSize;

            DrawRectangle(position.x, position.y, fontSize + MeasureText(name, fontSize), 2*fontSize, sliderFront);
            DrawRectangle(position.x + MeasureText(name, fontSize) + fontSize, position.y, sliderLength, height, outlineColor);
            DrawRectangle(position.x + MeasureText(name, fontSize) + fontSize*3/2, position.y + fontSize/2, sliderLength - fontSize, height - fontSize, sliderBack);
            DrawText(name, position.x + fontSize/2, position.y + fontSize/2, fontSize, textColor);
            
            DrawRectangleRec(sliderMain, sliderFront);
            DrawText(TextFormat("%0.0f", *valueEdit), sliderMain.x + sliderLength, sliderMain.y, fontSize, textColor);

        }

        void changeValue()
        {
            sliderMain.width = (*valueEdit - minValue)/(maxValue - minValue)*(sliderLength - fontSize);

            if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){sliderMain.x, sliderMain.y, sliderLength - fontSize, height - fontSize}))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                {
                    sliderMain.width = GetMousePosition().x - sliderMain.x;
                    *valueEdit = minValue + sliderMain.width*(maxValue-minValue)/(sliderLength - fontSize);
                }
            }
        }
};

/**
 * The checkbox class
 *
 * this class represents a CheckBox object, a CheckBox have a box that can be checked (duh) and a name
 * to give information to the user what it do at the righr side of the box, this objects modify a bool
 * type variable
 * 
 * just like the other ui related classes, you need 3 things to use it
 * Initialization
 * 1. declaration
 * 2. outline color
 * 3. box color
 * 4. checked color - the color of the cross
 * 5. font size
 * 6. outline thickness - its only for the box
 * 7. area.x - the top left x-axis position of the checkbox
 * 8. area.y - the top left y-axis position of the checkbox
 * 
 * changeValue method
 * this method takes a pointer of type boolean as a parameter, this parameter should points to the boolean
 * variable that the checkbox is modifying. this method also returns boolean type, it returns true whenever
 * the checkbox is clicked so that the value of the variable its modifying flipped
 * 
 * draw method
 * the draw method is a void type function that takes a boolean pointer to a boolean variable, it should
 * points to the variable the checkbox is modifying so that the draw method knows wether to draw the checked
 * indicator or not.
*/
class CheckBox
{
    public:
        Color outlineColor;
        Color boxColor;
        Color checkedColor;
        Color textColor;
        Rectangle area;

        float fontSize;
        float outlineThickness;

        void draw(const char *name, bool* value)
        {
            area.width = fontSize + 2*outlineThickness;
            area.height = fontSize + 2*outlineThickness;

            DrawRectangleRec(area, outlineColor);
            DrawRectangle(area.x + outlineThickness, area.y + outlineThickness, area.width - 2*outlineThickness, area.height - 2*outlineThickness, boxColor);
            DrawRectangle(area.x + area.width, area.y, MeasureText(name, fontSize) + 2*outlineThickness, fontSize + 2*outlineThickness, boxColor);

            DrawText(name, area.x + area.width + outlineThickness, area.y + outlineThickness, fontSize, textColor);

            if(*value == true)
            {
                DrawLineEx((Vector2){area.x + outlineThickness, area.y + outlineThickness}, (Vector2){area.x + area.width - outlineThickness, area.y + area.height - outlineThickness}, outlineThickness/3, checkedColor);
            }

        }

        bool changeValue(bool* value)
        {
            if(CheckCollisionPointRec(GetMousePosition(), (Rectangle){area.x + outlineThickness, area.y + outlineThickness, area.width - 2*outlineThickness, area.height - 2*outlineThickness}) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                *value = (*value == false ? true : false);
                return true;
            }
            else return false;
        }
};

/**
 * The Platform class
 * 
 * I made this class just to organize variables, to initialize it, you just neeed to define its area by its top left
 * corner position, its width, and its height, you will also need to define its position as a Vector2 type (raylib's struct)
 * so that it can be put into an array of Vector2 pointers to make it easy to work with multiple platforms, all implementation
 * of the platform is written in the main function.
 * 
 * this class have a draw method to display it on the screen
*/
class Platform
{
    public:
        Rectangle area;
        Vector2 position;
        Vector2 edgePos;
        
        void draw()
        {
            DrawRectangleRec(area, LIGHTGRAY);
        }

};

/**
 * the Projectile class
 * 
 * it represents bullets that the enemy shoot (and in the future Im planning to make the player can shoot as well)
 * 
 * to initialize a projectile, you need to declare its:
 * 1. declaration
 * 2. color
 * 3. position - just declare it to some random position the player and any enemy will never touch
 * 4. define all the boolean variable to false;
 * 
 * every implementation of how the bullet triggered, interacted with other objects, and terminated will be implemented
 * in the main function, the class only have a draw method to display it on the screen when its active
*/
class Projectile
{
    public:

        Vector2 position;
        Vector2 bgDisplacement; //this is the speed vector of the bullet
        bool status; // checks whether the bullet is active or not
        bool hooked; // checks whether the bullet is being hooked by the player or not
        bool redirecting; // - check if the player is finished hooking to the bullet and currently aiming the bullet to redirect it 
        bool redirected; // - checks it the bullet have already redirected by the player 

        Color color;

        void draw()
        {
            DrawCircleV(position, 10, BLUE);
        }

};

/**
 * the Enemy class
 * 
 * I need to change the name of this object to GroundEnemies in the future because im planning to add other types of enemies
 * to the game, anyway, this is an enemy, it doesn't move, its position always follows the position of the platform its on
 * it can shoot bullet to the player and it can also be killed by slashing, hooking, or if it gets hit with a bullet.
 * 
 * to initialize an enemy, you need:
 * 1. declaration
 * 2. length - the radius of the enemy
 * 3. color
 * 4. set status to true
 * 5. set hooked to false
 * 6. set aiming to false
 * 7. platform - the index of the platform its standing on in the platform pointer array
 * 
 * this class have a commented code at the end of the draw method, enable it to see wether the enemy is aiming at you or not.
 * the draw method itself is to display the enemy on the screen
*/
class Enemy
{
    public:

        Vector2 position;
        int length;
        Color color;
        int deathFrame;
        int platform;

        bool hooked; // checks whether the enemy is being hooked by the player or not
        bool status; // checks whether the enemy is alive or not
        bool aiming; // checks whether the enemy can see and aiming to the player or not
        bool terminated; // true if the enemy just got killed

        void draw(int playerPosX, int playerPosY, float groundX, float groundY, float groundLength)
        {
            
            position.x = (int)groundX + groundLength/2;
            position.y = (int)groundY - length;

            if (status) // draw the enemy alive
            {   
                int distance = sqrt(pow(playerPosX - position.x, 2) + pow(playerPosY - position.y, 2));
                DrawCircleV(position, length, color);
                DrawLineEx(position, (Vector2){(float)(position.x - (1.5*length*(position.x - playerPosX)/distance)),(float)(position.y - (1.5*length*(position.y - playerPosY)/distance))}, 10.0f ,color);
            }

            else if (!status) //draw the enemy dead
            {
                DrawCircleSector((Vector2){position.x, position.y -20}, 40, 300.0f, 120.0f, 100, BLUE); 
                DrawCircleSector(position, 40, 120.0f, -60.0f, 100, BLUE);
            }

            if (terminated) // dead animation for sthe enemy
            {
                DrawEllipse(position.x, position.y, length + 3*deathFrame, length/2, GREEN);
                deathFrame++;
                if(deathFrame > 5)
                {
                    terminated = false;
                    deathFrame = 0;
                }
            }
            else {deathFrame = 0;}

            //debugging aiming
            // if (aiming && status)
            // {
            //     DrawLine(playerPosX, playerPosY, position.x, position.y, BLUE);
            // }

        }

};

/**
 * the Player class
 * 
 * tbh I dont need to make the player into a class, I just do it to organize the variable. To initialize a player object, you need:
 * 1. declaration
 * 2. color
 * 3. length - radius of the player
 * 4. swordlength
 * 5. set redirecting to false
 * 6. declare a rectangle for the sword's hitbox
 * 7. declare a float for slashing frames
 * 
 * the draw method takes a float parameter, use the float variable we declared in the initialization part
 * 
 * every other implementation of how the player reacts to enything is written in the main function
*/
class Player
{
    public:
        int length;
        float swordLength;
        Vector2 swordPos; // the position of the edge of the sword
        Vector2 position; // the position of the player
        Color color;
        bool slash; // checks if the player is slashing or not
        bool redirecting; // check if the player is redirecting a bullet or not
        
        void draw(float slashFrame)
        {
            DrawCircle((int)position.x, (int)position.y, length, color);

            if(slash)
            {
                swordPos.x = position.x + (swordLength - swordLength*(slashFrame/31.125f));
                swordPos.y = position.y + sqrt(pow(swordLength, 2) - pow((swordLength - swordLength*(slashFrame/31.125f)), 2));
                DrawLineEx(position, swordPos, 5.0f, GRAY);
            }
        }

};



/*
==================================================================================================================================================================||
    FUNCTIONS                                                                                                                                                     ||
==================================================================================================================================================================||
*/

/**
 * Logoscreen function
 * 
 * it displays the Raylib animation at the start of the program, I tought I should give raylib some credit, so here it is
 * it takes the framesCounter variable as parameter for timing purposes
*/
void logoScreen(int framesCounter)
{
    int logoPositionX;
    int logoPositionY;

    int lettersCount = 0;

    int topSideRecWidth, initialTopSideRecWidth;
    int leftSideRecHeight, initialLeftSideRecHeight;
    int factor = 0;

    int bottomSideRecWidth, initialBottomSideRecWidth;
    int rightSideRecHeight, initialRightSideRecHeight;

    int state = 0;
    float alpha = 1.0f;

    bool skipTitle = 0;

    while (!skipTitle && state < 4)
    {
        if (IsKeyReleased(KEY_SPACE)) {break;}

        logoPositionX = GetScreenWidth()*4/10;
        logoPositionY = GetScreenHeight()*4/10 - GetScreenWidth()/80 ;

        initialTopSideRecWidth = GetScreenWidth()/80;
        initialLeftSideRecHeight = GetScreenWidth()/80;

        initialBottomSideRecWidth = GetScreenWidth()/80;
        initialRightSideRecHeight = GetScreenWidth()/80;

        if (state == 0)
        {
            framesCounter++;

            if (framesCounter == 120)
            {
                state = 1;
                framesCounter = 0;
            }
        }
        else if (state == 1)
        {
            factor += 4;
            topSideRecWidth = initialTopSideRecWidth + GetScreenWidth()*factor/1280;
            leftSideRecHeight = initialLeftSideRecHeight + GetScreenWidth()*factor/1280;

            if (topSideRecWidth >= GetScreenWidth()/5) {factor = 0; state = 2;}
        }
        else if (state == 2)
        {
            factor += 4;
            bottomSideRecWidth = initialBottomSideRecWidth + GetScreenWidth()*factor/1280;
            rightSideRecHeight = initialRightSideRecHeight + GetScreenWidth()*factor/1280;

            if (bottomSideRecWidth >= GetScreenWidth()/5) state = 3;
        }
        else if (state == 3)
        {
            framesCounter++;

            if (framesCounter/12)
            {
                lettersCount++;
                framesCounter = 0;
            }

            if (lettersCount >= 10)
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    state = 4;
                }
            }
        }
        else if (state == 4)
        {
            break;
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (state == 0)
            {
                if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, GetScreenWidth()/80, GetScreenWidth()/80, BLACK);
            }
            else if (state == 1)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, GetScreenWidth()/80, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, GetScreenWidth()/80, leftSideRecHeight, BLACK);
            }
            else if (state == 2)
            {
                DrawRectangle(logoPositionX, logoPositionY, GetScreenWidth()/5, GetScreenWidth()/80, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, GetScreenWidth()/80, GetScreenWidth()/5, BLACK);

                DrawRectangle(logoPositionX + GetScreenWidth()*3/16, logoPositionY, GetScreenWidth()/80, rightSideRecHeight, BLACK);
                DrawRectangle(logoPositionX, logoPositionY + GetScreenWidth()*3/16, bottomSideRecWidth, GetScreenWidth()/80, BLACK);
            }
            else if (state == 3)
            {
                DrawRectangle(logoPositionX, logoPositionY, GetScreenWidth()/5, GetScreenWidth()/80, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY, GetScreenWidth()/80, GetScreenWidth()/5, Fade(BLACK, alpha));

                DrawRectangle(logoPositionX + GetScreenWidth()*3/16, logoPositionY, GetScreenWidth()/80, GetScreenWidth()/5, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + GetScreenWidth()*3/16, GetScreenWidth()/5, GetScreenWidth()/80, Fade(BLACK, alpha));

                DrawText(TextSubtext("raylib", 0, lettersCount), logoPositionX + GetScreenWidth()*3/16 - MeasureText("raylib", GetScreenWidth()*5/128) - GetScreenWidth()/64, logoPositionY + GetScreenWidth()*3/16 - GetScreenWidth()*5/128 - GetScreenWidth()/64, GetScreenWidth()*5/128, Fade(BLACK, alpha));
            }

        EndDrawing();
        
    }
}

/**
 * nameScreen Function
 * 
 * this function displays my name and NRP at the start of the game, it takes framesCountner as parameter for timing purposes
*/
void nameScreen(int framesCounter)
{
    Vector2 namePosition;
    Vector2 nrpPosition;
    float alpha = 0.0f;
    bool skipName = 0;
    int nameSize;
    int nrpSize;


        while (!skipName && framesCounter <= 150)
        {
            BeginDrawing();
            if(IsKeyDown(KEY_SPACE)) {break;}
            nameSize = GetScreenWidth() * 5/128;
            nrpSize = GetScreenWidth() * 5/256;
            namePosition = {(float)(GetScreenWidth()/2 - MeasureText("FARREL GANENDRA", nameSize)/2), (float)(GetScreenHeight()/2 - nameSize - GetScreenHeight()/160)};
            nrpPosition = {(float)(GetScreenWidth()/2 - MeasureText("5024231036", nrpSize)/2), (float)(GetScreenHeight()/2 + GetScreenHeight()/160)};

            if(framesCounter <= 30)
            {
                ClearBackground(RAYWHITE);
                DrawText("FARREL GANENDRA", (int)namePosition.x, (int)namePosition.y, nameSize, Fade(BLACK, alpha));
                DrawText("5024231036", (int)nrpPosition.x, (int)nrpPosition.y, nrpSize, Fade(BLACK, alpha));
                alpha += 0.033f;
            }
            else if(framesCounter > 30 && framesCounter <= 120)
            {
                ClearBackground(RAYWHITE);
                DrawText("FARREL GANENDRA", (int)namePosition.x, (int)namePosition.y, nameSize, BLACK);
                DrawText("5024231036", (int)nrpPosition.x, (int)nrpPosition.y, nrpSize, BLACK);
            }
            else if(framesCounter > 120 && framesCounter <= 150)
            {
                ClearBackground(RAYWHITE);
                DrawText("FARREL GANENDRA", (int)namePosition.x, (int)namePosition.y, nameSize, Fade(BLACK, alpha));
                DrawText("5024231036", (int)nrpPosition.x, (int)nrpPosition.y, nrpSize, Fade(BLACK, alpha));
                alpha -= 0.033f;
            }

            framesCounter++;
            EndDrawing();
        }
    
}

/**
 * checkPlayerGround function
 * 
 * this function checks wether the player is on the ground or currently on air
 * it returns true if the player us on a platform or on the ground, and returns falsee when the player is on air
 * 
 * parameters:
 * 1. Player player - the player its checking
 * 2. Platform Grounds[] - an array of platforms to know their positions
 * 3. groundAmount - the amount of platforms its checking
*/
bool checkPlayerGround(Player player, Platform Grounds[], int groundAmount)
{
    for (int fallCheck = 0; fallCheck <= groundAmount; fallCheck++)
    {
        if
        (
            (player.position.y + player.length == Grounds[fallCheck].area.y && Grounds[fallCheck].area.x  <= player.position.x + player.length/2 && player.position.x + player.length/2 <= Grounds[fallCheck].area.x + Grounds[fallCheck].area.width)||
            (player.position.y + player.length == Grounds[fallCheck].area.y && Grounds[fallCheck].area.x  <= player.position.x - player.length/2 && player.position.x - player.length/2 <= Grounds[fallCheck].area.x + Grounds[fallCheck].area.width)
        )
        {return true;}
    }
    return false;
}

/**
 * checkHookable function
 * 
 * a bool type function that checks if a player can hook to a point, it works by checking if the mouse position from the player is at the same way the points are.
 * it checks by detecting detecting if the gradient of a line from player position to the mouse position is inside the interval with a limit of a gradient of
 * a line from the player position to the point its trying to hook plus or minus tolerance
 * 
 * this functions returns true if the gradient of player-mouse line is between the gradient of player-(platform +- tolerance) line
 * 
 * parameters:
 * 1. Vector2 playerPosition
 * 2. Vector mousePosition
 * 3. float platformX - the x position of the points the player trying to hook
 * 4. float platformY - the y position of the points the player trying to hook
 * 5. float toleranceX - the tolerance in the x axis wether the position of the mouse is in the same way with the point its hooking
 * 6. float toleranceY - the tolerance in the y axis wether the position of the mouse is in the same way with the point its hooking
*/
bool checkHookable(Vector2 playerPosition, Vector2 mousePosition, float platformX, float platformY, float toleranceX, float toleranceY)
{
    float PMGradient = (playerPosition.y - mousePosition.y)/(mousePosition.x - playerPosition.x);
    float upperGradient = (playerPosition.y - platformY + toleranceY)/(platformX + toleranceX - playerPosition.x);
    float lowerGradient = (playerPosition.y - platformY - toleranceY)/(platformX - toleranceX - playerPosition.x);

    if (PMGradient >= std::min(upperGradient, lowerGradient) && PMGradient <= std::max(upperGradient, lowerGradient))
    {
        return true;
    }
    else {return false;}
}

/**
 * the editSettings file
 * 
 * this settings overwrites anything in the settings.txt file with a new value that it gets from the parameters
 * in short, the parameter of this function is every variable the player can modify through the setting
*/
void editSettings(int highScore, bool intro, bool fullScreen,float hookGuidePlat, float hookGuideGrEnem, float hookGuideBullet)
{
    std::ofstream editFile("settings.txt");

    editFile << "High Score: " << highScore << std::endl;
    editFile << "Intro: " << intro << std::endl;
    editFile << "FullScreen: " << fullScreen << std::endl;
    editFile << "HookingGuidePlatLength: " << hookGuidePlat << std::endl;
    editFile << "hookingGuideGroundEnemLength: " << hookGuideGrEnem << std::endl;
    editFile << "hookingGuideBulletLength: " << hookGuideBullet << std::endl;

    editFile.close();
}




/* 
====================================================================================================================================================================||
MAIN FUNCTION                                                                                                                                                       ||
====================================================================================================================================================================||
*/
int main(void)
{

    //variable values from settings.txt
    int highScore;                          // High score
    bool intro;                             // checks if the program should play raylib logo and my name at the start
    bool fullScreen;                        // checks if the program should be run on fullscreen or not
    float hookingGuidePlatLength;           // the length of the hooking guide for platforms
    float hookingGuideGroundEnemLength;     // the length of the hooking guide for enemies
    float hookingGuideBulletLength;         // the length of the hooking guide for bullets

    //read from settings.txt
    std::string readHighscore;
    std::string readIntro;
    std::string readFullscreen;
    std::string readHookingGuidePlatLength;
    std::string readHookingGuideGroundEnemLength;
    std::string readHookingGuideBulletLength;

    std::ifstream settingsRead("settings.txt");
    
    getline(settingsRead, readHighscore);
    highScore = std::stoi(readHighscore.substr(12, readHighscore.size() - 12));

    getline(settingsRead, readIntro);
    intro = (bool)std::stoi(readIntro.substr(7, 1));

    getline(settingsRead, readFullscreen);
    fullScreen = (bool)std::stoi(readFullscreen.substr(12, 1));

    getline(settingsRead, readHookingGuidePlatLength);
    hookingGuidePlatLength = (float)std::stoi(readHookingGuidePlatLength.substr(24, readHookingGuidePlatLength.size() - 24));

    getline(settingsRead, readHookingGuideGroundEnemLength);
    hookingGuideGroundEnemLength = (float)std::stoi(readHookingGuideGroundEnemLength.substr(30, readHookingGuideGroundEnemLength.size() - 30));

    getline(settingsRead, readHookingGuideBulletLength);
    hookingGuideBulletLength = (float)std::stoi(readHookingGuideBulletLength.substr(26, readHookingGuideBulletLength.size() - 26));

    settingsRead.close();

    //main window
    const int screenWidth = 2560;
    const int screenHeight = 1600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "RANON");
    if (fullScreen) {ToggleFullscreen();}
    SetWindowMinSize(320, 240);

    SetExitKey(KEY_DELETE);
    int gameScreenWidth = 2560;
    int gameScreenHeight = 1600;
    GameScreen currentScreen;

    RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    SetTargetFPS(60);
    int frameCounter;


    //-------------------------
    //main menu intialization
    //-------------------------
    Button mainButton;                  //--"PLAY!" button
    mainButton.outlineColor = BLACK;
    mainButton.textColor = BLACK;

    Button settingsButton;              //--"Settings" button
    settingsButton.outlineColor = BLACK;
    settingsButton.textColor = BLACK;

    Button tutorialButton;              //--"Tutorial" button
    tutorialButton.outlineColor = BLACK;
    tutorialButton.textColor = BLACK;

    Button quitButton;                  //--"Quit" button
    quitButton.outlineColor = BLACK;
    quitButton.textColor = BLACK;


    //--------------------------------------------------------
    //transition from TITLE to GAMEPLAY screen initialization
    //--------------------------------------------------------
    int widdenMain;
    int frameTrans;

    bool newHighScore = false;
    Player playerMain;
    playerMain.position = {100, 1200};



    //-------------------------------
    //GAMEPLAY screen initialization
    //-------------------------------
    int score = 0;

    bool jumpState = 0;
    bool fallState = 0;
    int vGravity = 0; //velocity of gravity, not acceleration
    Platform top; //the platform that are right at the top of the player
    Platform bottom; //the platform that are right at the bottom of the player

    int gameFeelFrame = 0;
    bool gameFeel = 0;

    float pgDisplacementX = 0; //speed of the player in x-axis
    float pgDisplacementY = 200; //speed of the player in y-axis

    playerMain.color  = RED;
    playerMain.length = 40;
    playerMain.swordLength = 80;
    playerMain.redirecting = false;
    Rectangle swordHitBox;
    float slashFrame = 0;

    float energy = 1280.0f;

    int grappleCheck;
    float hookTolerance = 80;
    bool hookGuide = false;
    bool hookingMode = false;
    Vector2 hookGuideDraw;

    int bulletRedirect; // the index of the bullet the player is currently redirecting

    //gameplay platforms declaration 
    Platform ground;
    Platform platformG1;
    Platform platformG2;
    Platform platformG3;
    Platform platformG4;
    Platform platformG5;
    Platform platformG6;
    Platform platformG7;
    Platform platformG8;
    Platform platformG9;
    Platform sky;

    top = sky;
    bottom = ground;

    //gameplay platforms area definition
    sky.area = {0, 0, 2560, 1};
    platformG1.area = {273, 213, 365, 80};
    platformG2.area = {1095, 333, 365, 80};
    platformG3.area = {2098, 213, 365, 80};
    platformG4.area = {457, 639, 365, 80};
    platformG5.area = {1737, 639, 365, 80};
    platformG6.area = {2378, 639, 365, 80};
    platformG7.area = {273, 1066, 365, 80};
    platformG8.area = {1096, 946, 365, 80};
    platformG9.area = {2098, 1066, 365, 80};
    ground.area = {0, 1200, 2560, 800};

    //put the platforms, right edge position, and left edge positions of the platforms to a pointer array to make it easier to work with
    Platform* gameplayGrounds[10] = {&ground, &platformG1, &platformG2, &platformG3, &platformG4, &platformG5, &platformG6, &platformG7, &platformG8, &platformG9};
    Vector2* gameplayGroundsPos[10] = {&ground.position, &platformG1.position, &platformG2.position, &platformG3.position, &platformG4.position, &platformG5.position, &platformG6.position, &platformG7.position, &platformG8.position, &platformG9.position};
    Vector2* gameplayGroundsEdgePos[10] = {&ground.edgePos, &platformG1.edgePos, &platformG2.edgePos, &platformG3.edgePos, &platformG4.edgePos, &platformG5.edgePos, &platformG6.edgePos, &platformG7.edgePos, &platformG8.edgePos, &platformG9.edgePos};

    //gameplay enemy initializations
    Enemy enemyG1;
    enemyG1.length = 40;
    enemyG1.color = BLUE;
    enemyG1.status = true;
    enemyG1.hooked = false;
    enemyG1.aiming = false;

    Enemy enemyG2;
    enemyG2.length = 40;
    enemyG2.color = BLUE;
    enemyG2.status = true;
    enemyG2.hooked = false;
    enemyG2.aiming = false;

    Enemy enemyG3;
    enemyG3.length = 40;
    enemyG3.color = BLUE;
    enemyG3.status = true;
    enemyG3.hooked = false;
    enemyG3.aiming = false;

    Enemy enemyG4;
    enemyG4.length = 40;
    enemyG4.color = BLUE;
    enemyG4.status = true;
    enemyG4.hooked = false;
    enemyG4.aiming = false;

    Enemy enemyG5;
    enemyG5.length = 40;
    enemyG5.color = BLUE;
    enemyG5.status = true;
    enemyG5.hooked = false;
    enemyG5.aiming = false;

    //put all the enemy to a pointer array to make it easier to work with
    Enemy* gameplayEnemies[5] = {&enemyG1, &enemyG2, &enemyG3, &enemyG4, &enemyG5};

    //projectiles declarations and definitions
    Projectile bulletG1;
    bulletG1.color = BLUE;
    bulletG1.position = {2560, 1600};
    bulletG1.hooked = false;
    bulletG1.redirecting = false;
    bulletG1.redirected = false;
    bulletG1.status = false;

    Projectile bulletG2;
    bulletG2.color = BLUE;
    bulletG2.position = {2560, 1600};
    bulletG2.hooked = false;
    bulletG2.redirecting = false;
    bulletG2.redirected = false;
    bulletG2.status = false;

    Projectile bulletG3;
    bulletG3.color = BLUE;
    bulletG3.position = {2560, 1600};
    bulletG3.hooked = false;
    bulletG3.redirecting = false;
    bulletG3.redirected = false;
    bulletG3.status = false;

    Projectile bulletG4;
    bulletG4.color = BLUE;
    bulletG4.position = {2560, 1600};
    bulletG4.hooked = false;
    bulletG4.redirecting = false;
    bulletG4.redirected = false;
    bulletG4.status = false;

    Projectile bulletG5;
    bulletG5.color = BLUE;
    bulletG5.position = {2560, 1600};
    bulletG5.hooked = false;
    bulletG5.redirecting = false;
    bulletG5.redirected = false;
    bulletG5.status = false;

    //put all the projectile to a pointer array to make it easier to work with
    Projectile* gameplayBullets[5] = {&bulletG1, &bulletG2, &bulletG3, &bulletG4, &bulletG5};

    Vector2* hookables[28] = // a Vector2 pointer array of all the points the player can hook to
    {
        &platformG1.position,//0
        &platformG2.position, 
        &platformG3.position, 
        &platformG4.position, 
        &platformG5.position, 
        &platformG6.position, 
        &platformG7.position, 
        &platformG8.position, 
        &platformG9.position, 
        &platformG1.edgePos,//9
        &platformG2.edgePos,
        &platformG3.edgePos,
        &platformG4.edgePos,
        &platformG5.edgePos,
        &platformG6.edgePos,
        &platformG7.edgePos,
        &platformG8.edgePos,
        &platformG9.edgePos,
        &enemyG1.position,//18
        &enemyG2.position,
        &enemyG3.position,
        &enemyG4.position,
        &enemyG5.position,
        &bulletG1.position,//23
        &bulletG2.position,
        &bulletG3.position,
        &bulletG4.position,
        &bulletG5.position
    };


    //---------------------------
    //DEATHSCREEN initialization
    //---------------------------
    Button mainMenuButton;                  //"Main Menu" button
    mainMenuButton.outlineColor = BLACK;
    mainMenuButton.textColor = BLACK;
    
    Button retryButton;                     //"Retry" button
    retryButton.outlineColor = BLACK;
    retryButton.textColor = BLACK;


    //------------------------------------------------------------
    //Transition from the TITLE to SETTINGS screen initialization
    //------------------------------------------------------------
    int widdenSetting;



    //-------------------------------
    //Settings screen initialization
    //-------------------------------
    CheckBox fullScreenCheck;                   //Fullscreen CheckBox
    fullScreenCheck.outlineColor = BLACK;
    fullScreenCheck.boxColor = WHITE;
    fullScreenCheck.checkedColor = BLACK;
    fullScreenCheck.textColor = BLACK;
    fullScreenCheck.fontSize = 40;
    fullScreenCheck.outlineThickness = 20;
    fullScreenCheck.area.x = 100;
    fullScreenCheck.area.y = 100;

    CheckBox introCheck;                        //Intro CheckBox
    introCheck.outlineColor = BLACK;
    introCheck.boxColor = WHITE;
    introCheck.checkedColor = BLACK;
    introCheck.textColor = BLACK;
    introCheck.fontSize = 40;
    introCheck.outlineThickness = 20;
    introCheck.area.x = 100;
    introCheck.area.y = 200;

    Slider hookGuidePlatLen;                    //Hooking guide for platforms slider
    hookGuidePlatLen.sliderFront = WHITE;
    hookGuidePlatLen.sliderBack = GRAY;
    hookGuidePlatLen.position.x = 100;
    hookGuidePlatLen.position.y = 300;
    hookGuidePlatLen.outlineColor = BLACK;
    hookGuidePlatLen.textColor = BLACK;
    hookGuidePlatLen.sliderLength = 300;
    hookGuidePlatLen.height = 80;
    hookGuidePlatLen.fontSize = 40;
    hookGuidePlatLen.minValue = 0;
    hookGuidePlatLen.maxValue = 100;
    hookGuidePlatLen.valueEdit = &hookingGuidePlatLength;

    Slider hookGuideGrEnemLen;                   //Hooking guide for enemies slider
    hookGuideGrEnemLen.sliderFront = WHITE;
    hookGuideGrEnemLen.sliderBack = GRAY;
    hookGuideGrEnemLen.position.x = 100;
    hookGuideGrEnemLen.position.y = 400;
    hookGuideGrEnemLen.outlineColor = BLACK;
    hookGuideGrEnemLen.textColor = BLACK;
    hookGuideGrEnemLen.sliderLength = 300;
    hookGuideGrEnemLen.height = 80;
    hookGuideGrEnemLen.fontSize = 40;
    hookGuideGrEnemLen.minValue = 0;
    hookGuideGrEnemLen.maxValue = 20;
    hookGuideGrEnemLen.valueEdit = &hookingGuideGroundEnemLength;

    Slider hookGuideBulletLen;                  //Hooking guide for bullets slider
    hookGuideBulletLen.sliderFront = WHITE;
    hookGuideBulletLen.sliderBack = GRAY;
    hookGuideBulletLen.position.x = 100;
    hookGuideBulletLen.position.y = 500;
    hookGuideBulletLen.outlineColor = BLACK;
    hookGuideBulletLen.textColor = BLACK;
    hookGuideBulletLen.sliderLength = 300;
    hookGuideBulletLen.height = 80;
    hookGuideBulletLen.fontSize = 40;
    hookGuideBulletLen.minValue = 0;
    hookGuideBulletLen.maxValue = 100;
    hookGuideBulletLen.valueEdit = &hookingGuideBulletLength;

    Button resetHighScoreButton;                //reset Highscore button
    resetHighScoreButton.outlineColor = BLACK;
    resetHighScoreButton.textColor = BLACK;

    Button saveAndGoBackButton;                 //Save and go back to title button
    saveAndGoBackButton.outlineColor = BLACK;
    saveAndGoBackButton.textColor = BLACK;


    //--------------------------------------------------------
    //Transition from Title to Tutorial screen initialization
    //--------------------------------------------------------
    int widdenTutorial;

    Player playerTutorial;
    playerTutorial.position = {100, 1200};



    //-------------------------------
    //TUTORIAL screen initialization
    //-------------------------------
    bool jumpState_T = 0;
    bool fallState_T = 0;
    int vGravity_T = 0;
    Platform top_T;
    Platform bottom_T;

    float ptDisplacementX = 0;
    float ptDisplacementY = 200;

    playerTutorial.color = RED;
    playerTutorial.length = 40;
    playerTutorial.swordLength = 80;
    playerTutorial.redirecting = false;
    Rectangle swordHitBox_T;
    float slashFrame_T = 0;

    int grappleCheck_T;
    int hookingGround_T;
    float hookTolerance_T = 80;
    bool hookGuide_T = false;
    bool hookingMode_T = false;
    Vector2 hookGuideDraw_T;

    Platform ground_T;
    Platform platform1;
    Platform platform2;
    Platform platform3;
    Platform platform4;
    Platform platform5;
    Platform sky_T;
    
    top_T = sky_T;
    bottom_T = ground_T;

    sky_T.area = {0, 0, 2560, 1};
    platform1.area = {1098 , 600, 365, 80};
    platform2.area = {550, 800, 365, 80};
    platform3.area = {1645, 800, 365, 80};
    platform4.area = {4480, 800, 365, 80};
    platform5.area = {3840, 800, 365, 80};
    ground_T.area = {0, 1200, 2560, 800};

    Platform* TutorialGrounds[6] = {&ground_T, &platform1, &platform2, &platform3, &platform4, &platform5};
    Vector2* TutorialGroundsPos[10] = {&ground_T.position, &platform1.position, &platform2.position, &platform3.position, &platform4.position, &platform5.position};
    Vector2* TutorialGroundsEdgePos[10] = {&ground_T.edgePos, &platform1.edgePos, &platform2.edgePos, &platform3.edgePos, &platform4.edgePos, &platform5.edgePos};

    Enemy enemy1;
    enemy1.length = 40;
    enemy1.color = BLUE;
    enemy1.status = true;
    enemy1.hooked = false;
    enemy1.aiming = false;
    enemy1.platform = 5;

    Projectile bullet1;
    bullet1.color = BLUE;
    bullet1.position = {2560, 1600};
    bullet1.hooked = false;
    bullet1.redirecting = false;
    bullet1.redirected = false;
    bullet1.status = false;

    Vector2* hookables_T[12] =
    {
        &platform1.position,//0
        &platform2.position,
        &platform3.position,
        &platform4.position,
        &platform5.position,
        &platform1.edgePos,//5
        &platform2.edgePos,
        &platform3.edgePos,
        &platform4.edgePos,
        &platform5.edgePos,
        &enemy1.position,//10
        &bullet1.position//11
    };

    Button resetEnemyTButton;               //the "reset" button
    resetEnemyTButton.outlineColor = BLACK;
    resetEnemyTButton.textColor = BLACK;

    Button backButton;                      //go back to TITLE button
    backButton.outlineColor = BLACK;
    backButton.textColor = BLACK;

    float screenOneTextPos = 100;
    float screenTwoTextPos = 2560;


    //-------------------------------------
    //logo and name before the game starts
    //-------------------------------------
    if (intro)
    {
    logoScreen(0);
    nameScreen(0);
    }

    currentScreen = TITLE;



/*
=====================================================================================================================================================================||
    MAIN LOOP                                                                                                                                                        ||
=====================================================================================================================================================================||
*/
    while(!WindowShouldClose())
    {
        //-----------------------------------
        //screen manager and backend systems
        //-----------------------------------
        switch(currentScreen)
        {
            case TITLE:
            {
                SetTargetFPS(165);

                //main button
                mainButton.area = {(float)GetScreenWidth() * 3/8, (float)GetScreenHeight() * 3/8, (float)GetScreenWidth() / 4, (float)GetScreenHeight() / 4};
                mainButton.textSize = GetScreenWidth() * 3 / 64; //because 1280 * 3/64 = 60, 60 is the font size I want when the window is at the default size (1280 x 800)
                mainButton.outlineThickness = GetScreenWidth() * 3 / 256; //because 1280 * 3/256 = 15, 15 px is the thickness I want when the window is at the default size (1280 x 800)
                widdenMain = 0;

                if (CheckCollisionPointRec(GetMousePosition(), mainButton.area))
                {
                    mainButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        frameTrans = 0;
                        currentScreen = T_TitleGameplay;
                    }
                }
                else {mainButton.buttonColor = WHITE;}
            

                //settings button
                settingsButton.area = {(float)GetScreenWidth() * 447/1280, (float)GetScreenHeight() * 107/160, (float)GetScreenWidth() * 53/640, (float)GetScreenHeight()/25};
                settingsButton.textSize = GetScreenWidth()/64;
                settingsButton.outlineThickness = GetScreenWidth()/128;
                widdenSetting = 0;

                if (CheckCollisionPointRec(GetMousePosition(), settingsButton.area))
                {
                    settingsButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        frameTrans = 0;
                        currentScreen = T_TitleSettings;
                    }
                }
                else{settingsButton.buttonColor = WHITE;}


                //tutorial button
                tutorialButton.area = {(float)GetScreenWidth() * 583/1280, (float)GetScreenHeight() * 107/160, (float)GetScreenWidth() * 53/640, (float)GetScreenHeight()/25};
                tutorialButton.textSize = GetScreenWidth()/64;
                tutorialButton.outlineThickness = GetScreenWidth()/128;
                widdenTutorial = 0;

                if (CheckCollisionPointRec(GetMousePosition(), tutorialButton.area))
                {
                    tutorialButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        frameTrans = 0;
                        currentScreen = T_TitleTutorial;
                    }
                }
                else{tutorialButton.buttonColor = WHITE;}


                //quit button
                quitButton.area = {(float)GetScreenWidth() * 723/1280, (float)GetScreenHeight() * 107/160, (float)GetScreenWidth() * 53/640, (float)GetScreenHeight()/25};
                quitButton.textSize = GetScreenWidth()/64;
                quitButton.outlineThickness = GetScreenWidth()/128;

                if (CheckCollisionPointRec(GetMousePosition(), quitButton.area))
                {
                    quitButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        exit(0);
                    }
                }
                else{quitButton.buttonColor = WHITE;}


                //other buttons
                //.....................

            } break;
            case T_TitleGameplay:
            {
                SetTargetFPS(165);

                if (frameTrans == 80)
                {
                    playerMain.position = {100, 1200};

                    hookingMode = false;
                    newHighScore = false;

                    energy = 1280.0f;

                    top = sky;
                    bottom = ground;

                    platformG1.area = {273, 213, 365, 80};
                    platformG2.area = {1095, 333, 365, 80};
                    platformG3.area = {2098, 213, 365, 80};
                    platformG4.area = {457, 639, 365, 80};
                    platformG5.area = {1737, 639, 365, 80};
                    platformG6.area = {2378, 639, 365, 80};
                    platformG7.area = {273, 1066, 365, 80};
                    platformG8.area = {1096, 946, 365, 80};
                    platformG9.area = {2098, 1066, 365, 80};

                    for (int groundResetTrans = 1; groundResetTrans <= 9; groundResetTrans++)
                    {
                        gameplayGroundsPos[groundResetTrans]->x = gameplayGrounds[groundResetTrans]->area.x;
                        gameplayGroundsPos[groundResetTrans]->y = gameplayGrounds[groundResetTrans]->area.y;
                        gameplayGroundsEdgePos[groundResetTrans]->x = gameplayGrounds[groundResetTrans]->area.x + gameplayGrounds[groundResetTrans]->area.width;
                        gameplayGroundsEdgePos[groundResetTrans]->y = gameplayGrounds[groundResetTrans]->area.y;
                    }

                    enemyG1.status = true;
                    enemyG2.status = true;
                    enemyG3.status = true;
                    enemyG4.status = true;
                    enemyG5.status = true;

                    std::vector<int> platformIndexes = {1, 2, 3, 4, 5, 6, 7, 8};

                    for (int randomizeEnemy = 0; randomizeEnemy <= 4; randomizeEnemy++)
                    {
                        int tempPlace = GetRandomValue(0, 7 - randomizeEnemy);
                        gameplayEnemies[randomizeEnemy]->position.x = gameplayGrounds[platformIndexes[tempPlace]]->area.x + gameplayGrounds[platformIndexes[tempPlace]]->area.width/2;
                        gameplayEnemies[randomizeEnemy]->position.y = gameplayGrounds[platformIndexes[tempPlace]]->area.y;
                        gameplayEnemies[randomizeEnemy]->platform = platformIndexes[tempPlace];
                        platformIndexes.erase(platformIndexes.begin() + tempPlace);
                    }

                    bulletG1.position = {2560, 1600};
                    bulletG2.position = {2560, 1600};
                    bulletG3.position = {2560, 1600};
                    bulletG4.position = {2560, 1600};
                    bulletG5.position = {2560, 1600};

                    bulletG1.status = false;
                    bulletG2.status = false;
                    bulletG3.status = false;
                    bulletG4.status = false;
                    bulletG5.status = false;

                    score = 0;
                    currentScreen = GAMEPLAY;
                }
                frameTrans++;
            }break;
            case GAMEPLAY:
            {

                //slowmotion
                if (IsKeyDown(KEY_LEFT_SHIFT) && energy > 0 && !playerMain.redirecting ) {SetTargetFPS(30); energy -= 8;}
                else if (!playerMain.redirecting){SetTargetFPS(120);}

                //player
                playerMain.position.x += pgDisplacementX;
                playerMain.position.y -= pgDisplacementY;

                pgDisplacementX = 0;





                //player movement------------------------------------------------------------------------------

                //move in x direction
                if (IsKeyDown(KEY_D) && !hookingMode && !playerMain.redirecting)
                {
                    if (playerMain.position.x >= 1280)
                    {

                        for (int groundMove = 1; groundMove <= 9; groundMove++)
                        {
                            gameplayGrounds[groundMove]->area.x -= 8;
                        }

                        top.area.x -= 8;
                        bottom.area.x -= 8;

                    }
                    else {pgDisplacementX = 8;}
                }
                else if (IsKeyDown(KEY_A) && !hookingMode && !playerMain.redirecting && playerMain.position.x >= 10) {pgDisplacementX = -8;}
                else if (!hookingMode && !playerMain.redirecting) {pgDisplacementX = 0;}



                //reuse platforms when they're left behind by the player
                for (int groundRecycle = 1; groundRecycle <= 9; groundRecycle++)
                {
                    if(gameplayGrounds[groundRecycle]->area.x + gameplayGrounds[groundRecycle]->area.width <= 0)
                    {
                        gameplayGrounds[groundRecycle]->area.x = 2560;
                        if (groundRecycle <= 3)
                        {
                            gameplayGrounds[groundRecycle]->area.y =  GetRandomValue(80, 346);
                        }
                        else if (groundRecycle >= 7)
                        {
                            gameplayGrounds[groundRecycle]->area.y =  GetRandomValue(932, 1120);
                        }
                        else
                        {
                            gameplayGrounds[groundRecycle]->area.y =  GetRandomValue(506, 772);
                        }
                    }
                }



                //update ground positions
                for (int groundUpdate = 1; groundUpdate <= 9; groundUpdate++)
                {
                    gameplayGroundsPos[groundUpdate]->x = gameplayGrounds[groundUpdate]->area.x;
                    gameplayGroundsPos[groundUpdate]->y = gameplayGrounds[groundUpdate]->area.y;
                    gameplayGroundsEdgePos[groundUpdate]->x = gameplayGrounds[groundUpdate]->area.x + gameplayGrounds[groundUpdate]->area.width;
                    gameplayGroundsEdgePos[groundUpdate]->y = gameplayGrounds[groundUpdate]->area.y;
                }



                //Make enemies alive again after they're moved from the left edge of the screen to the right one
                for (int resetEnemy = 0; resetEnemy <= 4; resetEnemy++)
                {
                    if (!gameplayEnemies[resetEnemy]->status && gameplayEnemies[resetEnemy]->position.x <= -gameplayEnemies[resetEnemy]->length)
                    {
                    gameplayEnemies[resetEnemy]->status = true;
                    }
                }



                //move in y direction
                //checks nearest platform above and below the player
                if (!(CheckCollisionCircleRec(playerMain.position, playerMain.length, bottom.area))) {bottom = ground;}
                for(int groundCheck = 0; groundCheck <= 9; groundCheck++)
                {   
                    if (!(CheckCollisionCircleRec(playerMain.position, playerMain.length, gameplayGrounds[groundCheck]->area)))
                    {
                        if
                        (
                            (playerMain.position.x + playerMain.length/2 >= gameplayGrounds[groundCheck]->area.x && playerMain.position.x + playerMain.length/2 <= gameplayGrounds[groundCheck]->area.x + gameplayGrounds[groundCheck]->area.width) ||
                            (playerMain.position.x - playerMain.length/2 >= gameplayGrounds[groundCheck]->area.x && playerMain.position.x - playerMain.length/2 <= gameplayGrounds[groundCheck]->area.x + gameplayGrounds[groundCheck]->area.width)
                        )
                        {   

                            if (playerMain.position.y + playerMain.length > gameplayGrounds[groundCheck]->area.y + gameplayGrounds[groundCheck]->area.height && gameplayGrounds[groundCheck]->area.y + gameplayGrounds[groundCheck]->area.height > top.area.y + top.area.height)
                            {
                                top = *gameplayGrounds[groundCheck];
                            }
                            
                            else if (playerMain.position.y - playerMain.length < gameplayGrounds[groundCheck]->area.y && gameplayGrounds[groundCheck]->area.y < bottom.area.y)
                            {
                                bottom = *gameplayGrounds[groundCheck];
                            }

                        }
                        if
                        (
                            !(playerMain.position.x + playerMain.length/2 >= top.area.x && playerMain.position.x + playerMain.length/2 <= top.area.x + top.area.width) &&
                            !(playerMain.position.x - playerMain.length/2 >= top.area.x && playerMain.position.x - playerMain.length/2 <= top.area.x + top.area.width)
                        )
                        {
                            top = sky;
                        }
                    }
                }



                //reset acceleration in the y direction when not jumping or falling or hooking or redirecting
                if (!fallState && !jumpState && !hookingMode && !playerMain.redirecting) {vGravity = 0;}



                //triggers jump
                if (IsKeyDown(KEY_W) && !jumpState  && !fallState && checkPlayerGround(playerMain, *gameplayGrounds, 9))
                {
                    jumpState = true;
                }

                //jumping
                if (jumpState)
                {
                    pgDisplacementY = 20 - vGravity;
                    vGravity += 1;
                }



                //checks if the player is currently on air or not
                if (!jumpState && !fallState && !hookingMode && !playerMain.redirecting)
                {
                    if (!checkPlayerGround(playerMain, *gameplayGrounds, 9))
                    {vGravity = 0; fallState = true; goto onAir;}
                }
                onAir:;



                //falling
                if (fallState)
                {
                    pgDisplacementY = vGravity;
                    vGravity -= 1;
                }



                //prevent the player to clip through gameplayGrounds
                if (playerMain.position.y + playerMain.length > bottom.area.y && !hookingMode && !playerMain.redirecting)
                {
                    pgDisplacementY = 0;
                    playerMain.position.y = bottom.area.y - playerMain.length;
                    fallState = false;
                    jumpState = false;
                }
                if (playerMain.position.y - playerMain.length < top.area.y + top.area.height && !hookingMode && !playerMain.redirecting)
                {
                    pgDisplacementY = 0;
                    playerMain.position.y =  top.area.y + top.area.height + playerMain.length;
                    jumpState = false;
                    fallState = true;
                }



                //hooking movement
                //hook trigger
                float hookSpeedY;
                if (!hookingMode && !playerMain.redirecting)
                {
                    for (grappleCheck = 0; grappleCheck <= 27; grappleCheck++)
                    {
                        float length = sqrt(pow(hookables[grappleCheck]->x - playerMain.position.x, 2) + pow(hookables[grappleCheck]->y - playerMain.position.y, 2));
                        if //platforms in the right side of the player
                        (
                            grappleCheck <= 8 &&
                            playerMain.position.x < hookables[grappleCheck]->x - 2*playerMain.length &&
                            playerMain.position.x < GetMousePosition().x && 
                            checkHookable(playerMain.position, GetMousePosition(), hookables[grappleCheck]->x, hookables[grappleCheck]->y, 0, hookTolerance) &&
                            length <= 480
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && energy >= 200)
                            {
                                hookingMode = true;
                                hookSpeedY = 10*(playerMain.position.y - hookables[grappleCheck]->y + playerMain.length)/(hookables[grappleCheck]->x - playerMain.position.x);
                                energy -= 300;
                            }
                            hookGuide = true;
                            hookGuideDraw.x = hookables[grappleCheck]->x;
                            hookGuideDraw.y = hookables[grappleCheck]->y;
                            goto hookingNow;
                        }
                        else if //platforms in the left side of the player
                        (
                            grappleCheck >= 9 && grappleCheck <= 17 &&
                            playerMain.position.x > hookables[grappleCheck]->x + 2*playerMain.length && 
                            playerMain.position.x > GetMousePosition().x && 
                            checkHookable(playerMain.position, GetMousePosition(), hookables[grappleCheck]->x, hookables[grappleCheck]->y, 0, hookTolerance) &&
                            length <= 480
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && energy >= 200)
                            {
                                hookingMode = true;
                                hookSpeedY = 10*(playerMain.position.y - hookables[grappleCheck]->y + playerMain.length)/(playerMain.position.x - hookables[grappleCheck]->x);
                                energy -= 300;
                            }
                            hookGuide = true;
                            hookGuideDraw.x = hookables[grappleCheck]->x;
                            hookGuideDraw.y = hookables[grappleCheck]->y;
                            goto hookingNow;
                        }
                        else if //enemies
                        (
                            grappleCheck >= 18 && grappleCheck <= 22 &&
                            playerMain.position.y <= hookables[grappleCheck]->y + platformG1.area.height + enemyG1.length &&
                            checkHookable(playerMain.position, GetMousePosition(), hookables[grappleCheck]->x, hookables[grappleCheck]->y, gameplayEnemies[grappleCheck - 18]->length, hookTolerance/2) &&
                            ((playerMain.position.x <= hookables[grappleCheck]->x - 2*playerMain.length && playerMain.position.x <= GetMousePosition().x) || (playerMain.position.x >= hookables[grappleCheck]->x + 2*playerMain.length && playerMain.position.x >= GetMousePosition().x)) &&
                            gameplayEnemies[grappleCheck-18]->status &&
                            length <= 480
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && energy >= 200)
                            {
                                hookingMode = true;
                                gameplayEnemies[grappleCheck - 18]->hooked = true;
                                hookSpeedY = 10*(hookables[grappleCheck]->y - playerMain.position.y)/(playerMain.position.x - hookables[grappleCheck]->x);
                                energy -= 300;
                            }
                            hookGuide = true;
                            hookGuideDraw.x = hookables[grappleCheck]->x;
                            hookGuideDraw.y = hookables[grappleCheck]->y;
                            goto hookingNow;
                        }
                        else if //Bullets
                        (
                            grappleCheck >= 23 && grappleCheck <= 27 &&
                            checkHookable(playerMain.position, GetMousePosition(), hookables[grappleCheck]->x, hookables[grappleCheck]->y, hookTolerance/2, hookTolerance/2) &&
                            ((playerMain.position.x <= hookables[grappleCheck]->x - 2*playerMain.length && playerMain.position.x <= GetMousePosition().x) || (playerMain.position.x >= hookables[grappleCheck]->x + 2*playerMain.length && playerMain.position.x >= GetMousePosition().x)) &&
                            gameplayBullets[grappleCheck - 23]->status &&
                            !gameplayBullets[grappleCheck - 23]->redirected
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && energy >= 200 && !gameplayBullets[grappleCheck - 23]->hooked)
                            {
                                gameplayBullets[grappleCheck - 23]->hooked = true;
                                gameplayBullets[grappleCheck - 23]->bgDisplacement.x = 0;
                                gameplayBullets[grappleCheck - 23]->bgDisplacement.y = 0;

                                if (playerMain.position.x < hookables[grappleCheck]->x)
                                {
                                    hookingMode = true;
                                    hookSpeedY = 10*(playerMain.position.y - hookables[grappleCheck]->y)/(hookables[grappleCheck]->x - playerMain.position.x);
                                    energy -= 300;
                                }
                                else if (playerMain.position.x > hookables[grappleCheck]->x)
                                {
                                    hookingMode = true;
                                    hookSpeedY = 10*(playerMain.position.y - hookables[grappleCheck]->y)/(playerMain.position.x - hookables[grappleCheck]->x);
                                    energy -= 300;
                                }
                            }
                            hookGuide = true;
                            hookGuideDraw.x = hookables[grappleCheck]->x;
                            hookGuideDraw.y = hookables[grappleCheck]->y;
                            goto hookingNow;
                        }
                        else {hookGuide = false;}
                    }
                    hookingNow:;
                }

                //hooking mechanism
                if (hookingMode)
                {
                    if (hookables[grappleCheck]->x > playerMain.position.x)
                    {
                        if (playerMain.position.x >= 1280)
                        {

                            for (int hookMove = 1; hookMove <= 9; hookMove++)
                            {
                                gameplayGrounds[hookMove]->area.x -= 10;
                            }

                            top.area.x -= 10;
                            bottom.area.x -= 10;
                            hookGuideDraw.x -= 10;

                            if (grappleCheck >= 23 && grappleCheck <= 27)
                            {
                                hookables[grappleCheck]->x -= 10;
                            }

                            pgDisplacementX = 0;
                            pgDisplacementY = hookSpeedY; 

                        }
                        else
                        {
                            pgDisplacementX = 10;
                            pgDisplacementY = hookSpeedY;
                        }
                    }
                    else if (hookables[grappleCheck]->x < playerMain.position.x)
                    {
                        pgDisplacementX = -10;

                        if (grappleCheck >= 18 && grappleCheck <= 22)
                        {
                            pgDisplacementY = -hookSpeedY;
                        }
                        else
                        {
                            pgDisplacementY = hookSpeedY;
                        }
                    }
                    
                    //stops hooking mode when its done
                    int xRangeMin, xRangeMax;
                    if (grappleCheck <= 8) {xRangeMin = grappleCheck; xRangeMax = grappleCheck + 9;}
                    else if (grappleCheck >= 9 && grappleCheck <= 17) {xRangeMin = grappleCheck - 9; xRangeMax = grappleCheck;}
                    
                    if //platforms
                    (
                        grappleCheck <= 17 &&
                        playerMain.position.y <= hookables[grappleCheck]->y &&
                        playerMain.position.y >= hookables[grappleCheck]->y - 3*playerMain.length &&
                        playerMain.position.x <= hookables[xRangeMax]->x &&
                        playerMain.position.x >= hookables[xRangeMin]->x
                    )
                    {
                        if (grappleCheck <= 8)
                        {
                            playerMain.position.x = hookables[xRangeMin]->x + playerMain.length;
                            bottom = *gameplayGrounds[grappleCheck + 1];
                        }
                        else if (grappleCheck >= 9 && grappleCheck <= 17)
                        {
                            playerMain.position.x = hookables[xRangeMax]->x - playerMain.length;
                            bottom = *gameplayGrounds[grappleCheck - 8];
                        }

                        playerMain.position.y = hookGuideDraw.y - playerMain.length;
                        pgDisplacementY = 0;
                        hookingMode = false; 
                    }
                    else if //enemies
                    (
                        grappleCheck >= 18 && grappleCheck <= 22 &&
                        playerMain.position.y <= hookables[grappleCheck]->y + gameplayEnemies[grappleCheck - 18]->length &&
                        playerMain.position.y >= hookables[grappleCheck]->y - gameplayEnemies[grappleCheck - 18]->length &&
                        playerMain.position.x <= hookables[grappleCheck]->x + playerMain.swordLength &&
                        playerMain.position.x >= hookables[grappleCheck]->x - playerMain.swordLength
                    )
                    {
                        gameplayEnemies[grappleCheck - 18]->status = false;
                        gameplayEnemies[grappleCheck - 18]->terminated = true;
                        energy += 400;
                        score++;
                        playerMain.position.x = gameplayEnemies[grappleCheck - 18]->position.x;
                        playerMain.position.y = gameplayEnemies[grappleCheck - 18]->position.y;
                        pgDisplacementY = 0;
                        gameFeel = true;
                        hookingMode = false;

                        gameplayBullets[grappleCheck - 18]->position = {2560, 1600};
                        gameplayBullets[grappleCheck - 18]->status = false;
                        gameplayBullets[grappleCheck - 18]->hooked = false;
                        gameplayBullets[grappleCheck - 18]->redirected = false;
                        gameplayBullets[grappleCheck - 18]->bgDisplacement.x = 0;
                        gameplayBullets[grappleCheck - 18]->bgDisplacement.y = 0;
                    }
                    else if //bullets
                    (
                        grappleCheck >= 23 && grappleCheck <= 27 &&
                        playerMain.position.y <= hookables[grappleCheck]->y + playerMain.length &&
                        playerMain.position.y >= hookables[grappleCheck]->y - playerMain.length &&
                        playerMain.position.x <= hookables[grappleCheck]->x + playerMain.length &&
                        playerMain.position.x >= hookables[grappleCheck]->x - playerMain.length
                    )
                    {
                        bulletRedirect = grappleCheck - 23;
                        playerMain.redirecting = true;
                        pgDisplacementX = 0;
                        pgDisplacementY = 0;
                        SetTargetFPS(15);
                        gameplayBullets[grappleCheck - 23]->redirecting = true;
                        hookingMode = false;
                    }

                }



                //player and enemy combat------------------------------------------------------------------------------
                //slashing trigger
                if(IsKeyReleased(KEY_SPACE) && energy >= 200 && !playerMain.slash)
                {
                    playerMain.slash = true;
                }

                //slashing mechanism
                if (playerMain.slash)
                {
                    //define hitbox area
                    swordHitBox.height = playerMain.swordLength;
                    swordHitBox.width = playerMain.swordLength;
                    swordHitBox.y = playerMain.position.y;
                    if (slashFrame <= 31.125f)
                    {
                        swordHitBox.x = playerMain.position.x;
                    }
                    if (slashFrame >= 31.125f)
                    {
                        swordHitBox.x = playerMain.position.x - playerMain.swordLength;
                    }

                    // check if the sword hits an enemy
                    for (int enemyCheck = 0; enemyCheck < 5; enemyCheck++)
                    {
                        if (CheckCollisionCircleRec(gameplayEnemies[enemyCheck]->position, gameplayEnemies[enemyCheck]->length, swordHitBox) && gameplayEnemies[enemyCheck]->status)
                        {
                            gameplayEnemies[enemyCheck]->status = false;
                            gameplayEnemies[enemyCheck]->terminated = true;
                            energy += 400.0f;
                            score++;
                            gameFeel = true;

                            gameplayBullets[enemyCheck]->position = {2560, 1600};
                            gameplayBullets[enemyCheck]->status = false;
                            gameplayBullets[enemyCheck]->hooked = false;
                            gameplayBullets[enemyCheck]->redirected = false;
                            gameplayBullets[enemyCheck]->bgDisplacement.x = 0;
                            gameplayBullets[enemyCheck]->bgDisplacement.y = 0;
                        }
                    }

                    //slashin state control
                    slashFrame++;
                    if (slashFrame >= 63)
                    {
                        slashFrame = 0;
                        playerMain.slash = false;
                    }
                }



                //projectile implementation
                //checks if the player is visible from an enemy
                for (int aimingCheck = 0; aimingCheck <= 4; aimingCheck++)
                {
                    gameplayEnemies[aimingCheck]->aiming = true;
                    for(int aimBlock = 0; aimBlock <= 9; aimBlock++)
                    {
                        Vector2 collisionPoint;
                        if
                        (
                            CheckCollisionLines
                            (
                                gameplayEnemies[aimingCheck]->position,
                                playerMain.position,
                                (Vector2){gameplayGrounds[aimBlock]->area.x, gameplayGrounds[aimBlock]->area.y},
                                (Vector2){gameplayGrounds[aimBlock]->area.x + gameplayGrounds[aimBlock]->area.width, gameplayGrounds[aimBlock]->area.y + gameplayGrounds[aimBlock]->area.height},
                                &collisionPoint
                            )
                            ||
                            CheckCollisionLines
                            (
                                gameplayEnemies[aimingCheck]->position,
                                playerMain.position,
                                (Vector2){gameplayGrounds[aimBlock]->area.x + gameplayGrounds[aimBlock]->area.width, gameplayGrounds[aimBlock]->area.y},
                                (Vector2){gameplayGrounds[aimBlock]->area.x, gameplayGrounds[aimBlock]->area.y + gameplayGrounds[aimBlock]->area.height},
                                &collisionPoint
                            )
                        )
                        {
                            gameplayEnemies[aimingCheck]->aiming = false;
                        }
                    }
                }

                //randomize whether the enemy should shoot the player or not and also assign velocity to bullets that are shot
                for (int shouldShoot = 0; shouldShoot <= 4; shouldShoot++)
                {
                    if(gameplayEnemies[shouldShoot]->aiming && !gameplayBullets[shouldShoot]->status)
                    {
                        if (GetRandomValue(1, 165) == 1 && gameplayEnemies[shouldShoot]->status && sqrt(pow(playerMain.position.x - gameplayEnemies[shouldShoot]->position.x, 2) + pow(playerMain.position.y - gameplayEnemies[shouldShoot]->position.y, 2)) >= 300 && !hookingMode)
                        {
                            gameplayBullets[shouldShoot]->status = true;
                            gameplayBullets[shouldShoot]->position = gameplayEnemies[shouldShoot]->position;

                            if (playerMain.position.x <= gameplayBullets[shouldShoot]->position.x)
                            {
                                gameplayBullets[shouldShoot]->bgDisplacement.x = -12;
                                gameplayBullets[shouldShoot]->bgDisplacement.y = 12*(gameplayEnemies[shouldShoot]->position.y - playerMain.position.y)/(gameplayEnemies[shouldShoot]->position.x - playerMain.position.x);
                            }
                            else if (playerMain.position.x >= gameplayBullets[shouldShoot]->position.x)
                            {
                                gameplayBullets[shouldShoot]->bgDisplacement.x = 12;
                                gameplayBullets[shouldShoot]->bgDisplacement.y = 12*(gameplayEnemies[shouldShoot]->position.y - playerMain.position.y)/(playerMain.position.x - gameplayEnemies[shouldShoot]->position.x);
                            }
                        }
                    }
                }

                //moving the bullet and collision check
                for (int bulletMove = 0; bulletMove <= 4; bulletMove++)
                {
                    if (gameplayBullets[bulletMove]->status && !gameplayBullets[bulletMove]->hooked && gameplayEnemies[bulletMove]->status)
                    {
                        gameplayBullets[bulletMove]->position.x += gameplayBullets[bulletMove]->bgDisplacement.x;
                        gameplayBullets[bulletMove]->position.y -= gameplayBullets[bulletMove]->bgDisplacement.y;

                        //reset the bullet when it goes out of the screen
                        if
                        (
                            gameplayBullets[bulletMove]->position.x > 2560 ||
                            gameplayBullets[bulletMove]->position.x < 0 ||
                            gameplayBullets[bulletMove]->position.y < 0 ||
                            gameplayBullets[bulletMove]->position.y > 1600
                        )
                        {
                            gameplayBullets[bulletMove]->bgDisplacement.x = 0;
                            gameplayBullets[bulletMove]->bgDisplacement.y = 0;

                            gameplayBullets[bulletMove]->position = {2560, 1600};
                            gameplayBullets[bulletMove]->status = false;
                            gameplayBullets[bulletMove]->hooked = false;
                            gameplayBullets[bulletMove]->redirected = false;
                        }

                        //collision check of bullet with the player
                        if (CheckCollisionCircles(gameplayBullets[bulletMove]->position, 10, playerMain.position, playerMain.length) && !gameplayBullets[bulletMove]->redirected && !gameplayBullets[bulletMove]->redirecting)
                        {
                            gameplayBullets[bulletMove]->bgDisplacement.x = 0;
                            gameplayBullets[bulletMove]->bgDisplacement.y = 0;

                            gameplayBullets[bulletMove]->position = {2560, 1600};
                            gameplayBullets[bulletMove]->status = false;
                            gameplayBullets[bulletMove]->hooked = false;
                            gameplayBullets[bulletMove]->redirected = false;
                            currentScreen = DEATHSCREEN;
                            goto skipChecking;
                        }

                        //collision check of bullet with platforms
                        for (int checkCollisionBulletPlatforms = 0; checkCollisionBulletPlatforms <= 9; checkCollisionBulletPlatforms++)
                        {
                            if (CheckCollisionCircleRec(gameplayBullets[bulletMove]->position, 10, gameplayGrounds[checkCollisionBulletPlatforms]->area))
                            {
                                gameplayBullets[bulletMove]->bgDisplacement.x = 0;
                                gameplayBullets[bulletMove]->bgDisplacement.y = 0;

                                gameplayBullets[bulletMove]->position = {2560, 1600};
                                gameplayBullets[bulletMove]->status = false;
                                gameplayBullets[bulletMove]->hooked = false;
                                gameplayBullets[bulletMove]->redirected = false;
                                goto skipChecking;
                            }
                        }

                        //collision check of bullet with other enemies
                        for (int checkCollisionBulletEnemies = 0; checkCollisionBulletEnemies <= 4; checkCollisionBulletEnemies++)
                        {
                            if (CheckCollisionCircles(gameplayBullets[bulletMove]->position, 10, gameplayEnemies[checkCollisionBulletEnemies]->position, gameplayEnemies[checkCollisionBulletEnemies]->length) && gameplayEnemies[checkCollisionBulletEnemies]->status && (bulletMove != checkCollisionBulletEnemies || gameplayBullets[bulletMove]->redirected))
                            {

                                gameplayEnemies[checkCollisionBulletEnemies]->status = false;
                                gameplayEnemies[checkCollisionBulletEnemies]->terminated = true;

                                if(gameplayBullets[checkCollisionBulletEnemies]->redirected)
                                {
                                    gameFeel = true;
                                    energy += 400.0f;
                                    score++;
                                }

                                gameplayBullets[checkCollisionBulletEnemies]->position = {2560, 1600};
                                gameplayBullets[checkCollisionBulletEnemies]->status = false;
                                gameplayBullets[checkCollisionBulletEnemies]->hooked = false;
                                gameplayBullets[checkCollisionBulletEnemies]->redirected = false;
                                gameplayBullets[checkCollisionBulletEnemies]->bgDisplacement.x = 0;
                                gameplayBullets[checkCollisionBulletEnemies]->bgDisplacement.y = 0;

                                gameplayBullets[bulletMove]->position = {2560, 1600};
                                gameplayBullets[bulletMove]->status = false;
                                gameplayBullets[bulletMove]->hooked = false;
                                gameplayBullets[bulletMove]->redirected = false;
                                gameplayBullets[bulletMove]->bgDisplacement.x = 0;
                                gameplayBullets[bulletMove]->bgDisplacement.y = 0;

                                goto skipChecking;
                            }
                        }

                        skipChecking:;
                    }
                }

                //redirecting projectile
                if (playerMain.redirecting)
                {
                    playerMain.position.x = gameplayBullets[bulletRedirect]->position.x;
                    playerMain.position.y = gameplayBullets[bulletRedirect]->position.y;

                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if (GetMousePosition().x <= gameplayBullets[grappleCheck - 23]->position.x)
                        {
                            gameplayBullets[grappleCheck - 23]->bgDisplacement.x = -12;
                            gameplayBullets[grappleCheck - 23]->bgDisplacement.y = 12*(playerMain.position.y - GetMousePosition().y)/(playerMain.position.x - GetMousePosition().x);
                        }
                        else if (GetMousePosition().x >= gameplayBullets[grappleCheck - 23]->position.x)
                        {
                            gameplayBullets[grappleCheck - 23]->bgDisplacement.x = 12;
                            gameplayBullets[grappleCheck - 23]->bgDisplacement.y = 12*(playerMain.position.y - GetMousePosition().y)/(GetMousePosition().x - playerMain.position.x);
                        }

                        gameplayBullets[grappleCheck - 23]->hooked = false;
                        gameplayBullets[grappleCheck - 23]->redirecting = false;
                        gameplayBullets[grappleCheck - 23]->redirected = true;

                        playerMain.redirecting = false;
                    }
                }

                for (int deadEnemyReset = 0; deadEnemyReset <= 4; deadEnemyReset++)
                {
                    if(!gameplayEnemies[deadEnemyReset]->status)
                    {
                        gameplayBullets[deadEnemyReset]->position = {2560, 1600};
                        gameplayBullets[deadEnemyReset]->status = false;
                        gameplayBullets[deadEnemyReset]->hooked = false;
                        gameplayBullets[deadEnemyReset]->redirected = false;
                        gameplayBullets[deadEnemyReset]->bgDisplacement.x = 0;
                        gameplayBullets[deadEnemyReset]->bgDisplacement.y = 0;
                    }
                }



                //energy limits
                if(energy < 0)
                {energy = 0;}
                else if (energy > 2560)
                {energy = 2560;}

                //free energy recharge
                if(frameCounter % 8 == 0 && energy <= 1280)
                {energy++;}

                //game feel slow mo
                if (gameFeel)
                {
                    SetTargetFPS(15);
                    gameFeelFrame++;
                    if (gameFeelFrame >= 5)
                    {
                        gameFeelFrame = 0;
                        gameFeel = false;
                    }
                }

                //back button
                backButton.area = {2495 -  platform4.area.width*3/4, 65, platform4.area.width*3/4, platform4.area.height * 2};
                backButton.textSize = 35; 
                backButton.outlineThickness = 15; 

                if (CheckCollisionPointRec(GetMousePosition(), backButton.area))
                {
                    backButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        currentScreen = TITLE;
                    }
                }
                else {backButton.buttonColor = WHITE;}

            } break;
            case DEATHSCREEN:
            {
                SetTargetFPS(60);

                if (highScore < score)
                {
                    newHighScore = true;
                    highScore = score;

                    editSettings(score, intro, fullScreen, hookingGuidePlatLength, hookingGuideGroundEnemLength, hookingGuideBulletLength);
                }

                //main menu Button
                mainMenuButton.area = {1280 - MeasureText(TextFormat("%i", score), 300)/2.0f - 870, 600, 800, 300};
                mainMenuButton.textSize = 120; //because 1280 * 3/64 = 60, 60 is the font size I want when the window is at the default size (1280 x 800)
                mainMenuButton.outlineThickness = 30; //because 1280 * 3/256 = 15, 15 px is the thickness I want when the window is at the default size (1280 x 800)

                if (CheckCollisionPointRec(GetMousePosition(), mainMenuButton.area))
                {
                    mainMenuButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        currentScreen = TITLE;
                    }
                }
                else {mainMenuButton.buttonColor = WHITE;}
                
                //Retry Button
                retryButton.area = {1280 + MeasureText(TextFormat("%i", score), 300)/2.0f + 70, 600, 800, 300};
                retryButton.textSize = 120; //because 1280 * 3/64 = 60, 60 is the font size I want when the window is at the default size (1280 x 800)
                retryButton.outlineThickness = 30; //because 1280 * 3/256 = 15, 15 px is the thickness I want when the window is at the default size (1280 x 800)

                if (CheckCollisionPointRec(GetMousePosition(), retryButton.area))
                {
                    retryButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        frameTrans = 0;
                        currentScreen = T_TitleGameplay;
                    }
                }
                else {retryButton.buttonColor = WHITE;}
            }break;
            case T_TitleSettings:
            {
                if (frameTrans == 80)
                {
                    currentScreen = SETTINGS;
                }
                frameTrans++;
            }break;
            case SETTINGS:
            {
                //checkboxes
                if (fullScreenCheck.changeValue(&fullScreen)) {ToggleFullscreen();}
                introCheck.changeValue(&intro);

                //hooking guide platforms slider
                hookGuidePlatLen.changeValue();
                hookGuideGrEnemLen.changeValue();
                hookGuideBulletLen.changeValue();

                //reset highscrore button
                resetHighScoreButton.area = {120, 620, 40.0f + MeasureText("Reset highscore", 40), 80};
                resetHighScoreButton.textSize = 40;
                resetHighScoreButton.outlineThickness = 20;

                if (CheckCollisionPointRec(GetMousePosition(), resetHighScoreButton.area))
                {
                    resetHighScoreButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        highScore = 0;
                    }
                }
                else {resetHighScoreButton.buttonColor = WHITE;}

                //save and go back button
                saveAndGoBackButton.area = {1280 - (40.0f + MeasureText("Save and go back", 40))/2, 1380, 40.0f + MeasureText("Save and go back", 40), 80};
                saveAndGoBackButton.textSize = 40;
                saveAndGoBackButton.outlineThickness = 20;

                if(CheckCollisionPointRec(GetMousePosition(), saveAndGoBackButton.area))
                {
                    saveAndGoBackButton.buttonColor = GRAY;
                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        editSettings(highScore, intro, fullScreen, hookingGuidePlatLength, hookingGuideGroundEnemLength, hookingGuideBulletLength);
                        currentScreen = TITLE;
                    }
                }
                else {saveAndGoBackButton.buttonColor = WHITE;}
            }break;
            case T_TitleTutorial:
            {
                SetTargetFPS(165);

                if (frameTrans == 80)
                {
                    playerTutorial.position = {360, 1200};

                    hookingMode_T = false;

                    top_T = sky_T;
                    bottom_T = ground_T;

                    platform1.area = {1098 , 600, 365, 80};
                    platform2.area = {550, 800, 365, 80};
                    platform3.area = {1645, 800, 365, 80};
                    platform4.area = {3840, 800, 365, 80};
                    platform5.area = {2560, 800, 365, 80};

                    for (int groundResetTrans_T = 1; groundResetTrans_T <= 5; groundResetTrans_T++)
                    {
                        TutorialGroundsPos[groundResetTrans_T]->x = TutorialGrounds[groundResetTrans_T]->area.x;
                        TutorialGroundsPos[groundResetTrans_T]->y = TutorialGrounds[groundResetTrans_T]->area.y;
                        TutorialGroundsEdgePos[groundResetTrans_T]->x = TutorialGrounds[groundResetTrans_T]->area.x + TutorialGrounds[groundResetTrans_T]->area.width;
                        TutorialGroundsEdgePos[groundResetTrans_T]->y = TutorialGrounds[groundResetTrans_T]->area.y;
                    }

                    enemy1.status = true;

                    enemy1.platform = 5;
                    enemy1.position.x = platform5.area.x + platform5.area.width/2;
                    enemy1.position.y = platform5.area.y;

                    bullet1.position = {2560, 1600};
                    bullet1.status = false;

                    currentScreen = TUTORIAL;
                }
                frameTrans++;
            }break;
            case TUTORIAL:
            {
                //slowmotion
                if (IsKeyDown(KEY_LEFT_SHIFT) && !playerTutorial.redirecting) {SetTargetFPS(30);}
                else if (!playerTutorial.redirecting) {SetTargetFPS(120);}

                //player
                playerTutorial.position.x += ptDisplacementX;
                playerTutorial.position.y -= ptDisplacementY;

                ptDisplacementX = 0;





                //player movement------------------------------------------------------------------------------

                //move in x direction
                if (IsKeyDown(KEY_D) && !hookingMode_T && !playerTutorial.redirecting)
                {
                    if (playerTutorial.position.x >= 1920 && platform4.area.x >= 1645)
                    {
                        
                        for (int groundMoveR_T = 1; groundMoveR_T <= 5; groundMoveR_T++)
                        {
                            TutorialGrounds[groundMoveR_T]->area.x -= 8;
                        }
                        
                        top_T.area.x -= 8;
                        bottom_T.area.x -= 8;

                        screenOneTextPos -= 8;
                        screenTwoTextPos -= 8;
                    }
                    else  if (playerTutorial.position.x <= 1920) {ptDisplacementX = 8;}
                }
                else if (IsKeyDown(KEY_A) && !hookingMode_T && !playerTutorial.redirecting)
                {
                    if (playerTutorial.position.x <= 320 && platform2.area.x <= 550)
                    {
                        
                        for (int groundMoveL_T = 1; groundMoveL_T <= 5; groundMoveL_T++)
                        {
                            TutorialGrounds[groundMoveL_T]->area.x += 8;
                        }
                        
                        top_T.area.x += 8;
                        bottom_T.area.x += 8;

                        screenOneTextPos += 8;
                        screenTwoTextPos += 8;

                    }
                    else if (playerTutorial.position.x >= 320) {ptDisplacementX = -8;}
                }
                else if (!hookingMode_T && !playerTutorial.redirecting) {ptDisplacementX = 0;}



                //update ground positions
                for (int groundUpdate_T = 1; groundUpdate_T <= 5; groundUpdate_T++)
                {
                    TutorialGroundsPos[groundUpdate_T]->x = TutorialGrounds[groundUpdate_T]->area.x;
                    TutorialGroundsPos[groundUpdate_T]->y = TutorialGrounds[groundUpdate_T]->area.y;
                    TutorialGroundsEdgePos[groundUpdate_T]->x = TutorialGrounds[groundUpdate_T]->area.x + TutorialGrounds[groundUpdate_T]->area.width;
                    TutorialGroundsEdgePos[groundUpdate_T]->y = TutorialGrounds[groundUpdate_T]->area.y;
                }





                //move in y direction
                //checks nearest platform above and below the player
                if (!(CheckCollisionCircleRec(playerTutorial.position, playerTutorial.length, bottom_T.area))) {bottom_T = ground_T;}
                for(int groundCheck_T = 0; groundCheck_T <= 5; groundCheck_T++)
                {   
                    if (!(CheckCollisionCircleRec(playerTutorial.position, playerTutorial.length, TutorialGrounds[groundCheck_T]->area)))
                    {
                        if
                        (
                            (playerTutorial.position.x + playerTutorial.length/2 >= TutorialGrounds[groundCheck_T]->area.x && playerTutorial.position.x + playerTutorial.length/2 <= TutorialGrounds[groundCheck_T]->area.x + TutorialGrounds[groundCheck_T]->area.width) ||
                            (playerTutorial.position.x - playerTutorial.length/2 >= TutorialGrounds[groundCheck_T]->area.x && playerTutorial.position.x - playerTutorial.length/2 <= TutorialGrounds[groundCheck_T]->area.x + TutorialGrounds[groundCheck_T]->area.width)
                        )
                        {   

                            if (playerTutorial.position.y + playerTutorial.length > TutorialGrounds[groundCheck_T]->area.y + TutorialGrounds[groundCheck_T]->area.height && TutorialGrounds[groundCheck_T]->area.y + TutorialGrounds[groundCheck_T]->area.height > top_T.area.y + top_T.area.height)
                            {
                                top_T = *TutorialGrounds[groundCheck_T];
                            }
                            
                            else if (playerTutorial.position.y - playerTutorial.length < TutorialGrounds[groundCheck_T]->area.y && TutorialGrounds[groundCheck_T]->area.y < bottom_T.area.y)
                            {
                                bottom_T = *TutorialGrounds[groundCheck_T];
                            }

                        }
                        if
                        (
                            !(playerTutorial.position.x + playerTutorial.length/2 >= top_T.area.x && playerTutorial.position.x + playerTutorial.length/2 <= top_T.area.x + top_T.area.width) &&
                            !(playerTutorial.position.x - playerTutorial.length/2 >= top_T.area.x && playerTutorial.position.x - playerTutorial.length/2 <= top_T.area.x + top_T.area.width)
                        )
                        {
                            top_T = sky_T;
                        }
                    }
                }

                //reset acceleration in the y direction when not jumping or falling or hooking
                if (!fallState_T && !jumpState_T && !hookingMode_T) {vGravity_T = 0;}


                //triggers jump
                if (IsKeyDown(KEY_W) && !jumpState_T  && !fallState_T && checkPlayerGround(playerTutorial, *TutorialGrounds, 5))
                {
                    jumpState_T = true;
                }

                //jumping
                if (jumpState_T)
                {
                    ptDisplacementY = 20 - vGravity_T;
                    vGravity_T += 1;
                }

                //checks if the player is currently on air or not
                if (!jumpState_T && !fallState_T && !hookingMode_T)
                {
                    if (!checkPlayerGround(playerTutorial, *TutorialGrounds, 5))
                    {vGravity_T = 0; fallState_T = true; goto onAirTutorial;}
                }
                onAirTutorial:;

                //falling
                if (fallState_T)
                {
                    ptDisplacementY = vGravity_T;
                    vGravity_T -= 1;
                }

                //prevent the player to clip through TutorialGrounds
                if (playerTutorial.position.y + playerTutorial.length > bottom_T.area.y && (!hookingMode_T || bottom_T.area.y == ground_T.area.y) && !playerTutorial.redirecting)
                {
                    ptDisplacementY = 0;
                    playerTutorial.position.y = bottom_T.area.y - playerTutorial.length;
                    fallState_T = false;
                    jumpState_T = false;
                }
                if (playerTutorial.position.y - playerTutorial.length < top_T.area.y + top_T.area.height && !hookingMode_T && !playerTutorial.redirecting)
                {
                    ptDisplacementY = 0;
                    playerTutorial.position.y =  top_T.area.y + top_T.area.height + playerTutorial.length;
                    jumpState_T = false;
                    fallState_T = true;
                }


                //hooking movement
                //hook trigger
                float hookSpeedY_T; //THIS PART IS BROKEN BECAUSE OF THE CHANGES IN WHATS HOOKABLE TO THE PLAYER, WILL FIX LATER
                if (!hookingMode_T && !playerTutorial.redirecting)
                {
                    for (grappleCheck_T = 0; grappleCheck_T <= 11; grappleCheck_T++)
                    {
                        float lengthL_T = sqrt(pow(hookables_T[grappleCheck_T]->x - playerTutorial.position.x, 2) + pow(hookables_T[grappleCheck_T]->y - playerTutorial.position.y, 2));
                        if // platforms in the right side of the player
                        (
                            grappleCheck_T <= 4 &&
                            playerTutorial.position.x < hookables_T[grappleCheck_T]->x - 2*playerTutorial.length &&
                            playerTutorial.position.x < GetMousePosition().x && 
                            checkHookable(playerTutorial.position, GetMousePosition(), hookables_T[grappleCheck_T]->x, hookables_T[grappleCheck_T]->y, 0, hookTolerance_T) &&
                            lengthL_T <= 480
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                hookingMode_T = true;
                                hookSpeedY_T = 10*(playerTutorial.position.y - hookables_T[grappleCheck_T]->y + playerTutorial.length)/(hookables_T[grappleCheck_T]->x - playerTutorial.position.x);
                            }
                            hookGuide_T = true;
                            hookGuideDraw_T.x = hookables_T[grappleCheck_T]->x;
                            hookGuideDraw_T.y = hookables_T[grappleCheck_T]->y;
                            goto hookingNowTutorial;
                        }
                        else if // platforms in the left side of the player
                        (
                            grappleCheck_T >= 5 && grappleCheck_T <= 9 &&
                            playerTutorial.position.x > hookables_T[grappleCheck_T]->x - 2*playerTutorial.length &&
                            playerTutorial.position.x > GetMousePosition().x && 
                            checkHookable(playerTutorial.position, GetMousePosition(), hookables_T[grappleCheck_T]->x, hookables_T[grappleCheck_T]->y, 0, hookTolerance_T) &&
                            lengthL_T <= 480
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                hookingMode_T = true;
                                hookSpeedY_T = 10*(playerTutorial.position.y - hookables_T[grappleCheck_T]->y + playerTutorial.length)/(playerTutorial.position.x - hookables_T[grappleCheck_T]->x);
                            }
                            hookGuide_T = true;
                            hookGuideDraw_T.x = hookables_T[grappleCheck_T]->x;
                            hookGuideDraw_T.y = hookables_T[grappleCheck_T]->y;
                            goto hookingNowTutorial;
                        }
                        else if //enemies
                        (
                            grappleCheck_T == 10 &&
                            playerTutorial.position.y <= hookables_T[grappleCheck_T]->y + platform1.area.height + enemy1.length &&
                            checkHookable(playerTutorial.position, GetMousePosition(), hookables_T[grappleCheck_T]->x, hookables_T[grappleCheck_T]->y, enemy1.length, hookTolerance_T/2) &&
                            ((playerTutorial.position.x <= hookables_T[grappleCheck_T]->x - 2*playerTutorial.length && playerTutorial.position.x <= GetMousePosition().x) || (playerTutorial.position.x >= hookables_T[grappleCheck_T]->x + 2*playerTutorial.length && playerTutorial.position.x >= GetMousePosition().x)) &&
                            enemy1.status &&
                            lengthL_T <= 480
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                            {
                                hookingMode_T = true;
                                enemy1.hooked = true;
                                hookSpeedY_T = 10*(hookables_T[grappleCheck_T]->y - playerTutorial.position.y)/(playerTutorial.position.x - hookables_T[grappleCheck_T]->x);
                            }
                            hookGuide_T = true;
                            hookGuideDraw_T.x = hookables_T[grappleCheck_T]->x;
                            hookGuideDraw_T.y = hookables_T[grappleCheck_T]->y;
                            goto hookingNowTutorial;
                        }
                        else if //Bullets
                        (
                            grappleCheck_T == 11 &&
                            checkHookable(playerTutorial.position, GetMousePosition(), hookables_T[grappleCheck_T]->x, hookables_T[grappleCheck_T]->y, hookTolerance_T/2, hookTolerance_T/2) &&
                            (playerTutorial.position.x <= hookables_T[grappleCheck_T]->x - 2*playerTutorial.length || playerTutorial.position.x >= hookables_T[grappleCheck_T]->x + 2*playerTutorial.length) && 
                            bullet1.status &&
                            !bullet1.redirected
                        )
                        {
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !bullet1.hooked)
                            {
                                bullet1.hooked = true;
                                bullet1.bgDisplacement.x = 0;
                                bullet1.bgDisplacement.y = 0;

                                if (playerTutorial.position.x < hookables_T[grappleCheck_T]->x)
                                {
                                    hookingMode_T = true;
                                    hookSpeedY_T = 10*(playerTutorial.position.y - hookables_T[grappleCheck_T]->y)/(hookables_T[grappleCheck_T]->x - playerTutorial.position.x);
                                }
                                else if (playerTutorial.position.x > hookables_T[grappleCheck_T]->x)
                                {
                                    hookingMode_T = true;
                                    hookSpeedY_T = 10*(playerTutorial.position.y - hookables_T[grappleCheck_T]->y)/(playerTutorial.position.x - hookables_T[grappleCheck_T]->x);
                                }
                            }
                            hookGuide_T = true;
                            hookGuideDraw_T.x = hookables_T[grappleCheck_T]->x;
                            hookGuideDraw_T.y = hookables_T[grappleCheck_T]->y;
                            goto hookingNowTutorial;
                        }
                        else {hookGuide_T = false;}
                    }
                    hookingNowTutorial:;
                }

                //hooking mechanism
                if (hookingMode_T)
                {
                    if (hookables_T[hookingGround_T]->x > playerTutorial.position.x)
                    {
                        if (playerTutorial.position.x >= 1280)
                        {

                            for (int hookMoveR_T = 1; hookMoveR_T <= 5; hookMoveR_T++)
                            {
                                TutorialGrounds[hookMoveR_T]->area.x -= 10;
                            }

                            top_T.area.x -= 10;
                            bottom_T.area.x -= 10;
                            hookGuideDraw_T.x -= 10;
                            screenOneTextPos -= 10;
                            screenTwoTextPos -= 10;

                            if (grappleCheck_T == 11)
                            {
                                bullet1.position.x -= 10;
                            }

                            ptDisplacementX = 0;
                            ptDisplacementY = hookSpeedY_T;
                        }
                        else
                        {
                            ptDisplacementX = 10;
                            ptDisplacementY = hookSpeedY_T;
                        }
                    }
                    else if (hookables_T[hookingGround_T]->x < playerTutorial.position.x)
                    {
                        if (playerTutorial.position.x <= 320)
                        {
                            for (int hookMoveL_T = 1; hookMoveL_T <= 5; hookMoveL_T++)
                            {
                                TutorialGrounds[hookMoveL_T]->area.x += 10;
                            }
                            top_T.area.x += 10;
                            bottom_T.area.x += 10;
                            hookGuideDraw_T.x += 10;

                            screenOneTextPos += 10;
                            screenTwoTextPos += 10;

                            if (grappleCheck_T == 11)
                            {
                                bullet1.position.x += 10;
                            }

                            ptDisplacementX = 0;

                        }
                        else
                        {
                            ptDisplacementX = -10;
                        }

                        if (grappleCheck_T == 10)
                        {
                            ptDisplacementY = -hookSpeedY_T;
                        }
                        else
                        {
                            ptDisplacementY = hookSpeedY_T;
                        }
                    }

                    //stops hooking when its done
                    int xRangeMin_T, xRangeMax_T;
                    if (grappleCheck_T <= 4) {xRangeMin_T = grappleCheck_T; xRangeMax_T = grappleCheck_T + 5;}
                    else if (grappleCheck_T >= 5 && grappleCheck_T <= 9) {xRangeMin_T = grappleCheck_T - 5; xRangeMax_T = grappleCheck_T;}
                    
                    if //platforms
                    (
                        grappleCheck_T <= 9 &&
                        playerTutorial.position.y <= hookables_T[grappleCheck_T]->y &&
                        playerTutorial.position.y >= hookables_T[grappleCheck_T]->y - 3*playerTutorial.length &&
                        playerTutorial.position.x <= hookables_T[xRangeMax_T]->x &&
                        playerTutorial.position.x >= hookables_T[xRangeMin_T]->x
                    )
                    {
                        if (grappleCheck <= 4)
                        {
                            playerTutorial.position.x = hookables_T[xRangeMin_T]->x + playerTutorial.length;
                            bottom_T = *TutorialGrounds[grappleCheck_T + 1];
                        }
                        else if (grappleCheck_T >= 5 && grappleCheck_T <= 9)
                        {
                            playerTutorial.position.x = hookables_T[xRangeMax_T]->x - playerTutorial.length;
                            bottom_T = *TutorialGrounds[grappleCheck_T - 4];
                        }

                        playerTutorial.position.y = hookGuideDraw_T.y - playerTutorial.length;
                        ptDisplacementY = 0;
                        hookingMode_T = false; 
                    }
                    else if //enemies
                    (
                        grappleCheck_T == 10 &&
                        playerTutorial.position.y <= hookables_T[grappleCheck_T]->y + enemy1.length &&
                        playerTutorial.position.y >= hookables_T[grappleCheck_T]->y - enemy1.length &&
                        playerTutorial.position.x <= hookables_T[grappleCheck_T]->x + playerTutorial.swordLength &&
                        playerTutorial.position.x >= hookables_T[grappleCheck_T]->x - playerTutorial.swordLength
                    )
                    {
                        enemy1.status = false;
                        enemy1.terminated = true;
                        playerTutorial.position.x = enemy1.position.x;
                        playerTutorial.position.y = enemy1.position.y;
                        ptDisplacementY = 0;
                        hookingMode_T = false;

                        bullet1.position = {2560, 1600};
                        bullet1.status = false;
                        bullet1.hooked = false;
                        bullet1.redirected = false;
                        bullet1.bgDisplacement.x = 0;
                        bullet1.bgDisplacement.y = 0;
                    }
                    else if //bullets
                    (
                        grappleCheck_T == 11 &&
                        playerTutorial.position.y <= hookables_T[grappleCheck_T]->y + playerTutorial.length &&
                        playerTutorial.position.y >= hookables_T[grappleCheck_T]->y - playerTutorial.length &&
                        playerTutorial.position.x <= hookables_T[grappleCheck_T]->x + playerTutorial.length &&
                        playerTutorial.position.x >= hookables_T[grappleCheck_T]->x - playerTutorial.length
                    )
                    {
                        playerTutorial.redirecting = true;
                        ptDisplacementX = 0;
                        ptDisplacementY = 0;
                        SetTargetFPS(15);
                        bullet1.redirecting = true;
                        hookingMode_T = false;
                    }

                }

                //slashing trigger
                if(IsKeyReleased(KEY_SPACE) && !playerTutorial.slash)
                {
                    playerTutorial.slash = true;
                }

                //slashing mechanism
                if (playerTutorial.slash)
                {
                    //define hitbox area
                    swordHitBox_T.height = playerTutorial.swordLength;
                    swordHitBox_T.width = playerTutorial.swordLength;
                    swordHitBox_T.y = playerTutorial.position.y;
                    if (slashFrame_T <= 31.125f)
                    {
                        swordHitBox_T.x = playerTutorial.position.x;
                    }
                    if (slashFrame_T >= 31.125f)
                    {
                        swordHitBox_T.x = playerTutorial.position.x - playerTutorial.swordLength;
                    }

                    //check if the sword hits an enemy
                    //should be a loop for the gameplay, but there's only one enemy in the tutorial
                    if (CheckCollisionCircleRec(enemy1.position, enemy1.length, swordHitBox_T))
                    {
                        enemy1.status = false;
                        enemy1.terminated = true;

                        bullet1.position = {2560, 1600};
                        bullet1.status = false;
                        bullet1.hooked = false;
                        bullet1.redirected = false;
                        bullet1.bgDisplacement.x = 0;
                        bullet1.bgDisplacement.y = 0;
                    }

                    //slashin state control
                    slashFrame_T++;
                    if (slashFrame_T >= 63)
                    {
                        slashFrame_T = 0;
                        playerTutorial.slash = false;
                    }
                }

                //projectile implementation
                //checks if the player is visible from an enemy
                enemy1.aiming = true;
                for(int aimBlock_T = 0; aimBlock_T <= 5; aimBlock_T++)
                {
                    Vector2 collisionPoint_T;
                    if
                    (
                        CheckCollisionLines
                        (
                            enemy1.position,
                            playerTutorial.position,
                            (Vector2){TutorialGrounds[aimBlock_T]->area.x, TutorialGrounds[aimBlock_T]->area.y},
                            (Vector2){TutorialGrounds[aimBlock_T]->area.x + TutorialGrounds[aimBlock_T]->area.width, TutorialGrounds[aimBlock_T]->area.y + TutorialGrounds[aimBlock_T]->area.height},
                            &collisionPoint_T
                        )
                        ||
                        CheckCollisionLines
                        (
                            enemy1.position,
                            playerTutorial.position,
                            (Vector2){TutorialGrounds[aimBlock_T]->area.x + TutorialGrounds[aimBlock_T]->area.width, TutorialGrounds[aimBlock_T]->area.y},
                            (Vector2){TutorialGrounds[aimBlock_T]->area.x, TutorialGrounds[aimBlock_T]->area.y + TutorialGrounds[aimBlock_T]->area.height},
                            &collisionPoint_T
                        )
                    )
                    {
                        enemy1.aiming = false;
                    }
                }

                //randomize whether the enemy should shoot the player or not and also assign velocity to bullets that are shot
                if(enemy1.aiming &&  !bullet1.status)
                {
                    if (GetRandomValue(1, 165) == 1 && enemy1.status && sqrt(pow(playerTutorial.position.x - enemy1.position.x, 2) + pow(playerTutorial.position.y - playerTutorial.position.y, 2)) >= 250)
                    {
                        bullet1.status = true;
                        bullet1.position = enemy1.position;

                        if (playerTutorial.position.x <= bullet1.position.x)
                        {
                            bullet1.bgDisplacement.x = -12;
                            bullet1.bgDisplacement.y = 12*(enemy1.position.y - playerTutorial.position.y)/(enemy1.position.x - playerTutorial.position.x);
                        }
                        else if (playerTutorial.position.x >= bullet1.position.x)
                        {
                            bullet1.bgDisplacement.x = 12;
                            bullet1.bgDisplacement.y = 12*(enemy1.position.y - playerTutorial.position.y)/(playerTutorial.position.x - enemy1.position.x);
                        }
                    }
                }            

                //moving the bullet and collision check
                if (bullet1.status && !bullet1.hooked && enemy1.status)
                {
                    bullet1.position.x += bullet1.bgDisplacement.x;
                    bullet1.position.y -= bullet1.bgDisplacement.y;

                    //reset the bullet when it goes out of the screen
                    if
                    (
                        bullet1.position.x > 2560 ||
                        bullet1.position.x < 0 ||
                        bullet1.position.y < 0 ||
                        bullet1.position.y > 1600
                    )
                    {
                        bullet1.bgDisplacement.x = 0;
                        bullet1.bgDisplacement.y = 0;

                        bullet1.position = {2560, 1600};
                        bullet1.status = false;
                        bullet1.hooked = false;
                        bullet1.redirected = false;
                    }

                    //collision check of bullet with the player
                    if (CheckCollisionCircles(bullet1.position, 10, playerTutorial.position, playerTutorial.length) && !bullet1.redirected && !bullet1.redirecting)
                    {
                        bullet1.bgDisplacement.x = 0;
                        bullet1.bgDisplacement.y = 0;

                        bullet1.position = {2560, 1600};
                        bullet1.status = false;
                        bullet1.hooked = false;
                        bullet1.redirected = false;
                        
                        playerTutorial.position = {360, 1200};
                        goto skipChecking_T;
                    }

                    //collision check of bullet with platforms
                    for (int checkCollisionBulletPlatforms_T = 0; checkCollisionBulletPlatforms_T <= 5; checkCollisionBulletPlatforms_T++)
                    {
                        if (CheckCollisionCircleRec(bullet1.position, 10, TutorialGrounds[checkCollisionBulletPlatforms_T]->area))
                        {
                            bullet1.bgDisplacement.x = 0;
                            bullet1.bgDisplacement.y = 0;

                            bullet1.position = {2560, 1600};
                            bullet1.status = false;
                            bullet1.hooked = false;
                            bullet1.redirected = false;
                            goto skipChecking_T;
                        }
                    }

                    //collision check of the bullet with the enemy
                    if(CheckCollisionCircles(bullet1.position, 10, enemy1.position, enemy1.length) && enemy1.status && bullet1.redirected)
                    {
                        enemy1.status = false;
                        enemy1.terminated = true;

                        bullet1.bgDisplacement.x = 0;
                        bullet1.bgDisplacement.y = 0;

                        bullet1.position = {2560, 1600};
                        bullet1.status = false;
                        bullet1.hooked = false;
                        bullet1.redirected = false;

                        goto skipChecking_T;
                    }
                    skipChecking_T:;

                }

                //redirecting projectile
                if (playerTutorial.redirecting)
                {
                    playerTutorial.position.x = bullet1.position.x;
                    playerTutorial.position.y = bullet1.position.y;

                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        if (GetMousePosition().x <= bullet1.position.x)
                        {
                            bullet1.bgDisplacement.x = -12;
                            bullet1.bgDisplacement.y = 12*(playerTutorial.position.y - GetMousePosition().y)/(playerTutorial.position.x - GetMousePosition().x);
                        }
                        else if (GetMousePosition().x >= bullet1.position.x)
                        {
                            bullet1.bgDisplacement.x = 12;
                            bullet1.bgDisplacement.y = 12*(playerTutorial.position.y - GetMousePosition().y)/(GetMousePosition().x - playerTutorial.position.x);
                        }

                        bullet1.hooked = false;
                        bullet1.redirecting = false;
                        bullet1.redirected = true;

                        playerTutorial.redirecting = false;
                    }
                }

                //reset enemy button
                resetEnemyTButton.area = {platform4.area.x + platform4.area.width/2 - platform4.area.width*3/8 , platform4.area.y + platform4.area.height + 25, platform4.area.width*3/4, platform4.area.height * 2};
                resetEnemyTButton.textSize = 35; 
                resetEnemyTButton.outlineThickness = 15; 

                if (CheckCollisionPointRec(GetMousePosition(), resetEnemyTButton.area))
                {
                    resetEnemyTButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        enemy1.status = true;
                        enemy1.terminated = false;
                    }
                }
                else {resetEnemyTButton.buttonColor = WHITE;}

                //back button
                backButton.area = {2495 -  platform4.area.width*3/4, 65, platform4.area.width*3/4, platform4.area.height * 2};
                backButton.textSize = 35; 
                backButton.outlineThickness = 15; 

                if (CheckCollisionPointRec(GetMousePosition(), backButton.area))
                {
                    backButton.buttonColor = GRAY;
                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                    {
                        currentScreen = TITLE;
                    }
                }
                else {backButton.buttonColor = WHITE;}

            }break;
        }





        //------------------
        //front end systems
        //------------------
        BeginDrawing();
    
            switch(currentScreen)
            {
                case TITLE:
                {
                    ClearBackground(GOLD);
                    mainButton.draw("RANON!");
                    settingsButton.draw("Settings");
                    tutorialButton.draw("Tutorial");
                    quitButton.draw("Quit");

                    DrawText(TextFormat("Hi: %i", highScore), 1280 - MeasureText(TextFormat("Hi: %i", highScore), 30)/2, 850, 30, BLACK);
                } break;
                case T_TitleGameplay:
                {
                    ClearBackground(GOLD);
                    mainButton.area.x -= widdenMain;
                    mainButton.area.y -= widdenMain;
                    mainButton.area.width += 2*widdenMain;
                    mainButton.area.height += 2*widdenMain;
                    DrawRectangleRounded(mainButton.area, 0.4, 0, GRAY);
                    widdenMain += 40;
                }break;
                case GAMEPLAY:
                {
                    if (IsKeyDown(KEY_LEFT_SHIFT) && energy > 0) {ClearBackground(GOLD);}
                    else {ClearBackground(WHITE);}

                    ground.draw();
                    platformG1.draw();
                    platformG2.draw();
                    platformG3.draw();
                    platformG4.draw();
                    platformG5.draw();
                    platformG6.draw();
                    platformG7.draw();
                    platformG8.draw();
                    platformG9.draw();

                    enemyG1.draw(playerMain.position.x, playerMain.position.y, gameplayGrounds[enemyG1.platform]->area.x, gameplayGrounds[enemyG1.platform]->area.y, gameplayGrounds[enemyG1.platform]->area.width);
                    enemyG2.draw(playerMain.position.x, playerMain.position.y, gameplayGrounds[enemyG2.platform]->area.x, gameplayGrounds[enemyG2.platform]->area.y, gameplayGrounds[enemyG2.platform]->area.width);
                    enemyG3.draw(playerMain.position.x, playerMain.position.y, gameplayGrounds[enemyG3.platform]->area.x, gameplayGrounds[enemyG3.platform]->area.y, gameplayGrounds[enemyG3.platform]->area.width);
                    enemyG4.draw(playerMain.position.x, playerMain.position.y, gameplayGrounds[enemyG4.platform]->area.x, gameplayGrounds[enemyG4.platform]->area.y, gameplayGrounds[enemyG4.platform]->area.width);
                    enemyG5.draw(playerMain.position.x, playerMain.position.y, gameplayGrounds[enemyG5.platform]->area.x, gameplayGrounds[enemyG5.platform]->area.y, gameplayGrounds[enemyG5.platform]->area.width);

                    playerMain.draw(slashFrame);

                    for (int bulletDraw = 0; bulletDraw <= 4; bulletDraw++)
                    {
                        if (gameplayEnemies[bulletDraw]->status) {gameplayBullets[bulletDraw]->draw();}
                    }

                    //ui level
                    DrawRectangle(0, 0, energy, 10, RED); //energy bar

                    if (gameFeel) //score
                    {
                        DrawText(TextFormat("%i", score), 1280 - MeasureText(TextFormat("%i", score), 160)/2, 10, 160, BLACK);
                    }
                    else{
                        DrawText(TextFormat("%i", score), 1280 - MeasureText(TextFormat("%i", score), 80)/2, 10, 80, BLACK);
                    }

                    if (hookGuide) //hooking guide
                    {   
                        if (grappleCheck <= 8) {
                            DrawCircle(hookGuideDraw.x, hookGuideDraw.y, hookingGuidePlatLength, Fade(RED, 0.5));
                        }
                        else if (grappleCheck >= 9 && grappleCheck <= 17)
                        {
                            DrawCircle(hookGuideDraw.x, hookGuideDraw.y, hookingGuidePlatLength, Fade(RED, 0.5));
                        }
                        else if (grappleCheck >= 18 && grappleCheck <= 22)
                        {
                            DrawRing(hookGuideDraw, gameplayEnemies[grappleCheck-18]->length, gameplayEnemies[grappleCheck-18]->length + hookingGuideGroundEnemLength, 0, 360, 100, RED);
                        }
                        else if (grappleCheck >= 23 && grappleCheck <= 27)
                        {
                            DrawCircleV(hookGuideDraw, hookingGuideBulletLength, Fade(RED, 0.5f));
                        }
                    }

                    if (hookingMode)//hooking rope
                    {
                        DrawLine(playerMain.position.x, playerMain.position.y, hookGuideDraw.x, hookGuideDraw.y, RED);
                    }

                    if (playerMain.redirecting) //letting the player aim
                    {
                        DrawLineV(playerMain.position, GetMousePosition(), BLUE);
                    }
                } break;
                case DEATHSCREEN:
                {
                    ClearBackground(GOLD);
                    DrawText(TextFormat("%i", score), 1280 - MeasureText(TextFormat("%i", score), 300)/2, 650, 300, BLACK);

                    mainMenuButton.draw("Main Menu");
                    retryButton.draw("Retry");

                    if (newHighScore)
                    {
                        DrawText("New High Score!!!", 1280 - MeasureText("New High Score!!!", 30)/2, 1000, 30, BLACK);
                    }
                    else{
                        DrawText(TextFormat("High Score: %i", highScore), 1280 - MeasureText(TextFormat("High Score : %i", highScore), 30)/2, 1000, 30, BLACK);
                    }
                }break;
                case T_TitleSettings:
                {
                    ClearBackground(GOLD);
                    settingsButton.area.x -= widdenSetting;
                    settingsButton.area.y -= widdenSetting;
                    settingsButton.area.width += 2*widdenSetting;
                    settingsButton.area.height += 2*widdenSetting;
                    DrawRectangleRounded(settingsButton.area, 0.4, 0, GRAY);
                    widdenSetting += 40;
                }break;
                case SETTINGS:
                {
                    ClearBackground(GOLD);

                    fullScreenCheck.draw("Fullscreen", &fullScreen);

                    introCheck.draw("Intro before the game starts", &intro);
                    
                    hookGuidePlatLen.draw("Hooking Grounds Guide");

                    hookGuideGrEnemLen.draw("Hooking Enemies Guide");

                    hookGuideBulletLen.draw("Hooking Bullets Guide");

                    resetHighScoreButton.draw("Reset Highscore");
                    if (highScore == 0) {DrawText("DONE!", resetHighScoreButton.area.x + resetHighScoreButton.area.width + 40.0f, resetHighScoreButton.area.y + 20, 40, BLACK);}

                    saveAndGoBackButton.draw("Save and go back");
                }break;
                case T_TitleTutorial:
                {
                    ClearBackground(GOLD);
                    tutorialButton.area.x -= widdenTutorial;
                    tutorialButton.area.y -= widdenTutorial;
                    tutorialButton.area.width += 2*widdenTutorial;
                    tutorialButton.area.height += 2*widdenTutorial;
                    DrawRectangleRounded(tutorialButton.area, 0.4, 0, GRAY);
                    widdenTutorial += 40;
                }break;
                case TUTORIAL:
                {
                    if (IsKeyDown(KEY_LEFT_SHIFT)) {ClearBackground(GOLD);}
                    else {ClearBackground(WHITE);}

                    //texts....................
                    DrawText("use 'A', 'D', and 'W' to move", screenOneTextPos, 100, 35, GRAY);
                    DrawText("'left click' to hook to platforms", screenOneTextPos, 145, 35, GRAY);
                    DrawText("press 'shift' to go to slowmotion mode", screenOneTextPos, 190, 35, GRAY);
                    DrawText("hooking and slowmotion mode drains energy", screenOneTextPos, 235, 35, GRAY);

                    DrawText("This is an enemy", screenTwoTextPos, 100, 35, GRAY);
                    DrawText("you can gain score by slashing, hooking,", screenTwoTextPos, 145, 35, GRAY);
                    DrawText("or redirecting their own bullet back to them", screenTwoTextPos, 190, 35, GRAY);
                    DrawText("press 'space' to slash enemies", screenTwoTextPos, 235, 35, GRAY);
                    DrawText("hook to projectiles to redirect them", screenTwoTextPos, 280, 35, GRAY);

                    resetEnemyTButton.draw("Reset Enemy");
                    backButton.draw("Back");


                    //platforms
                    ground_T.draw();
                    platform1.draw();
                    platform2.draw();
                    platform3.draw();
                    platform4.draw();
                    platform5.draw();

                    //enemy
                    enemy1.draw(playerTutorial.position.x, playerTutorial.position.y, platform4.area.x, platform4.area.y, platform4.area.width);

                    //player
                    playerTutorial.draw(slashFrame_T);

                    //projectile
                    if (bullet1.status) {bullet1.draw();}

                    //ui level
                    if (hookGuide_T) //hooking guide
                    {
                        if (grappleCheck_T <= 9)
                        {
                            hookingGround_T = grappleCheck_T;
                            DrawCircle(hookGuideDraw_T.x, hookGuideDraw_T.y, hookingGuidePlatLength, Fade(RED, 0.5));
                        }
                        else if (grappleCheck_T == 10)
                        {
                            hookingGround_T = grappleCheck_T;
                            DrawRing(hookGuideDraw_T, enemy1.length, enemy1.length + hookingGuideGroundEnemLength, 0, 360, 100, RED);
                        }
                        else if (grappleCheck_T == 11)
                        {
                            hookingGround_T = grappleCheck_T;
                            DrawCircle(hookGuideDraw_T.x, hookGuideDraw_T.y, hookingGuideBulletLength, Fade(RED, 0.5));
                        }

                    }
                    if (hookingMode_T)//hooking rope
                    {
                        DrawLine(playerTutorial.position.x, playerTutorial.position.y, hookGuideDraw_T.x, hookGuideDraw_T.y, RED);
                    }

                    if (playerTutorial.redirecting) //letting the player aim
                    {
                        DrawLineV(playerTutorial.position, GetMousePosition(), BLUE);
                    }
                }break;
            }

        EndDrawing();
        frameCounter++;
    }    

    CloseWindow();
    return 0;
}

//Farrel Ganendra, 25 - 11 - 2023.
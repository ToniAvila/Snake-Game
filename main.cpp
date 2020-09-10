/*
HW1SnakeGameExtensions

Toni Avila (TXA180025), CS 1337.010
1/30/2020

When executed, this program runs a typical arcade snake game in which you
eat fruits to grow, and lose by eating your own tail. I have made some
minor fixes to the code's readability and modularity, as well as adding
some new features.

Change/Improvement Log:
-Eliminated a redundancy in code in Draw() when drawing top/bottom rows of board. (1/23/2020)
-Renamed majority of variables for greater understandability. (1/23/2020)
-Introduced snakeMovement(), checkSnakeBoundaries(), checkSnakeEatTail(), incrementScore()
printTailOrSpace(int j, int i), drawTopAndBottomRow(), displayResults(), executeGame(),
fruitPlacement(), gameInstructions(), hardOption(), quitGame(), snakeWrapAround().  (1/24/2020-1/30/2020)
-Created Char variables for drawing the game. (1/26/2020)
-Simplified the logic of tail pieces following the head (Logic()) for greater understandability. (1/27/2020)
-Added pause option to game when user presses 'p'. (1/27/2020)
-Added a game instruction screen on user's first playthrough. (1/28/2020)
-Added option to end game on the snake hitting the wall rather than wrapping around. (hard mode) (1/28/2020)
-Added option to play another game when player loses. (1/29/2020)
-Added presence of 5 fruits at a time rather than one. (Can be changed by altering single variable in code) (1/31/2020)
-Fixed bug where snake could disappear into wall. (1/31/2020)
-Pressing X quits game and gives user their results as is while ending the program. (2/1/2020)
-Added a game tracker which tells the user how many games they've played. (2/1/2020)


Notes:
-Snake grows to 100 units.
-Fruit is represented by + (changed from "F")
-Game is played primarily with WASD keys. R, P, X keys also have functions.
-Collision detection is automatically set to off, can be changed in-game.
-To change number of fruits, simply change the const int numFruits object.

For reference:
https://youtu.be/E_-lMZDi7Uw ---> Videos explaining the making of the snake game
https://youtu.be/W1e5wO7XR2w
https://youtu.be/PSoLD9mVXTA

*/

#include <iostream> // cout and cin
#include <conio.h> // for _kbhit _getch
#include <windows.h> // for sleep and time
#include <ctime> // for time -> seeds rand

using namespace std;


bool gameOver; // flag for ending game
bool hardMode; // flag for collisions
bool fruitPrint; // flag to print fruit

// Dimensions of board
const int boardWidth = 20;
const int boardHeight = 20;

//srand(time(0));
const int numFruits = 5;
int snakeHeadX, snakeHeadY, score, fruitsEaten;
int fruitX[numFruits], fruitY[numFruits];

char boardEdge = '#', space = ' ', fruitSymbol = '+', // Chars for draw function
     tailPiece = 'o', snakeHead = 'O';

// Used to read in user's choice
char playAgain;
char startGame;

const int maxTailLength = 100;
int snakeTailX[maxTailLength], snakeTailY[maxTailLength]; // Max size is 100 units
int tailLength;
int gamesPlayed = 0;

enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirecton direction;

void fruitPlacement()
{
    srand(time(0)); // seeding random num generator

    for (int i = 0; i < numFruits; i++)
    {
        fruitX[i] = rand() % boardWidth; // Random vals assigned to fruit coords
        fruitY[i] = rand() % boardHeight;
    }
}

void Setup()
{
    gamesPlayed++;
    tailLength = 0; // resetting length, clearing board
    system("cls");

    gameOver = false; // flag
    hardMode = false; // flag
    direction = STOP; // stop movement
    snakeHeadX = boardWidth / 2; // Centering snake head
    snakeHeadY = boardHeight / 2;

    fruitPlacement();
    fruitsEaten = 0;
    score = 0;
} // Setup()

void displayResults()
{

    cout << "Your score was " << score << "." << endl;
    cout << "You ate " << fruitsEaten << " fruits." << endl;
    cout << "You played " << gamesPlayed << " game(s)." << endl;
} // displayResults()

void drawTopAndBottomRow()
{
    for (int i = 0; i < boardWidth + 2; i++)
        cout << "#";

    cout << endl;
} // drawTopAndBottomRow()

void printTailOrSpace(int j, int i)
{
    // Prints out a tail piece or space as needed
    bool print = false;
    for (int k = 0; k < tailLength; k++)
    {
        if (snakeTailX[k] == j && snakeTailY[k] == i)
        {
            cout << tailPiece;
            print = true;
        }
    }

    if (!print)
        cout << space;
} // printTailOrSpace()

void checkFruitPrint(int boardX, int boardY)
{
    fruitPrint = false;
    for (int i = 0; i < numFruits; i ++)
    {
        if (boardX == fruitX[i] && boardY == fruitY[i])
            fruitPrint = true;
    }
} // checkFruitPrint()

void Draw()
{
    system("cls"); //system("clear");

    drawTopAndBottomRow();

    for (int boardY = 0; boardY < boardHeight; boardY++)
    {
        for (int boardX = 0; boardX < boardWidth; boardX++)
        {
            checkFruitPrint(boardX, boardY);

            if (boardX == 0) // draws left border of board
                cout << boardEdge;
            // if, else if, else routine draws the majority of board
            if (boardY == snakeHeadY && boardX == snakeHeadX) // Drawing snake head
                cout << snakeHead;
            else if (fruitPrint)
                cout << fruitSymbol;
            else
            {
                printTailOrSpace(boardX, boardY); // Passed boardX and boardY from for loops above for easy reference
            }

            if (boardX == boardWidth - 1) // Draws right border of board
                cout << boardEdge;
        }

        cout << endl;

    }

    drawTopAndBottomRow();

    cout << "Score: " << score << endl;
    cout << "Fruits Eaten: " << fruitsEaten << endl;

    if (hardMode) // Prints out respective message depending on user
        cout << "Collision detection is on." << endl;
    else
        cout << "Collision detection is off." << endl;
} // Draw()

void quitGame()
{
    system("cls");
    cout << "You are now exiting the game." << endl;
    displayResults();
    exit(EXIT_SUCCESS); // exits game
} //quitGame()

void hardOption()
{
    // if user presses option for collision on again, it will
    // alternate, meaning if on, will go off and vice versa
    if (hardMode)
        hardMode = false;
    else
        hardMode = true;
} // hardOption()

void Input()
{
    // If a, d, w, s, or x are hit, read & store
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'A':
        case 'a':
            direction = LEFT;
            break;
        case 'D':
        case 'd':
            direction = RIGHT;
            break;
        case 'W':
        case 'w':
            direction = UP;
            break;
        case 'S':
        case 's':
            direction = DOWN;
            break;
        case 'X':
        case 'x':
            quitGame();
        case 'P':
        case 'p':
            system("pause");
            break;
        case 'H':
        case 'h':
            hardOption();
            break;
        }
    }
} // Input()

void snakeMovement()
{
    // Actually moves snake head using
    // direction stored in Input()

    switch (direction)
    {
    case LEFT:
        snakeHeadX--;
        break;
    case RIGHT:
        snakeHeadX++;
        break;
    case UP:
        snakeHeadY--;
        break;
    case DOWN:
        snakeHeadY++;
        break;
    default:
        break;
    }

} // snakeMovement()

void snakeWrapAround()
{
    // Snake head X coord wrap
    if (snakeHeadX >= boardWidth)
        snakeHeadX = 0;
    else if (snakeHeadX < 0)
        snakeHeadX = boardWidth - 1;

    // Snake head y coord wrap
    if (snakeHeadY >= boardHeight)
        snakeHeadY = 0;
    else if (snakeHeadY < 0)
        snakeHeadY = boardHeight - 1;

} // snakeWrapAround()

void checkSnakeBoundaries()
{

    if (hardMode)
    {
        // Checking x bounds
        if (snakeHeadX >= boardWidth || snakeHeadX < 0)
            gameOver = true;

        // Checking y bounds
        if (snakeHeadY >= boardHeight || snakeHeadY < 0)
            gameOver = true;
    }
    else
    {
        snakeWrapAround();
    }

} // checkSnakeBoundaries()

void checkSnakeEatTail()
{
    for (int i = 0; i < tailLength; i++)
        if (snakeTailX[i] == snakeHeadX && snakeTailY[i] == snakeHeadY)
            gameOver = true;
} // checkSnakeEatTail()

void incrementScore()
{
    for (int i = 0; i < numFruits; i++)
    {
        if (snakeHeadX == fruitX[i] && snakeHeadY == fruitY[i]) // If head eats fruit
        {
            score += 10; // increment score, fruits eaten, place the fruit eaten again, and increment tail length
            fruitsEaten += 1;
            fruitX[i] = rand() % boardWidth;
            fruitY[i] = rand() % boardHeight;
            tailLength++;
        }
    }
} // incrementScore()

void Logic()
{

    // Loops assign coords so tail follows previous
    // (first tail bit will follow head)
    for (int i = tailLength; i > 0; i--)
    {
        snakeTailX[i] = snakeTailX[i - 1]; // assigning previous tail piece x, y values
        snakeTailY[i] = snakeTailY[i - 1];
    }
    snakeTailX[0] = snakeHeadX; // If placed before loop, draw bug will occur
    snakeTailY[0] = snakeHeadY;

    snakeMovement();
    checkSnakeBoundaries();
    checkSnakeEatTail();
    incrementScore(); // score incremented when fruit eaten

} // Logic()

void gameInstructions()
{
    cout << "Welcome to the snake game. The controls are: " << endl
         << ">W for up" << endl
         << ">S for down." << endl
         << ">A for left." << endl
         << ">D for right." << endl
         << ">P to pause." << endl
         << ">X to quit." << endl
         << ">H to turn hard-mode (collisions) on." << endl;

    cout << "\nPress y to start." << endl;
    cin >> startGame;

    while (startGame != 'y' && startGame != 'Y') // error message will loop until user enters y
    {
        cout << "You did not press y to start." << endl;
        cout << "Please press y to start." << endl;
        cin >> startGame;
    }

} // gameInstructions();

void executeGame()
{
    Setup();

    while (!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(10); //sleep(10);
    }

    system("cls");
    cout << "You lost." << endl;

    displayResults();

    cout << "Play again? Press R, otherwise press any other key." << endl; // Option to play again
    cin >> playAgain;

} // executeGame()

int main()
{

    gameInstructions(); // only displayed once

    do // Game is executed once for sure
    {
        executeGame();
    }
    while (playAgain == 'r' || playAgain == 'R'); // Loops if user wants to play again

    return 0;
}

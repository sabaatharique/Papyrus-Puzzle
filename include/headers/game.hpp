#ifndef GAME_H
#define GAME_H

#define WIDTH 810
#define HEIGHT 540

#include <iostream>
#include <vector>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Tiles
{
public:
    int xco, yco, size;
    int colour;
    bool monster;

    void load(int i, int j, int r);
};

class Player
{
public:
    float xco, yco, move;
    int width, height, frame, facing;
    bool orange, canMove, isMoving, inPuzzle;
    unsigned int time;
    // facing: right = 0, left = 1, up = 2, down = 3;

    void process();

    void animate();
};

class Game
{
public:
    Tiles tiles[48];
    Player frisk;
    bool gen, miniGame;
    vector<int> impassableX;
    vector<int> impassableY;

    // const char clr[7][10] = {"red", "yellow", "green", "orange", "blue", "purple", "pink"};
    const int hexCodes[7][3] = {{255, 25, 0}, {255, 233, 0}, {0, 194, 0}, {255, 140, 40}, {0, 16, 255}, {134, 19, 128}, {240, 90, 100}};

    void generateTiles();

    void initialise();

    void reset(SDL_Event *event);

    bool isImpassable();

    bool isOnBridge();

    bool isInPuzzle();

    bool tileStepped(int i);

    bool yellowTileAround(int i);

    bool tileEffect(int i);

    void reflectBack();

    void slideAcross();

    bool process(SDL_Event *event);

    void renderPuzzle();

    void renderGame();

    bool quitCheck(SDL_Event *event);

    void playGame();

    int playMiniGame();

    void end();
};

#endif
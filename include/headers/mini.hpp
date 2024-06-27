#ifndef MINI_H
#define MINI_H

#define WIDTH 810
#define HEIGHT 540

#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Target
{
public:
    float xco, yco, move;
    int height, width;
    bool right;

    void moving();
};

class Monster
{
public:
    float xco, yco;
    int height, width, frame;
};

class Mini
{
public:
    Target bar;
    Monster froggit;
    bool win;
    int time;

    void initialise();

    void renderMiniGame();

    void process(SDL_Event *event);
};

#endif
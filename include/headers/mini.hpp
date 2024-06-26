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

class Mini
{
public:
    Target bar;
    bool win;

    void initialise();

    void renderMiniGame();
};

#endif
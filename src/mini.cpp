#include "mini.hpp"

void Target::moving()
{
    if (xco + width >= WIDTH - 80)
        right = false;

    if (xco <= 80)
        right = true;

    if (right)
        xco += move;
    else
        xco -= move;
}

void Mini::initialise()
{
    bar.height = 186;
    bar.width = 33;

    bar.xco = 0;
    bar.yco = HEIGHT - 241;
    bar.move = 3;

    bar.right = true;

    froggit.width = 95;
    froggit.height = 95;
    froggit.xco = (WIDTH / 2) - froggit.width;
    froggit.yco = 304 - froggit.height;
    froggit.frame = 0;

    time = 0;

    win = false;
}

void Mini::process(SDL_Event *event)
{
    time++;

    if (!(time % 75))
    {
        if (froggit.frame == 0)
            froggit.frame = 1;
        else
            froggit.frame = 0;
    }
}

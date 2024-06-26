#include "mini.hpp"

void Target::moving()
{
    if (xco + width >= WIDTH)
        right = false;

    if (xco <= 0)
        right = true;

    if (right)
        xco += move;
    else
        xco -= move;
}

void Mini::initialise()
{
    bar.height = 209;
    bar.width = 38;

    bar.xco = 0;
    bar.yco = HEIGHT - bar.height;
    bar.move = 3;

    bar.right = true;

    win = false;
}

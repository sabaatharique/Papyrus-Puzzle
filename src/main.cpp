#include <iostream>
#include <time.h>
using namespace std;

#include "game.hpp"

Game game;

// main
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    game.initialise();

    game.playGame();

    game.end();

    SDL_Quit();
    return 0;
}

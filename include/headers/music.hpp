#ifndef MUSIC_H
#define MUSIC_H

#include <iostream>
#include <string>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Music
{
public:
    Mix_Music *Bonetrousle;
    Mix_Chunk *EnemyApproaching;

    void initialise();

    void destroy();
};

#endif
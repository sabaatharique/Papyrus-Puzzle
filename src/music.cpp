#include "music.hpp"

void Music::initialise()
{
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    Bonetrousle = Mix_LoadMUS("music/Undertale OST - Bonetrousle.mp3");
    if (Bonetrousle == NULL)
    {
        printf("Error: could not load 'Bonetrousle.mp3'.\n%s\n", Mix_GetError());
        SDL_Quit();
        exit(1);
    }

    EnemyApproaching = Mix_LoadWAV("music/Undertale OST - Enemy Approaching.wav");
    if (EnemyApproaching == NULL)
    {
        printf("Error: could not load 'Enemy Approaching.mp3'.\n%s\n", Mix_GetError());
        SDL_Quit();
        exit(1);
    }
}

void Music::destroy()
{
    Mix_FreeMusic(Bonetrousle);
    Mix_FreeChunk(EnemyApproaching);

    Mix_Quit();
}
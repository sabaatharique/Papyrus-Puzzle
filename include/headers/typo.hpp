#ifndef TYPO_H
#define TYPO_H

#include <iostream>
#include <string>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Typography
{
public:
    SDL_Texture *text1, *text2;
    TTF_Font *DTM, *Papyrus;

    const char res[5][35] = {"For this time...",
                             "You were electrocuted!",
                             "You couldn't beat the monster!",
                             "You got shocked in water!",
                             "You were eaten by piranhas!"};

    void initialise();

    void renderText(int x, int y, float size, string str, SDL_Color colour, TTF_Font *font, SDL_Texture *texture);

    void refresh();

    void destroy();
};

#endif
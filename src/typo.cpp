#include "typo.hpp"

void Typography::initialise()
{
    TTF_Init();

    DTM = TTF_OpenFont("fonts/DTM-Mono.otf", 60);
    Papyrus = TTF_OpenFont("fonts/Pixelated-Papyrus.ttf", 20);
}

void Typography::refresh()
{
    SDL_DestroyTexture(text1);
    SDL_DestroyTexture(text2);
}

void Typography::destroy()
{
    TTF_CloseFont(DTM);
    TTF_CloseFont(Papyrus);
    TTF_Quit();
}
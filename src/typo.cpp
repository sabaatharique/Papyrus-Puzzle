#include "typo.hpp"

void Typography::initialise()
{
    TTF_Init();

    DTM = TTF_OpenFont("fonts/DTM-Mono.otf", 60);
    Papyrus = TTF_OpenFont("fonts/Pixelated-Papyrus.ttf", 60);
}

void Typography::refresh()
{
    SDL_DestroyTexture(text1);
    SDL_DestroyTexture(text2);
    SDL_DestroyTexture(text3);
}

void Typography::destroy()
{
    SDL_DestroyTexture(text1);
    SDL_DestroyTexture(text2);
    SDL_DestroyTexture(text3);
    TTF_CloseFont(DTM);
    TTF_CloseFont(Papyrus);
    TTF_Quit();
}
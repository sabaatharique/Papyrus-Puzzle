#include <iostream>
#include <time.h>
using namespace std;

#include "game.hpp"

Game game;

/*class Typography
{
    SDL_Texture *text;
    TTF_Font *font;

public:
    SDL_Texture *renderText(SDL_Colour colour, const char *str)
    {
        SDL_Surface *surface = TTF_RenderText_Solid(font, str, colour);
        if (surface == NULL)
        {
            printf("Error: could not create '%s' text surface.\n", str);
            SDL_Quit();
            exit(1);
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(gfx.renderer, surface);
        free(surface);
        return texture;
    }

    void destroy()
    {
        SDL_DestroyTexture(text);
    }
};

Typography txt;*/

// main
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    srand(time(0));

    game.playGame();

    SDL_Quit();
    return 0;
}

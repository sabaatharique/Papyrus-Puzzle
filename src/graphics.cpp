#include "graphics.hpp"

SDL_Texture *Graphics::loadTexture(string file)
{
    SDL_Texture *texture = IMG_LoadTexture(renderer, &file[0]);
    if (texture == NULL)
    {
        printf("Could not create %s texture.\nSDL Error: %s\n", file, SDL_GetError());
        SDL_Quit();
        exit(1);
    }
    return texture;
}

void Graphics::initialise()
{
    window = SDL_CreateWindow("✧ Papyrus' Puzzle ✧",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH, HEIGHT,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    background = loadTexture("graphics/background.png");
    target = loadTexture("graphics/target.png");
    bar = loadTexture("graphics/bar.png");

    frisk[0] = loadTexture("graphics/frisk1.png");
    frisk[1] = loadTexture("graphics/frisk2.png");
    frisk[2] = loadTexture("graphics/frisk3.png");
    frisk[3] = loadTexture("graphics/frisk4.png");
    frisk[4] = loadTexture("graphics/frisk5.png");
    frisk[5] = loadTexture("graphics/frisk6.png");
    frisk[6] = loadTexture("graphics/frisk7.png");
    frisk[7] = loadTexture("graphics/frisk8.png");
    frisk[8] = loadTexture("graphics/frisk9.png");
    frisk[9] = loadTexture("graphics/frisk10.png");
    frisk[10] = loadTexture("graphics/frisk11.png");
    frisk[11] = loadTexture("graphics/frisk12.png");
}

void Graphics::renderImage(int x, int y, int w, int h, SDL_Texture *texture)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void Graphics::destroy()
{
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(target);
    SDL_DestroyTexture(bar);
    for (int i = 0; i < 12; i++)
        SDL_DestroyTexture(frisk[i]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
}
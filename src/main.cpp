#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

const int WIDTH = 720, HEIGHT = 540;

// red, yellow, green, orange, blue, purple, pink
const int hexCodes[7][3] = {{255, 25, 0}, {255, 233, 0}, {0, 194, 0}, {255, 140, 40}, {0, 16, 255}, {134, 19, 128}, {240, 90, 100}};

class Tiles
{
public:
    int xco, yco, size, colour;
    // bool electricity, piranhas, monster, slide;

    void loadTiles(int i, int j, int r)
    {
        size = 90;
        colour = r;
        yco = i * size;
        xco = j * size;
    }
};

// game
bool quitCheck(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_QUIT:
        return true;
        break;
    case SDL_KEYDOWN:
        if (event->key.keysym.sym == SDLK_ESCAPE)
        {
            return true;
            break;
        }
    }

    return false;
}

void generateTiles(Tiles tiles[])
{
    int r;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            r = (rand()) % 7;
            tiles[(i * 8) + j].loadTiles(i, j, r);
        }
    }
}

void renderPuzzle(SDL_Renderer *renderer, Tiles tiles[])
{
    for (int i = 0; i < 48; i++)
    {
        SDL_SetRenderDrawColor(renderer, hexCodes[tiles[i].colour][0], hexCodes[tiles[i].colour][1], hexCodes[tiles[i].colour][2], 255);
        SDL_Rect rect = {tiles[i].xco, tiles[i].yco, tiles[i].size, tiles[i].size};
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);
}

void endGame(SDL_Renderer *renderer, SDL_Window *window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// main
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window *window = SDL_CreateWindow("✧ Papyrus' Puzzle ✧", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Tiles tiles[48];
    generateTiles(tiles);
    printf("PRESS R TO REGENERATE\n");

    // main game loop
    bool quit = false;
    while (!quit)
    {
        renderPuzzle(renderer, tiles);

        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            quit = quitCheck(&event);

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r)
            {
                generateTiles(tiles);
            }
        }
    }

    endGame(renderer, window);
    return 0;
}

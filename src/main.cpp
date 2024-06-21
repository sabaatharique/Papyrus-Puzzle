#include <iostream>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

const int WIDTH = 810, HEIGHT = 540;

char clr[7][10] = {"red", "yellow", "green", "orange", "blue", "purple", "pink"};
const int hexCodes[7][3] = {{255, 25, 0}, {255, 233, 0}, {0, 194, 0}, {255, 140, 40}, {0, 16, 255}, {134, 19, 128}, {240, 90, 100}};

class Tiles
{
public:
    int xco, yco, size;
    int colour;

    void load(int i, int j, int r)
    {
        size = HEIGHT / 10;
        colour = r;
        yco = i * size + ((HEIGHT - (4.5 * size)) / 2);
        xco = j * size + ((WIDTH - (8 * size)) / 2);
    }
};

class Player
{
public:
    float xco, yco, move;
    int width, height, frame, facing;
    bool electric, piranhas, canMove;
    // facing: right = 0, left = 1, up = 2, down = 3;

    void process(SDL_Event *event)
    {
        if (event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.sym == SDLK_RIGHT)
            {
                facing = 0;
                xco += move;
            }
            else if (event->key.keysym.sym == SDLK_LEFT)
            {
                facing = 1;
                xco -= move;
            }
            else if (event->key.keysym.sym == SDLK_UP)
            {
                facing = 2;
                yco -= move;
            }
            else if (event->key.keysym.sym == SDLK_DOWN)
            {
                facing = 3;
                yco += move;
            }
        }
    }
};

class Game
{
public:
    Tiles tiles[48];
    Player frisk;
    bool gen;

    void generateTiles(SDL_Event *event)
    {
        if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
        {
            int r;
            for (int i = 0; i < 6; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    r = (rand()) % 7;
                    tiles[(i * 8) + j].load(i, j, r);
                }
            }
            gen = true;
            reset();
        }
    }

    void initialise()
    {
        frisk.width = 50;
        frisk.height = 75;

        frisk.xco = (WIDTH / 5) - frisk.width;
        frisk.yco = (HEIGHT / 1.85) - frisk.height;
        frisk.move = 7;

        frisk.canMove = true;
        frisk.facing = 0;

        gen = false;
    }

    void reset()
    {
        frisk.xco = (WIDTH / 5) - frisk.width;
        frisk.yco = (HEIGHT / 1.85) - frisk.height;

        frisk.canMove = true;
        frisk.facing = 0;
    }

    bool tileStepped(int i)
    {
        float x1 = frisk.xco, y1 = frisk.yco;
        float w1 = frisk.width, h1 = frisk.height;

        float x2 = tiles[i].xco, y2 = tiles[i].yco;
        float w2 = tiles[i].size, h2 = tiles[i].size;

        if (x1 < (x2 + w2 - 15) && x2 < (x1 + w1 - 15) && y1 < (y2 + h2 - 60) && y2 < (y1 + h1))
            return true;

        return false;
    }

    void process()
    {
        for (int i = 0; i < 48; i++)
        {
            if (tileStepped(i))
                printf("%s\n", clr[tiles[i].colour]);
        }

        if ((frisk.xco + frisk.width) > (tiles[7].xco + tiles[7].size) && gen)
            printf("YOU WIN!\n");
    }

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
};

Game game;

class Graphics
{
public:
    SDL_Texture *background;
    SDL_Texture *frisk;

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *loadTexture(const char *file)
    {
        SDL_Texture *texture = IMG_LoadTexture(renderer, file);
        if (texture == NULL)
        {
            printf("Could not create %s texture.\nSDL Error: %s\n", file, SDL_GetError());
            SDL_Quit();
            exit(1);
        }
        return texture;
    }

    void initialise()
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
        frisk = loadTexture("graphics/frisk1.png");
    }

    void renderImage(int x, int y, int w, int h, SDL_Texture *texture)
    {
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    void renderSprite(int x, int y, int w, int h, SDL_RendererFlip direction, SDL_Texture *texture)
    {
        SDL_Rect rect = {x, y, w, h};
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, 0, NULL, direction);
    }

    void renderPuzzle()
    {
        for (int i = 0; i < 48; i++)
        {
            int x = game.tiles[i].xco;
            int y = game.tiles[i].yco;
            int size = game.tiles[i].size;
            int clr = game.tiles[i].colour;

            SDL_SetRenderDrawColor(renderer, hexCodes[clr][0], hexCodes[clr][1], hexCodes[clr][2], 255);
            SDL_Rect rect = {x, y, size, size};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    void renderGame()
    {
        renderImage(0, 0, WIDTH, HEIGHT, background);

        renderPuzzle();

        renderImage(game.frisk.xco, game.frisk.yco, game.frisk.width, game.frisk.height, frisk);

        SDL_RenderPresent(renderer);
    }

    void destroy()
    {
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(frisk);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
};

Graphics gfx;

class Typography
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

Typography txt;

// main
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    game.initialise();
    gfx.initialise();

    // main game loop
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;

        gfx.renderGame();

        game.process();

        if (SDL_PollEvent(&event))
        {
            quit = game.quitCheck(&event);

            game.generateTiles(&event);

            game.frisk.process(&event);
        }
    }

    gfx.destroy();
    SDL_Quit();
    return 0;
}

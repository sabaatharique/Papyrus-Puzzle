#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <time.h>
using namespace std;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 810
#define HEIGHT 540

class Graphics
{
public:
    SDL_Texture *background;
    SDL_Texture *frisk[12];

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *loadTexture(string file);

    void initialise();

    void renderImage(int x, int y, int w, int h, SDL_Texture *texture);

    SDL_Renderer *passRenderer();

    void destroy();
};

#endif
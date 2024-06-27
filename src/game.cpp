#include "game.hpp"
#include "graphics.hpp"
#include "mini.hpp"
#include "typo.hpp"

Graphics gfx;

Mini mini;

Typography txt;

bool QUIT;

void Tiles::load(int i, int j, int r)
{
    size = HEIGHT / 10;
    colour = r;
    monster = false;
    yco = i * size + ((HEIGHT - (4.5 * size)) / 2);
    xco = j * size + ((WIDTH - (8 * size)) / 2);
}

void Player::process()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
    {
        isMoving = true;
        facing = 0;
        if (xco + width < WIDTH)
            xco += move;
    }
    else if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
    {
        isMoving = true;
        facing = 1;
        if (xco > 0)
            xco -= move;
    }
    else if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
    {
        isMoving = true;
        facing = 2;
        if (yco > 0)
            yco -= move;
    }
    else if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
    {
        isMoving = true;
        facing = 3;
        if (yco + height < HEIGHT)
            yco += move;
    }
    else
    {
        isMoving = false;
    }
}

void Player::animate()
{
    if (isMoving)
    {
        if (facing == 0)
        {
            if (!(time % 20))
            {
                if (frame == 0)
                    frame = 1;
                else
                    frame = 0;
            }
        }
        else if (facing == 1)
        {
            if (!(time % 20))
            {
                if (frame == 2)
                    frame = 3;
                else
                    frame = 2;
            }
        }
        else if (facing == 2)
        {
            if (!(time % 20))
            {
                if (frame == 4)
                    frame = 5;
                else if (frame == 5)
                    frame = 6;
                else if (frame == 6)
                    frame = 7;
                else
                    frame = 4;
            }
        }
        else if (facing == 3)
        {
            if (!(time % 20))
            {
                if (frame == 8)
                    frame = 9;
                else if (frame == 9)
                    frame = 10;
                else if (frame == 10)
                    frame = 11;
                else
                    frame = 8;
            }
        }
    }
    else
    {
        if (facing == 0)
            frame = 0;
        else if (facing == 1)
            frame = 2;
        else if (facing == 2)
            frame = 4;
        else if (facing == 3)
            frame = 8;
    }
}

void Mini::renderMiniGame()
{
    SDL_SetRenderDrawColor(gfx.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gfx.renderer);

    SDL_SetRenderDrawColor(gfx.renderer, 255, 255, 255, 255);
    SDL_Rect rect = {80, HEIGHT - 236, WIDTH - 160, 176};
    SDL_RenderFillRect(gfx.renderer, &rect);

    SDL_SetRenderDrawColor(gfx.renderer, 0, 0, 0, 255);
    rect = {85, HEIGHT - 231, WIDTH - 170, 166};
    SDL_RenderFillRect(gfx.renderer, &rect);

    gfx.renderImage(30, HEIGHT - 510, WIDTH - 60, 274, gfx.grid);

    gfx.renderImage(froggit.xco, froggit.yco, froggit.width, froggit.height, gfx.monster[froggit.frame]);

    gfx.renderImage(90, HEIGHT - 236, WIDTH - 180, 176, gfx.target);

    gfx.renderImage(bar.xco, bar.yco, bar.width, bar.height, gfx.bar);

    SDL_RenderPresent(gfx.renderer);
}

void Typography::renderText(int x, int y, float size, string str, SDL_Color colour, TTF_Font *font, SDL_Texture *texture)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, &str[0], colour);
    if (surface == NULL)
    {
        printf("Error: could not create '%s' text surface.\n%s\n", str, TTF_GetError());
        SDL_Quit();
        exit(1);
    }
    texture = SDL_CreateTextureFromSurface(gfx.renderer, surface);
    SDL_FreeSurface(surface);

    int w = size * surface->w;
    int h = size * surface->h;
    SDL_Rect rect = {x - (w / 2), y, w, h};
    SDL_RenderCopy(gfx.renderer, texture, NULL, &rect);
}

int Game::playMiniGame()
{
    mini.initialise();

    bool win = false;
    while (!win)
    {
        SDL_Event event;

        mini.renderMiniGame();

        mini.bar.moving();

        mini.process(&event);

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
            win = true;

        if (SDL_PollEvent(&event))
            if (quitCheck(&event))
                return -1;
    }
    SDL_Delay(500);
    return (abs((WIDTH / 2) - mini.bar.xco));
}

void Game::generateTiles()
{
    int r;
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            r = (rand()) % 7;
            tiles[(i * 8) + j].load(i, j, r);

            if (r == 0)
            {
                impassableX.push_back(tiles[(i * 8) + j].xco);
                impassableY.push_back(tiles[(i * 8) + j].yco);
            }
        }
    }
    gen = true;
}

void Game::initialise()
{
    frisk.width = 50;
    frisk.height = 75;

    frisk.xco = (WIDTH / 5) - frisk.width;
    frisk.yco = (HEIGHT / 1.85) - frisk.height;
    frisk.move = 1.25;

    frisk.facing = 0;
    frisk.frame = 0;
    frisk.time = 0;

    frisk.orange = false;
    frisk.canMove = true;
    frisk.isMoving = false;
    frisk.inPuzzle = false;

    miniGame = false;
    gen = false;
    gameOver = false;
    gameWin = false;

    res = -1;

    srand(time(0));

    gfx.initialise();
    txt.initialise();
}

void Game::reset(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
    {
        frisk.xco = (WIDTH / 5) - frisk.width;
        frisk.yco = (HEIGHT / 1.85) - frisk.height;
        frisk.move = 1.25;

        frisk.canMove = true;
        frisk.orange = false;
        frisk.isMoving = false;
        frisk.inPuzzle = false;

        frisk.facing = 0;
        frisk.frame = 0;
        frisk.time = 0;

        miniGame = false;
        gameOver = false;
        gameWin = false;

        res = -1;

        impassableX.clear();
        impassableY.clear();
        generateTiles();
    }
}

bool Game::isImpassable()
{
    for (int i = 0; i < impassableX.size(); i++)
    {
        float x1 = frisk.xco, y1 = frisk.yco;
        float w1 = frisk.width, h1 = frisk.height;

        float x2 = impassableX[i], y2 = impassableY[i];
        float w2 = tiles[0].size, h2 = tiles[0].size;

        if (x1 < (x2 + w2 - 20) && x2 < (x1 + w1 - 20) && y1 < (y2 + h2 - h1) && y2 < (y1 + h1))
            return true;
    }
    return false;
}

bool Game::isOnBridge()
{
    if (frisk.xco >= WIDTH / 10 && frisk.xco < tiles[0].xco && frisk.yco >= 186 && frisk.yco <= 236)
        return true;
    return false;
}

bool Game::isInPuzzle()
{
    if (frisk.xco + (frisk.width / 3) >= tiles[0].xco && frisk.yco + (frisk.height / 1.5) >= tiles[0].yco && frisk.yco + frisk.height <= tiles[40].yco + tiles[40].size)
        return true;
    return false;
}

bool Game::tileStepped(int i)
{
    float x1 = frisk.xco, y1 = frisk.yco;
    float w1 = frisk.width, h1 = frisk.height;

    float x2 = tiles[i].xco, y2 = tiles[i].yco;
    float w2 = tiles[i].size, h2 = tiles[i].size;

    if (x1 < (x2 + w2 - 20) && x2 < (x1 + w1 - 20) && y1 < (y2 + h2 - h1) && y2 < (y1 + h1))
        return true;

    return false;
}

void Game::reflectBack()
{
    switch (frisk.facing)
    {
    case 0:
        frisk.xco -= frisk.move;
        frisk.facing = 1;
        break;
    case 1:
        frisk.xco += frisk.move;
        frisk.facing = 0;
        break;
    case 2:
        frisk.yco += frisk.move;
        frisk.facing = 3;
        break;
    case 3:
        frisk.yco -= frisk.move;
        frisk.facing = 2;
        break;
    }
}

void Game::slideAcross()
{
    switch (frisk.facing)
    {
    case 0:
        frisk.xco += 2;
        break;
    case 1:
        frisk.xco -= 2;
        break;
    case 2:
        frisk.yco -= 2;
        break;
    case 3:
        frisk.yco += 2;
        break;
    }
}

bool Game::yellowTileAround(int i)
{
    if (((tiles[i - 1].colour == 1) && (i % 8)) ||
        ((tiles[i + 1].colour == 1) && ((i + 1) % 8)) ||
        ((tiles[i + 8].colour == 1) && (i < 40)) ||
        ((tiles[i - 8].colour == 1) && (i > 7)))
        return true;
    return false;
}

bool Game::tileEffect(int i)
{
    switch (tiles[i].colour)
    {
    case 0:
        reflectBack();
        break;
    case 1:
        res = 1;
        gameOver = true;
        frisk.canMove = false;
        break;
    case 2:
        if (!tiles[i].monster)
        {
            SDL_Delay(250);
            int score = playMiniGame();
            if (score >= 90)
            {
                res = 2;
                frisk.canMove = false;
                gameOver = true;
            }
            else if (score < 0)
                return true;
            else
                frisk.canMove = true;
        }
        tiles[i].monster = true;
        break;
    case 3:
        frisk.orange = true;
        break;
    case 4:
        if (yellowTileAround(i))
        {
            res = 3;
            gameOver = true;
            frisk.canMove = false;
        }
        else if (frisk.orange)
        {
            res = 4;
            gameOver = true;
            frisk.canMove = false;
        }
        break;
    case 5:
        slideAcross();
        break;
    }
    return false;
}

bool Game::process(SDL_Event *event)
{
    frisk.time++;

    frisk.animate();

    if (!isImpassable())
        frisk.process();

    if (gen && !isOnBridge() && !isInPuzzle())
        reflectBack();

    for (int i = 0; i < 48; i++)
    {
        if (tileStepped(i))
            QUIT = tileEffect(i);
    }

    if (!frisk.canMove)
    {
        frisk.move = 0;
    }

    if ((frisk.xco) > (tiles[7].xco + tiles[7].size) && gen)
    {
        res = 0;
        gameWin = true;
        frisk.move = 0;
    }

    if (SDL_PollEvent(event))
    {
        QUIT = quitCheck(event);
    }

    return QUIT;
}

void Game::renderPuzzle()
{
    for (int i = 0; i < 48; i++)
    {
        int x = tiles[i].xco;
        int y = tiles[i].yco;
        int size = tiles[i].size;
        int clr = tiles[i].colour;

        SDL_SetRenderDrawColor(gfx.renderer, hexCodes[clr][0], hexCodes[clr][1], hexCodes[clr][2], 255);
        SDL_Rect rect = {x, y, size, size};
        SDL_RenderFillRect(gfx.renderer, &rect);
    }
}

void Game::renderGame()
{
    gfx.renderImage(0, 0, WIDTH, HEIGHT, gfx.background);

    renderPuzzle();

    gfx.renderImage(frisk.xco, frisk.yco, frisk.width, frisk.height, gfx.frisk[frisk.frame]);

    if (gameWin)
    {
        txt.renderText(WIDTH / 2, 10, 1, "YOU WIN!", {255, 255, 255}, txt.DTM, txt.text1);
        txt.renderText(WIDTH / 2, 80, 0.4, txt.res[res], {175, 175, 175}, txt.DTM, txt.text2);
    }

    if (gameOver)
    {
        txt.renderText(WIDTH / 2, 10, 1, "GAME OVER", {255, 255, 255}, txt.DTM, txt.text1);
        txt.renderText(WIDTH / 2, 80, 0.4, txt.res[res], {175, 175, 175}, txt.DTM, txt.text2);
    }

    SDL_RenderPresent(gfx.renderer);

    txt.refresh();
}

bool Game::quitCheck(SDL_Event *event)
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

void Game::end()
{
    gfx.destroy();
    txt.destroy();
}

void Game::playGame()
{
    // main game loop
    QUIT = false;
    while (!QUIT)
    {
        SDL_Event event;

        renderGame();

        QUIT = process(&event);

        reset(&event);
    }
}
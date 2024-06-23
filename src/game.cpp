#include "game.hpp"
#include "graphics.hpp"

Graphics gfx;

void Tiles::load(int i, int j, int r)
{
    size = HEIGHT / 10;
    colour = r;
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

    gen = false;

    gfx.initialise();
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
    if (tiles[i - 1].colour == 1 ||
        tiles[i + 1].colour == 1 ||
        tiles[i + 8].colour == 1 ||
        tiles[i - 8].colour == 1)
        return true;
    return false;
}

void Game::tileEffect(int i)
{
    switch (tiles[i].colour)
    {
    case 0:
        reflectBack();
        // frisk.canMove = false;
        break;
    case 1:
        printf("YOU GOT SHOCKED! GAME OVER\nPRESS SPACE TO RESTART\n");
        frisk.canMove = false;
        break;
    case 2:
        printf("MONSTER!\n");
        // insert minigame (pls kill me)
        frisk.canMove = true;
        break;
    case 3:
        frisk.orange = true;
        printf("YOU SMELL OF ORANGES!\n");
        frisk.canMove = true;
        break;
    case 4:
        if (yellowTileAround(i))
        {
            printf("YOU GOT SHOCKED IN WATER! GAME OVER\nPRESS SPACE TO RESTART\n");
            frisk.canMove = false;
        }
        else if (frisk.orange)
        {
            printf("YOU WERE EATEN BY PIRANHAS! GAME OVER\nPRESS SPACE TO RESTART\n");
            frisk.canMove = false;
        }
        break;
    case 5:
        slideAcross();
        frisk.canMove = true;
        break;
    default:
        frisk.canMove = true;
    }
}

void Game::process(SDL_Event *event)
{
    frisk.time++;

    frisk.animate();

    if (!isImpassable())
        frisk.process();

    if (!frisk.canMove)
        frisk.move = 0;

    if (frisk.xco >= tiles[0].xco)
        frisk.inPuzzle = true;
    else
        frisk.inPuzzle = false;

    for (int i = 0; i < 48; i++)
    {
        if (tileStepped(i))
        {
            tileEffect(i);
        }
    }

    if ((frisk.xco) > (tiles[7].xco + tiles[7].size) && gen)
    {
        printf("YOU WIN!\nPRESS SPACE TO RESTART\n");
        frisk.move = 0;
    }

    reset(event);
}

void Game::renderPuzzle()
{
    for (int i = 0; i < 48; i++)
    {
        int x = tiles[i].xco;
        int y = tiles[i].yco;
        int size = tiles[i].size;
        int clr = tiles[i].colour;

        SDL_SetRenderDrawColor(gfx.passRenderer(), hexCodes[clr][0], hexCodes[clr][1], hexCodes[clr][2], 255);
        SDL_Rect rect = {x, y, size, size};
        SDL_RenderFillRect(gfx.passRenderer(), &rect);
    }
}

void Game::renderGame()
{
    gfx.renderImage(0, 0, WIDTH, HEIGHT, gfx.background);

    renderPuzzle();

    gfx.renderImage(frisk.xco, frisk.yco, frisk.width, frisk.height, gfx.frisk[frisk.frame]);

    SDL_RenderPresent(gfx.passRenderer());
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
}

void Game::playGame()
{
    initialise();

    // main game loop
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;

        renderGame();

        process(&event);

        if (SDL_PollEvent(&event))
        {
            quit = quitCheck(&event);
        }
    }

    end();
}
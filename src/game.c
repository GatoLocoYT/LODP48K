#include "game.h"
#include "renderer.h"
#include "../tools/generated/assets.h"
#include <SDL2/SDL.h>

bool Game_Init(void)
{
    return Renderer_Init();
}

void Game_Run(void)
{
    bool running = true;

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
                break;
            }
        }

        Renderer_Clear();

        Renderer_DrawSprite(
            10,
            10,
            0,
            0,
            gSpriteSheet);
        Renderer_Present();
    }
}

void Game_Quit(void)
{
    Renderer_Quit();
}
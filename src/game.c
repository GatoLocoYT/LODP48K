#include "game.h"

#include <SDL2/SDL.h>
#include <stdio.h>

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;

bool Game_Init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow(
        "LODP48K",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_SHOWN
    );

    if (gWindow == NULL)
    {
        printf("Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    gRenderer = SDL_CreateRenderer(
        gWindow,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (gRenderer == NULL)
    {
        printf("Renderer Error: %s\n", SDL_GetError());

        SDL_DestroyWindow(gWindow);
        SDL_Quit();

        return false;
    }

    return true;
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

        SDL_SetRenderDrawColor(
            gRenderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(gRenderer);

        SDL_RenderPresent(gRenderer);
    }
}

void Game_Quit(void)
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    SDL_Quit();
}
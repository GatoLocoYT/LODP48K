#include "input.h"

#include <SDL2/SDL.h>

static bool gLeft  = false;
static bool gRight = false;
static bool gUp    = false;
static bool gDown  = false;

static bool gQuit  = false;

void Input_Update(void)
{
    SDL_Event event;

    gLeft  = false;
    gRight = false;
    gUp    = false;
    gDown  = false;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                gQuit = true;
                break;

            case SDL_KEYDOWN:

                switch(event.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        gLeft = true;
                        break;

                    case SDLK_RIGHT:
                        gRight = true;
                        break;

                    case SDLK_UP:
                        gUp = true;
                        break;

                    case SDLK_DOWN:
                        gDown = true;
                        break;

                    case SDLK_ESCAPE:
                        gQuit = true;
                        break;
                }

                break;
        }
    }
}

bool Input_Left(void)
{
    return gLeft;
}

bool Input_Right(void)
{
    return gRight;
}

bool Input_Up(void)
{
    return gUp;
}

bool Input_Down(void)
{
    return gDown;
}

bool Input_Quit(void)
{
    return gQuit;
}
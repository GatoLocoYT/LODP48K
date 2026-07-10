#include "input.h"

#include <SDL2/SDL.h>

static bool gLeft  = false;
static bool gRight = false;
static bool gUp    = false;
static bool gDown  = false;

static bool gQuit = false;

void Input_Update(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                gQuit = true;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    gQuit = true;
                }
                break;
        }
    }

    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    gLeft  = keyboard[SDL_SCANCODE_LEFT];
    gRight = keyboard[SDL_SCANCODE_RIGHT];
    gUp    = keyboard[SDL_SCANCODE_UP];
    gDown  = keyboard[SDL_SCANCODE_DOWN];
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
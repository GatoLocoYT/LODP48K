#include "input.h"

#include <SDL2/SDL.h>

static bool gLeft = false;
static bool gRight = false;
static bool gUp = false;
static bool gDown = false;
static bool gShoot = false;
static bool gStart = false;
static bool gQuit = false;

void Input_Update(void)
{
    SDL_Event event;
    gShoot = false;
    gStart = false;

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
            else if ((event.key.keysym.sym == SDLK_RETURN ||
                      event.key.keysym.sym == SDLK_KP_ENTER) &&
                     event.key.repeat == 0)
            {
                gStart = true;
            }
            break;
        }
    }

    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    gLeft = keyboard[SDL_SCANCODE_LEFT];
    gRight = keyboard[SDL_SCANCODE_RIGHT];
    gUp = keyboard[SDL_SCANCODE_UP];
    gDown = keyboard[SDL_SCANCODE_DOWN];

    gShoot = keyboard[SDL_SCANCODE_SPACE];
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
bool Input_Shoot(void)
{
    return gShoot;
}

bool Input_Start(void)
{
    return gStart;
}

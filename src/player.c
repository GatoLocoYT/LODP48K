#include "player.h"
#include "bullet.h"
#include "input.h"
#include "renderer.h"
#include "room.h"

#include "../tools/generated/assets.h"
#include <SDL2/SDL.h>
#include "bullet.h"

static Uint32 gLastShotTime = 0;

#define FIRE_DELAY 120
static int gPlayerX = 8;
static int gPlayerY = 8;

static Direction gDirection = DIR_RIGHT;

#include <SDL2/SDL.h>

static Uint32 gLastMoveTime = 0;

#define PLAYER_MOVE_DELAY 140

bool Player_Init(void)
{
    gPlayerX = 8;
    gPlayerY = 8;

    gDirection = DIR_RIGHT;

    gLastMoveTime = SDL_GetTicks();
    gLastShotTime = SDL_GetTicks();

    return true;
}

void Player_Update(void)
{
    Uint32 now = SDL_GetTicks();

    if (now - gLastMoveTime < PLAYER_MOVE_DELAY)
    {
        return;
    }

    gLastMoveTime = now;
    if (Input_Left())
    {
        gDirection = DIR_LEFT;

        if (Room_IsWalkable(gPlayerX - 1, gPlayerY))
        {
            gPlayerX--;
        }
    }
    else if (Input_Right())
    {
        gDirection = DIR_RIGHT;

        if (Room_IsWalkable(gPlayerX + 1, gPlayerY))
        {
            gPlayerX++;
        }
    }
    else if (Input_Up())
    {
        gDirection = DIR_UP;

        if (Room_IsWalkable(gPlayerX, gPlayerY - 1))
        {
            gPlayerY--;
        }
    }
    else if (Input_Down())
    {
        gDirection = DIR_DOWN;

        if (Room_IsWalkable(gPlayerX, gPlayerY + 1))
        {
            gPlayerY++;
        }
    }

    if (gPlayerX < 0)
    {
        Room_LoadRandom();
        gPlayerX = ROOM_WIDTH - 2;
    }

    if (gPlayerX >= ROOM_WIDTH)
    {
        Room_LoadRandom();
        gPlayerX = 1;
    }

    if (gPlayerY < 0)
    {
        Room_LoadRandom();
        gPlayerY = ROOM_HEIGHT - 2;
    }

    if (gPlayerY >= ROOM_HEIGHT)
    {
        Room_LoadRandom();
        gPlayerY = 1;
    }
    if (Input_Shoot())
    {
        if (now - gLastShotTime >= FIRE_DELAY)
        {
            Bullet_Shoot(
                gPlayerX,
                gPlayerY,
                gDirection);

            gLastShotTime = now;
        }
    }
}

void Player_Draw(void)
{
    Renderer_DrawSpriteEx(
        gPlayerX,
        gPlayerY,
        0,
        0,
        gDirection,
        gSpriteSheet);
}

int Player_GetX(void)
{
    return gPlayerX;
}

int Player_GetY(void)
{
    return gPlayerY;
}
#include "player.h"

#include <SDL2/SDL.h>

#include "bullet.h"
#include "input.h"
#include "renderer.h"
#include "room.h"

#define FIRE_DELAY 120
#define PLAYER_MAX_HP 5
#define PLAYER_INVULNERABILITY_TIME 3000
#define PLAYER_MOVE_DELAY 140

static int gPlayerX = 8;
static int gPlayerY = 8;
static int gPlayerHP = PLAYER_MAX_HP;

static Direction gDirection = DIR_RIGHT;

static Uint32 gLastMoveTime = 0;
static Uint32 gLastShotTime = 0;
static Uint32 gLastDamageTime = 0;

void Player_Init(void)
{
    Uint32 now = SDL_GetTicks();

    gPlayerX = 8;
    gPlayerY = 8;
    gPlayerHP = PLAYER_MAX_HP;

    gDirection = DIR_RIGHT;

    gLastMoveTime = now;
    gLastShotTime = now;
    gLastDamageTime =
        now -
        PLAYER_INVULNERABILITY_TIME;
}

bool Player_Update(void)
{
    Uint32 now = SDL_GetTicks();

    if (now - gLastMoveTime < PLAYER_MOVE_DELAY)
    {
        return false;
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

    if (gPlayerX < 0 ||
        gPlayerX >= ROOM_WIDTH ||
        gPlayerY < 0 ||
        gPlayerY >= ROOM_HEIGHT)
    {
        return true;
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

    return false;
}

void Player_EnterRoom(void)
{
    if (gPlayerX < 0)
    {
        gPlayerX = ROOM_WIDTH - 2;
    }

    if (gPlayerX >= ROOM_WIDTH)
    {
        gPlayerX = 1;
    }

    if (gPlayerY < 0)
    {
        gPlayerY = ROOM_HEIGHT - 2;
    }

    if (gPlayerY >= ROOM_HEIGHT)
    {
        gPlayerY = 1;
    }
}

void Player_Draw(void)
{
    Renderer_DrawAssetSpriteEx(
        gPlayerX,
        gPlayerY,
        0,
        0,
        gDirection);
}

void Player_Damage(void)
{
    if (gPlayerHP <= 0)
    {
        return;
    }

    Uint32 now = SDL_GetTicks();

    if (now - gLastDamageTime <
        PLAYER_INVULNERABILITY_TIME)
    {
        return;
    }

    gPlayerHP--;
    gLastDamageTime = now;
}

int Player_GetX(void)
{
    return gPlayerX;
}

int Player_GetY(void)
{
    return gPlayerY;
}

int Player_GetHP(void)
{
    return gPlayerHP;
}

#include "player.h"

#include "input.h"
#include "renderer.h"
#include "room.h"

#include "../tools/generated/assets.h"

static int gPlayerX = 8;
static int gPlayerY = 8;

bool Player_Init(void)
{
    gPlayerX = 8;
    gPlayerY = 8;

    return true;
}

void Player_Update(void)
{
    if (Input_Left())
    {
        if (Room_IsWalkable(gPlayerX - 1, gPlayerY))
        {
            gPlayerX--;
        }
    }
    else if (Input_Right())
    {
        if (Room_IsWalkable(gPlayerX + 1, gPlayerY))
        {
            gPlayerX++;
        }
    }
    else if (Input_Up())
    {
        if (Room_IsWalkable(gPlayerX, gPlayerY - 1))
        {
            gPlayerY--;
        }
    }
    else if (Input_Down())
    {
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
}

void Player_Draw(void)
{
    Renderer_DrawSprite(
        gPlayerX,
        gPlayerY,
        0,
        0,
        gSpriteSheet
    );
}

int Player_GetX(void)
{
    return gPlayerX;
}

int Player_GetY(void)
{
    return gPlayerY;
}
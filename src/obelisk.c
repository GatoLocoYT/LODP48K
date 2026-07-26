#include "obelisk.h"

#include <stdlib.h>

#include "player.h"
#include "renderer.h"
#include "room.h"

#define OBELISK_MIN_ROOMS 3
#define OBELISK_SPAWN_CHANCE 5
#define OBELISK_SPAWN_ATTEMPTS 100

#define OBELISK_SPRITE_X 2
#define OBELISK_SPRITE_Y 0

static bool gObeliskActive = false;
static int gObeliskX = 0;
static int gObeliskY = 0;

static bool Obelisk_CanSpawnAt(
    int x,
    int y)
{
    if (!Room_IsWalkable(x, y))
    {
        return false;
    }

    if (x == Player_GetX() &&
        y == Player_GetY())
    {
        return false;
    }

    return true;
}

void Obelisk_Spawn(int roomsPassed)
{
    Obelisk_Clear();

    if (roomsPassed < OBELISK_MIN_ROOMS ||
        rand() % 100 >= OBELISK_SPAWN_CHANCE)
    {
        return;
    }

    for (int i = 0;
         i < OBELISK_SPAWN_ATTEMPTS;
         i++)
    {
        int x = rand() % ROOM_WIDTH;
        int y = rand() % ROOM_HEIGHT;

        if (!Obelisk_CanSpawnAt(x, y))
        {
            continue;
        }

        gObeliskX = x;
        gObeliskY = y;
        gObeliskActive = true;
        return;
    }
}

void Obelisk_Clear(void)
{
    gObeliskActive = false;
}

void Obelisk_Draw(void)
{
    if (!gObeliskActive)
    {
        return;
    }

    Renderer_DrawAssetSprite(
        gObeliskX,
        gObeliskY,
        OBELISK_SPRITE_X,
        OBELISK_SPRITE_Y);
}

bool Obelisk_IsAt(
    int x,
    int y)
{
    return gObeliskActive &&
           gObeliskX == x &&
           gObeliskY == y;
}

#include "raycast.h"

#include "enemy.h"
#include "room.h"

#define TILE_SIZE 8

RaycastResult Raycast_Cast(
    int pixelX,
    int pixelY)
{
    RaycastResult result =
    {
        RAYCAST_HIT_NONE,
        -1
    };

    if (pixelX < 0 ||
        pixelX >= ROOM_WIDTH * TILE_SIZE ||
        pixelY < 0 ||
        pixelY >= ROOM_HEIGHT * TILE_SIZE)
    {
        result.hit = RAYCAST_HIT_WALL;
        return result;
    }

    int tileX = pixelX / TILE_SIZE;
    int tileY = pixelY / TILE_SIZE;

    if (!Room_IsWalkable(tileX, tileY))
    {
        result.hit = RAYCAST_HIT_WALL;
        return result;
    }

    result.enemyIndex = Enemy_FindAt(tileX, tileY);

    if (result.enemyIndex >= 0)
    {
        result.hit = RAYCAST_HIT_ENEMY;
    }

    return result;
}

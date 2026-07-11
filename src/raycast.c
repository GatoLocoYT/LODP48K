#include "raycast.h"

#include "room.h"

bool Raycast_HitWall(
    int pixelX,
    int pixelY)
{
    int tileX = pixelX / 8;
    int tileY = pixelY / 8;

    if (tileX < 0 ||
        tileX >= ROOM_WIDTH ||
        tileY < 0 ||
        tileY >= ROOM_HEIGHT)
    {
        return true;
    }

    return !Room_IsWalkable(
        tileX,
        tileY);
}
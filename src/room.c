#include "room.h"

#include <stdlib.h>

#include "renderer.h"

#include "../tools/generated/assets.h"
#include "../tools/generated/rooms.h"

static const uint8_t* gCurrentRoom = NULL;
static int gCurrentRoomIndex = -1;

void Room_Load(int index)
{
    gCurrentRoomIndex = index;
    gCurrentRoom = gRooms[index];
}

const uint8_t* Room_GetCurrent(void)
{
    return gCurrentRoom;
}

void Room_Draw(void)
{
    if (gCurrentRoom == NULL)
    {
        return;
    }

    for (int y = 0; y < ROOM_HEIGHT; y++)
    {
        for (int x = 0; x < ROOM_WIDTH; x++)
        {
            uint8_t tile =
                gCurrentRoom[y * ROOM_WIDTH + x];

            switch (tile)
            {
                case TILE_WALL:

                    Renderer_DrawSprite(
                        x,
                        y,
                        3,
                        0,
                        gSpriteSheet
                    );

                    break;

                case TILE_EMPTY:
                default:
                    break;
            }
        }
    }
}

bool Room_IsWalkable(
    int x,
    int y)
{
    if (gCurrentRoom == NULL)
    {
        return false;
    }

    // Permitimos salir del mapa.
    // El cambio de habitación se maneja en game.c.
    if (x < 0 || x >= ROOM_WIDTH)
    {
        return true;
    }

    if (y < 0 || y >= ROOM_HEIGHT)
    {
        return true;
    }

    uint8_t tile =
        gCurrentRoom[y * ROOM_WIDTH + x];

    return tile == TILE_EMPTY;
}

void Room_LoadRandom(void)
{
    int index;

    do
    {
        index = rand() % ROOM_COUNT;
    }
    while (index == gCurrentRoomIndex);

    Room_Load(index);
}
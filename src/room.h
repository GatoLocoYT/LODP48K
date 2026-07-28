#ifndef ROOM_H
#define ROOM_H

#include <stdbool.h>

#define ROOM_WIDTH 16
#define ROOM_HEIGHT 16

#define TILE_EMPTY 0
#define TILE_WALL 1

void Room_LoadRandom(void);

void Room_Draw(void);

bool Room_IsWalkable(
    int x,
    int y
);

#endif

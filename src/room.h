#ifndef ROOM_H
#define ROOM_H

#include <stdint.h>
#include <stdbool.h>

#include "../tools/generated/rooms.h"

void Room_Load(int index);

void Room_LoadRandom(void);

void Room_Draw(void);

bool Room_IsWalkable(
    int x,
    int y
);

const uint8_t* Room_GetCurrent(void);

#endif
#ifndef OBELISK_H
#define OBELISK_H

#include <stdbool.h>

void Obelisk_Spawn(int roomsPassed);

void Obelisk_Clear(void);

void Obelisk_Draw(void);

bool Obelisk_IsAt(
    int x,
    int y
);

#endif

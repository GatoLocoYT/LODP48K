#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>

#include "direction.h"

bool Bullet_Init(void);

void Bullet_Update(void);

void Bullet_Draw(void);

void Bullet_Shoot(
    int startX,
    int startY,
    Direction direction
);

bool Bullet_IsActive(void);

#endif
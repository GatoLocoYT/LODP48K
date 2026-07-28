#ifndef BULLET_H
#define BULLET_H

#include "direction.h"

void Bullet_Init(void);

int Bullet_Update(void);

void Bullet_Draw(void);

void Bullet_Shoot(
    int startX,
    int startY,
    Direction direction
);

#endif
